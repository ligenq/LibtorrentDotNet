#pragma once

#pragma managed(push, off)
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/torrent_status.hpp>
#pragma managed(pop)

#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Threading;
using namespace System::IO;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

namespace LibtorrentDotNet
{
	/// <summary>
	/// Provides a Stream implementation for reading torrent files while they are being downloaded.
	/// This class enables streaming of media files before the complete download has finished.
	/// </summary>
	public ref class TorrentStream sealed : public Stream, IDisposable
	{
	private:
		static initonly Int32 MinBufferSize = 4 * 1024 * 1024;
		static initonly Int32 DynamicBufferSize = 8 * 1024 * 1024;
		static initonly Int32 MinimumPlaybackBuffer = 2 * 1024 * 1024;
		static initonly Int32 MaxWaitTimeMs = 10000;
		static initonly Int32 CheckIntervalMs = 50;
		static initonly Int32 InitialPiecesToPrioritize = 30;
		static initonly Int32 MinReadAheadPieces = 10;
		static initonly Int32 MaxReadAheadPieces = 30;
		static initonly double ReadRateSmoothing = 0.3;

		const libtorrent::torrent_handle* torrentHandle;
		const TimeSpan readTimeout;
		int64_t position;
		int64_t length;
		Object^ syncRoot;
		bool disposed;
		Int32 pieceLength;
		Int32 totalPieces;
		FileStream^ fileStream;
		array<Byte>^ buffer;
		int64_t bufferStart;
		Int32 bufferLength;
		double averageReadRate;
		DateTime lastReadTime;
		int64_t lastReadPosition;
		int lastPrioritizedPiece;

		TorrentStream(const libtorrent::torrent_handle* torrentHandleParam, TimeSpan readTimeoutParam) :
			torrentHandle(torrentHandleParam),
			readTimeout(readTimeoutParam),
			position(0),
			syncRoot(gcnew Object()),
			disposed(false),
			buffer(nullptr),
			bufferStart(0),
			bufferLength(0),
			averageReadRate(0),
			lastPrioritizedPiece(-1),
			lastReadTime(DateTime::Now),
			lastReadPosition(0)
		{
		}

	internal:
		/// <summary>
		/// Creates a new instance of TorrentStream for the specified torrent handle and file index.
		/// </summary>
		static TorrentStream^ Create(const libtorrent::torrent_handle* torrentHandle,
			const libtorrent::file_index_t& fileIndex, TimeSpan readTimeout)
		{
			if (!torrentHandle)
			{
				throw gcnew ArgumentNullException("torrentHandle");
			}

			try
			{
				const auto& torrentInfo = torrentHandle->torrent_file();
				if (!torrentInfo)
				{
					throw gcnew InvalidOperationException("Invalid torrent: No torrent info available");
				}

				const auto& fileStorage = torrentInfo->files();
				if (fileIndex.operator int() < 0 || fileIndex.operator int() >= fileStorage.num_files())
				{
					throw gcnew ArgumentOutOfRangeException("fileIndex", "File index is out of range");
				}

				const std::string& filePath = torrentHandle->status().save_path + "/" + fileStorage.file_path(
					libtorrent::file_index_t(fileIndex));

				if (!File::Exists(gcnew String(filePath.c_str())))
				{
					throw gcnew FileNotFoundException(
						String::Format("File not found: {0}", gcnew String(filePath.c_str())));
				}

				auto stream = gcnew TorrentStream(torrentHandle, readTimeout);

				stream->pieceLength = torrentInfo->piece_length();
				stream->totalPieces = torrentInfo->num_pieces();
				stream->length = fileStorage.file_size(libtorrent::file_index_t(fileIndex));

				torrentHandle->set_flags(libtorrent::torrent_flags::sequential_download);

				const auto& firstPiece = fileStorage.map_file(libtorrent::file_index_t(fileIndex), 0, 1);
				const auto& lastPiece = fileStorage.
					map_file(libtorrent::file_index_t(fileIndex), stream->length - 1, 1);

				int piecesToPrioritize = Math::Min(
					lastPiece.piece.operator int() - firstPiece.piece.operator int() + 1,
					InitialPiecesToPrioritize);

				for (int i = 0; i < piecesToPrioritize; ++i)
				{
					torrentHandle->piece_priority(libtorrent::piece_index_t(firstPiece.piece.operator int() + i),
						libtorrent::top_priority);
					torrentHandle->set_piece_deadline(libtorrent::piece_index_t(firstPiece.piece.operator int() + i),
						i * 50);
				}

				stream->fileStream = gcnew FileStream(
					gcnew String(filePath.c_str()),
					FileMode::Open,
					FileAccess::Read,
					FileShare::ReadWrite,
					MinBufferSize);

				stream->buffer = gcnew array<Byte>(DynamicBufferSize);

				return stream;
			}
			catch (Exception^ ex)
			{
				throw gcnew InvalidOperationException("Failed to create TorrentStream instance", ex);
			}
		}

	public:
		/// <summary>
		/// Occurs when the stream starts buffering data.
		/// </summary>
		event EventHandler^ BufferingStarted;

		/// <summary>
		/// Occurs when the stream completes buffering data.
		/// </summary>
		event EventHandler^ BufferingCompleted;

		/// <summary>
		/// Occurs when reading the stream if not enough data was available within the timeframe specified by <see cref="readTimeout"/> and 0 was returned by the stream's read method.
		/// </summary>
		event EventHandler^ ReadTimeout;

		/// <summary>
		/// Gets a value indicating whether the stream supports reading.
		/// </summary>
		virtual property bool CanRead { bool get() override { return true; } }

		/// <summary>
		/// Gets a value indicating whether the stream supports writing.
		/// </summary>
		virtual property bool CanWrite { bool get() override { return false; } }

		/// <summary>
		/// Gets a value indicating whether the stream supports seeking.
		/// </summary>
		virtual property bool CanSeek { bool get() override { return true; } }

		/// <summary>
		/// Gets the length of the stream in bytes.
		/// </summary>
		virtual property int64_t Length { int64_t get() override { return length; } }

		/// <summary>
		/// Gets or sets the current position in the stream.
		/// </summary>
		virtual property int64_t Position
		{
			int64_t get() override { return position; }
			void set(int64_t value) override { Seek(value, SeekOrigin::Begin); }
		}

		/// <summary>
		/// Reads a sequence of bytes from the stream and advances the position within the stream by the number of bytes read.
		/// </summary>
		/// <param name="outputBuffer">An array of bytes. When this method returns, the buffer contains the specified byte array with the values between offset and (offset + count - 1) replaced by the bytes read from the current source.</param>
		/// <param name="offset">The zero-based byte offset in buffer at which to begin storing the data read from the current stream.</param>
		/// <param name="count">The maximum number of bytes to be read from the current stream.</param>
		/// <returns>The total number of bytes read into the buffer.</returns>
		int Read(array<Byte>^ outputBuffer, int offset, int count) override
		{
			if (disposed)
			{
				throw gcnew ObjectDisposedException("TorrentStream");
			}

			Monitor::Enter(syncRoot);
			try
			{
				if (position >= length)
					return 0;

				int bytesRead = 0;
				DateTime startTime = DateTime::Now;
				bool isCurrentlyBuffering = false;

				while (bytesRead < count && DateTime::Now - startTime < readTimeout)
				{
					try
					{
						if (!IsInBuffer(position))
						{
							int currentPiece = static_cast<int>(position / pieceLength);

							// Start buffering if we need to wait for pieces
							if (!VerifyPieceAvailability(currentPiece, CalculateReadAheadPieces()) && !isCurrentlyBuffering)
							{
								isCurrentlyBuffering = true;
								BufferingStarted(this, EventArgs::Empty);
							}

							while (!VerifyPieceAvailability(currentPiece, CalculateReadAheadPieces()) && DateTime::Now - startTime < readTimeout)
							{
								Thread::Sleep(CheckIntervalMs);
							}

							// Signal completion if we were buffering
							if (isCurrentlyBuffering)
							{
								isCurrentlyBuffering = false;
								BufferingCompleted(this, EventArgs::Empty);
							}

							PreloadBuffer();
						}

						int64_t bufferOffset = position - bufferStart;
						int toRead = Math::Min(count - bytesRead, bufferLength - static_cast<int>(bufferOffset));

						Array::Copy(buffer, static_cast<int>(bufferOffset), outputBuffer, offset + bytesRead, toRead);
						bytesRead += toRead;
						position += toRead;

						if (position >= length) break;
					}
					catch (IOException^)
					{
						Thread::Sleep(CheckIntervalMs);
					}
				}

				// Update read rate for adaptive buffering
				UpdateReadRate(bytesRead);

				if (bytesRead == 0)
				{
					ReadTimeout(this, EventArgs::Empty);
				}

				return bytesRead;
			}
			finally
			{
				Monitor::Exit(syncRoot);
			}
		}

		/// <summary>
		/// Sets the position within the current stream.
		/// </summary>
		/// <param name="offset">A byte offset relative to the origin parameter.</param>
		/// <param name="origin">A value of type SeekOrigin indicating the reference point used to obtain the new position.</param>
		/// <returns>The new position within the current stream.</returns>
		int64_t Seek(int64_t offset, SeekOrigin origin) override
		{
			if (disposed)
			{
				throw gcnew ObjectDisposedException("TorrentStream");
			}

			Monitor::Enter(syncRoot);
			try
			{
				switch (origin)
				{
				case SeekOrigin::Begin:
					position = offset;
					break;
				case SeekOrigin::Current:
					position += offset;
					break;
				case SeekOrigin::End:
					position = length + offset;
					break;
				default:
					throw gcnew ArgumentException("Invalid SeekOrigin.");
				}

				if (position < 0 || position > length)
					throw gcnew ArgumentOutOfRangeException("Offset is out of range.");

				return position;

			}
			finally
			{
				Monitor::Exit(syncRoot);
			}
		}

		/// <summary>
		/// Not supported. Throws NotSupportedException.
		/// </summary>
		void Write(array<Byte>^ buffer, int offset, int count) override
		{
			throw gcnew NotSupportedException("Write operation is not supported.");
		}

		/// <summary>
		/// Not supported. Throws NotSupportedException.
		/// </summary>
		void SetLength(int64_t value) override
		{
			throw gcnew NotSupportedException("SetLength is not supported.");
		}

		/// <summary>
		/// Clears all buffers for this stream and causes any buffered data to be written to the underlying device.
		/// </summary>
		void Flush() override
		{
		}

	protected:
		!TorrentStream()
		{
			CleanupResources(false);
		}

		~TorrentStream()
		{
			CleanupResources(true);
		}

	private:
		void UpdateReadRate(int bytesRead)
		{
			auto currentTime = DateTime::Now;
			auto elapsed = (currentTime - lastReadTime).TotalSeconds;
			if (elapsed > 0)
			{
				double currentRate = bytesRead / elapsed;
				averageReadRate = (averageReadRate * (1 - ReadRateSmoothing)) + (currentRate * ReadRateSmoothing);
				lastReadTime = currentTime;
				lastReadPosition = position;
			}
		}

		Int32 CalculateReadAheadPieces()
		{
			// Calculate pieces needed for 10 seconds of playback at the current average read rate
			int piecesForTenSeconds = static_cast<int>((averageReadRate * 10) / pieceLength);

			// Limit the range to sensible bounds
			return Math::Min(MaxReadAheadPieces, Math::Max(MinReadAheadPieces, piecesForTenSeconds));
		}

		void UpdatePiecePriorities(int currentPiece)
		{
			if (disposed || !torrentHandle) return;

			int readAheadCount = CalculateReadAheadPieces();
			static const int TopPriority = 7; // libtorrent::top_priority
			static const int NormalPriority = 4; // libtorrent::normal_priority

			// Determine the range of pieces to prioritize
			int startPriorityPiece = Math::Max(0, currentPiece - 1); // One piece back for safety
			int endPriorityPiece = Math::Min(totalPieces - 1, currentPiece + readAheadCount);

			// Skip recalculating priorities if the range hasn't changed significantly
			if (startPriorityPiece == lastPrioritizedPiece && endPriorityPiece == lastPrioritizedPiece + readAheadCount)
			{
				return;
			}

			// Reset priorities for pieces outside the new range
			if (lastPrioritizedPiece >= 0)
			{
				int previousEnd = lastPrioritizedPiece + readAheadCount;
				for (int i = lastPrioritizedPiece; i <= previousEnd; i++)
				{
					if (i < totalPieces)
					{
						torrentHandle->piece_priority(libtorrent::piece_index_t(i), libtorrent::default_priority);
					}
				}
			}

			// Update priorities for the current range
			for (int i = startPriorityPiece; i <= endPriorityPiece; i++)
			{
				if (i < totalPieces)
				{
					// Priority decays based on distance from current piece
					int priority = Math::Max(NormalPriority, TopPriority - ((i - currentPiece) / 5));
					torrentHandle->piece_priority(libtorrent::piece_index_t(i),
						static_cast<libtorrent::download_priority_t>(priority));

					// Apply progressive deadlines
					int deadline = (i - currentPiece) * (30 + (i - currentPiece) * 2); // Progressive deadline growth
					torrentHandle->set_piece_deadline(libtorrent::piece_index_t(i), deadline);
				}
			}

			// Update the last prioritized piece range
			lastPrioritizedPiece = currentPiece;
		}

		void PreloadBuffer()
		{
			int startPiece = static_cast<int>(position / pieceLength);
			int elapsedTime = 0;
			int readAheadCount = CalculateReadAheadPieces();

			// Predict next buffer position based on read rate
			int64_t predictedPosition = position + static_cast<int64_t>(averageReadRate * 2.0);
			int predictedPiece = static_cast<int>(predictedPosition / pieceLength);

			UpdatePiecePriorities(predictedPiece);

			while (elapsedTime < MaxWaitTimeMs)
			{
				int consecutiveAvailable = GetConsecutiveAvailablePieces(startPiece);

				// Break if we have enough consecutive pieces for smooth playback
				if (consecutiveAvailable >= Math::Min(5, readAheadCount / 2)) 
				{
					break;
				}

				Thread::Sleep(CheckIntervalMs);
				elapsedTime += CheckIntervalMs;
			}

			// Use larger buffer size for high read rates
			int effectiveBufferSize = averageReadRate > 1024 * 1024 ? DynamicBufferSize : MinBufferSize;

			fileStream->Position = position;
			bufferStart = position;
			bufferLength = fileStream->Read(buffer, 0, effectiveBufferSize);
		}

		bool VerifyPieceAvailability(int startPiece, int count)
		{
			int consecutiveAvailable = 0;
			for (int i = 0; i < count; i++)
			{
				if (torrentHandle->have_piece(libtorrent::piece_index_t(startPiece + i)))
				{
					consecutiveAvailable++;
					if (consecutiveAvailable >= 3) // We have at least 3 consecutive pieces
					{
						return true;
					}
				}
				else
				{
					consecutiveAvailable = 0;
				}
			}
			return false;
		}

		int GetConsecutiveAvailablePieces(int startPiece)
		{
			int consecutive = 0;
			int maxConsecutive = 0;

			for (int i = 0; i < totalPieces - startPiece; i++)
			{
				if (torrentHandle->have_piece(libtorrent::piece_index_t(startPiece + i))) 
				{
					consecutive++;
					maxConsecutive = Math::Max(maxConsecutive, consecutive);
				}
				else
				{
					consecutive = 0;
				}
			}

			return maxConsecutive;
		}

		bool IsInBuffer(int64_t position)
		{
			return position >= bufferStart && position < bufferStart + bufferLength;
		}

		void CleanupResources(bool disposing)
		{
			if (!disposed)
			{
				if (disposing)
				{
					if (fileStream != nullptr)
					{
						fileStream->Close();
						delete fileStream;
					}

					buffer = nullptr;
				}

				if (torrentHandle != nullptr)
				{
					delete torrentHandle;
					torrentHandle = nullptr;
				}

				disposed = true;
			}
		}
	};
}
