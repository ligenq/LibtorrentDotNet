#pragma once

#pragma managed(push, off)
#include <cstring>    
#include <exception>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/info_hash.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/torrent_status.hpp>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
#pragma managed(pop)

#include <msclr/marshal_cppstd.h>
#include "TorrentOperationEvent.h"
#include "TorrentState.h"
#include "Optional.h"
#include "Utilities.h"
#include "TorrentId.h"
#include "TorrentStatus.h"
#include "TorrentInfo.h"
#include "TorrentFile.h"
#include "TorrentSessionConfig.h"
#include "AddTorrentRequest.h"
#include "TorrentEvents.h"

using namespace System;
using namespace msclr::interop;
using namespace System::Collections::Generic;
using namespace System::Text::RegularExpressions;
using namespace System::Timers;

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents a wrapper for a libtorrent session.
	/// </summary>
	public interface class ITorrentSession : public IDisposable
	{
		/// <summary>
		/// Event that is raised when a torrent operation (add, finish, pause, resume) occurs.
		/// </summary>
		event EventHandler<TorrentOperationEventArgs^>^ TorrentOperationChanged;

		/// <summary>
		/// Event that is raised when an error occurs for a torrent.
		/// </summary>
		event EventHandler<TorrentErrorEventArgs^>^ TorrentError;

		/// <summary>
		/// Event that is raised when the state of one or more torrents is updated.
		/// </summary>
		event EventHandler<TorrentStateUpdateEventArgs^>^ TorrentStateUpdated;

		/// <summary>
		/// Event that is raised when the metadata for a torrent is received.
		/// </summary>
		event EventHandler<TorrentMetadataEventArgs^>^ TorrentMetadataReceived;

		/// <summary>
		/// Adds a torrent to the session using a magnet link.
		/// </summary>
		/// <param name="request">Contains what is needed to add a torrent.</param>
		/// <returns>True if the torrent was added, false if it already existed.</returns>
		virtual bool AddTorrent(AddTorrentFromMagnetLinkRequest^ request) = 0;

		/// <summary>
		/// Adds a torrent to the session using a torrent file.
		/// </summary>
		/// <param name="request">Contains what is needed to add a torrent.</param>
		/// <returns>True if the torrent was added, false if it already existed.</returns>
		virtual bool AddTorrent(AddTorrentFromTorrentFileRequest^ request) = 0;

		/// <summary>
		/// Adds a torrent to the session using a byte array containing a torrent's data.
		/// </summary>
		/// <param name="request">Contains what is needed to add a torrent.</param>
		/// <returns>True if the torrent was added, false if it already existed.</returns>
		virtual bool AddTorrent(AddTorrentFromByteArrayRequest^ request) = 0;

		/// <summary>
		/// Pauses a specific torrent.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to pause.</param>
		/// <returns>True if the torrent was paused, false if it wasn't found or couldn't be paused.</returns>
		virtual bool PauseTorrent(TorrentId^ torrentId) = 0;

		/// <summary>
		/// Pauses specified torrents.
		/// </summary>
		/// <param name="torrentIds">The identifiers of torrents to pause.</param>
		/// <returns>True if all torrents were paused successfully, false otherwise.</returns>
		virtual bool PauseTorrents(IReadOnlyList<TorrentId^>^ torrentIds) = 0;

		/// <summary>
		/// Resumes a specific torrent.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to resume.</param>
		/// <returns>True if the torrent was resumed, false if it wasn't found or couldn't be resumed.</returns>
		virtual bool ResumeTorrent(TorrentId^ torrentId) = 0;

		/// <summary>
		/// Resumes specified torrents.
		/// </summary>
		/// <param name="torrentIds">The identifiers of torrents to resume.</param>
		/// <returns>True if all torrents were resumed successfully, false otherwise.</returns>
		virtual bool ResumeTorrents(IReadOnlyList<TorrentId^>^ torrentIds) = 0;

		/// <summary>
		/// Removes a specific torrent from the session.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to remove.</param>
		/// <param name="deleteFiles">Specifies whether to delete all files downloaded by the torrent.</param>
		/// <returns>True if the torrent was removed, false if it wasn't found.</returns>
		virtual bool RemoveTorrent(TorrentId^ torrentId, bool deleteFiles) = 0;

		/// <summary>
		/// Removes specified torrents.
		/// </summary>
		/// <param name="torrentIds">The identifiers of torrents to remove.</param>
		/// <param name="deleteFiles">Specifies whether to delete all files downloaded by a torrent.</param>
		/// <returns>True if all torrents were removed successfully, false otherwise.</returns>
		virtual bool RemoveTorrents(IReadOnlyList<TorrentId^>^ torrentIds, bool deleteFiles) = 0;

		/// <summary>
		/// Sets the download rate limit for a specific torrent.
		/// </summary>
		/// <param name="torrentId">The unique identifier of the torrent.</param>
		/// <param name="downloadRateLimit">The download rate limit in bytes per second. Use -1 for unlimited.</param>
		/// <returns>True if the rate limit was set successfully, false if the torrent wasn't found.</returns>
		virtual bool SetTorrentDownloadRateLimit(TorrentId^ torrentId, int downloadRateLimit) = 0;

		/// <summary>
		/// Sets the upload rate limit for a specific torrent.
		/// </summary>
		/// <param name="torrentId">The unique identifier of the torrent.</param>
		/// <param name="uploadRateLimit">The upload rate limit in bytes per second. Use -1 for unlimited.</param>
		/// <returns>True if the rate limit was set successfully, false if the torrent wasn't found.</returns>
		virtual bool SetTorrentUploadRateLimit(TorrentId^ torrentId, int uploadRateLimit) = 0;

		/// <summary>
		/// Changes the interval at which events are raised. The default interval is once every second.
		/// Adjusting this interval can increase or decrease the frequency of event invocations, which may impact performance.
		/// </summary>
		/// <param name="interval">The new interval to use. The interval represents the duration between consecutive events.</param>
		/// <remarks>Use caution when setting very short intervals, as it may result in a high frequency of events.</remarks>
		virtual void ChangeEventTimerInterval(TimeSpan interval);

		/// <summary>
		/// Gets the status of a specific torrent in the session.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to get the status of.</param>
		/// <returns>The status of the specified torrent.</returns>
		virtual TorrentStatus^ GetTorrentStatus(TorrentId^ torrentId) = 0;

		/// <summary>
		/// Gets the status of all torrents in the session.
		/// </summary>
		/// <returns>A list of the status of all torrents in the session.</returns>
		virtual IReadOnlyList<TorrentStatus^>^ GetTorrentStatuses() = 0;

		/// <summary>
		/// Gets all torrents in the session.
		/// </summary>
		/// <returns>A list of all torrents in the session.</returns>
		virtual IReadOnlyList<TorrentInfo^>^ GetTorrents() = 0;

		/// <summary>
		/// Gets the torrent info of a specific torrent in the session.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to get the torrent info of.</param>
		/// <returns>The torrent info of the specified torrent.</returns>
		virtual TorrentInfo^ GetTorrentInfo(TorrentId^ torrentId) = 0;
	};

	/// <summary>
	/// Represents a wrapper for a libtorrent session.
	/// </summary>
	public ref class TorrentSession sealed : public ITorrentSession
	{
		libtorrent::session* nativeSession;
		Threading::ReaderWriterLockSlim^ lock;
		ElapsedEventHandler^ elapsedEventHandler;
		Timer^ alertTimer;
		bool isListeningToAlerts;
		ILogger^ logger;
		ref class NullLogger sealed : ILogger
		{
		public:
			virtual void Log(ILogger::LogLevel level, String^ message) {}
		};

		// Private constructor to prevent direct instantiation
		TorrentSession() {}
	public:

		/// <summary>
		/// Event that is raised when a torrent operation (add, finish, pause, resume) occurs.
		/// </summary>
		virtual event EventHandler<TorrentOperationEventArgs^>^ TorrentOperationChanged;

		/// <summary>
		/// Event that is raised when an error occurs for a torrent.
		/// </summary>
		virtual event EventHandler<TorrentErrorEventArgs^>^ TorrentError;

		/// <summary>
		/// Event that is raised when the state of one or more torrents is updated.
		/// </summary>
		virtual event EventHandler<TorrentStateUpdateEventArgs^>^ TorrentStateUpdated;

		/// <summary>
		/// Event that is raised when the metadata for a torrent is received.
		/// </summary>
		virtual event EventHandler<TorrentMetadataEventArgs^>^ TorrentMetadataReceived;

		/// <summary>
		/// Creates a new TorrentSession with default configuration.
		/// </summary>
		/// <returns>A new instance of ITorrentSession.</returns>
		static ITorrentSession^ Create()
		{
			auto torrentSession = gcnew TorrentSession();
			torrentSession->Initialize(Optional<TorrentSessionConfig^>::None(), Optional<ILogger^>::None());
			return torrentSession;
		}

		/// <summary>
		/// Creates a new TorrentSession with a custom logger.
		/// </summary>
		/// <param name="logger">The custom logger to use.</param>
		/// <returns>A new instance of ITorrentSession.</returns>
		static ITorrentSession^ Create(ILogger^ logger)
		{
			auto torrentSession = gcnew TorrentSession();
			torrentSession->Initialize(Optional<TorrentSessionConfig^>::None(), Optional<ILogger^>::Some(logger));
			return torrentSession;
		}

		/// <summary>
		/// Creates a new TorrentSession with custom configuration.
		/// </summary>
		/// <param name="config">The configuration to use for the session.</param>
		/// <returns>A new instance of ITorrentSession.</returns>
		static ITorrentSession^ Create(TorrentSessionConfig^ config)
		{
			auto torrentSession = gcnew TorrentSession();
			torrentSession->Initialize(Optional<TorrentSessionConfig^>::Some(config), Optional<ILogger^>::None());
			return torrentSession;
		}

		/// <summary>
		/// Creates a new TorrentSession with custom configuration and logger.
		/// </summary>
		/// <param name="config">The configuration to use for the session.</param>
		/// <param name="logger">The custom logger to use.</param>
		/// <returns>A new instance of ITorrentSession.</returns>
		static ITorrentSession^ Create(TorrentSessionConfig^ config, ILogger^ logger)
		{
			auto torrentSession = gcnew TorrentSession();
			torrentSession->Initialize(Optional<TorrentSessionConfig^>::Some(config), Optional<ILogger^>::Some(logger));
			return torrentSession;
		}

		~TorrentSession()
		{
			try
			{
				this->!TorrentSession();
			}
			catch (Exception^ ex)
			{
				Diagnostics::Trace::WriteLine(String::Format("Exception in TorrentSession destructor: {0}", ex->Message));
			}
		}

		!TorrentSession()
		{
			try
			{
				StopListeningToAlerts();

				if (alertTimer != nullptr)
				{
					delete alertTimer;
					alertTimer = nullptr;
				}

				if (nativeSession != nullptr)
				{
					delete nativeSession;
					nativeSession = nullptr;
				}

				if (lock != nullptr)
				{
					delete lock;
					lock = nullptr;
				}

				if (logger != nullptr)
				{
					logger->Log(ILogger::LogLevel::Info, "TorrentSession resources released");
				}
			}
			catch (...)
			{
				Diagnostics::Trace::WriteLine("Exception during TorrentSession cleanup.");
			}
		}

		/// <summary>
		/// Adds a torrent to the session using a magnet link.
		/// </summary>
		/// <param name="request">Contains what is needed to add a torrent.</param>
		/// <returns>True if the torrent was added, false if it already existed.</returns>
		virtual bool AddTorrent(AddTorrentFromMagnetLinkRequest^ request)
		{
			ArgumentNullException::ThrowIfNull(request, "request");
			logger->Log(ILogger::LogLevel::Info, "Adding torrent from magnet link");

			lock->EnterReadLock();

			libtorrent::add_torrent_params params;
			try
			{
				libtorrent::error_code ec;
				marshal_context context;
				const auto& magnetLink = context.marshal_as<std::string>(request->MagnetLink);
				const auto& savePath = context.marshal_as<std::string>(request->SavePath);

				params = libtorrent::parse_magnet_uri(magnetLink, ec);
				if (ec)
				{
					throw gcnew TorrentException(String::Format("Failed to parse magnet link: {0}", gcnew String(ec.message().c_str())));
				}
				params.save_path = savePath;

				if (const libtorrent::sha1_hash infoHash = params.info_hashes.get_best(); nativeSession->find_torrent(infoHash).is_valid())
				{
					logger->Log(ILogger::LogLevel::Info, "Torrent already exists, skipping addition");
					return false;
				}
			}
			finally
			{
				lock->ExitReadLock();
			}

			lock->EnterWriteLock();

			try
			{
				nativeSession->async_add_torrent(std::move(params));
				return true;
			}
			catch (const std::exception& e)
			{
				auto message = String::Format("Failed to add torrent from magnet link: {0}", gcnew String(e.what()));
				logger->Log(ILogger::LogLevel::Error, message);
				throw gcnew TorrentException(message);
			}
			finally
			{
				lock->ExitWriteLock();
			}
		}

		/// <summary>
		/// Adds a torrent to the session using a torrent file.
		/// </summary>
		/// <param name="request">Contains what is needed to add a torrent.</param>
		/// <returns>True if the torrent was added, false if it already existed.</returns>
		virtual bool AddTorrent(AddTorrentFromTorrentFileRequest^ request)
		{
			ArgumentNullException::ThrowIfNull(request, "request");
			logger->Log(ILogger::LogLevel::Info, "Adding torrent from torrent file");
			lock->EnterWriteLock();
			try
			{
				marshal_context context;
				const auto& torrentFilePath = context.marshal_as<std::string>(request->TorrentFilePath);
				const auto& savePath = context.marshal_as<std::string>(request->SavePath);
				libtorrent::torrent_info torrentInfo(torrentFilePath);

				if (nativeSession->find_torrent(torrentInfo.info_hashes().get_best()).is_valid())
				{
					logger->Log(ILogger::LogLevel::Info, "Torrent already exists, skipping addition");
					return false;
				}

				libtorrent::add_torrent_params addTorrentParams;
				addTorrentParams.ti = std::make_shared<libtorrent::torrent_info>(torrentInfo);
				addTorrentParams.save_path = savePath;
				nativeSession->async_add_torrent(addTorrentParams);
				return true;
			}
			catch (const std::exception& e)
			{
				auto message = String::Format("Failed to add torrent from file: {0}", gcnew String(e.what()));
				logger->Log(ILogger::LogLevel::Error, message);
				throw gcnew TorrentException(message);
			}
			finally
			{
				lock->ExitWriteLock();
			}
		}

		/// <summary>
		/// Adds a torrent to the session using a byte array containing a torrent's data.
		/// </summary>
		/// <param name="request">Contains what is needed to add a torrent.</param>
		/// <returns>True if the torrent was added, false if it already existed.</returns>
		virtual bool AddTorrent(AddTorrentFromByteArrayRequest^ request)
		{
			ArgumentNullException::ThrowIfNull(request, "request");
			logger->Log(ILogger::LogLevel::Info, "Adding torrent from byte array");
			lock->EnterWriteLock();
			const int length = request->TorrentData->Length;
			const auto nativeArray = std::make_unique<char[]>(length);
			try
			{
				marshal_context context;
				const auto& savePath = context.marshal_as<std::string>(request->SavePath);
				if (nativeArray != nullptr)
				{
					const pin_ptr<Byte> pinnedArray = &request->TorrentData[0];
					std::memcpy(nativeArray.get(), pinnedArray, length);
					libtorrent::torrent_info torrentInfo(nativeArray.get());

					// Check if the torrent already exists
					if (nativeSession->find_torrent(torrentInfo.info_hashes().get_best()).is_valid())
					{
						logger->Log(ILogger::LogLevel::Info, "Torrent already exists, skipping addition");
						return false;
					}

					libtorrent::add_torrent_params addTorrentParams;
					addTorrentParams.ti = std::make_shared<libtorrent::torrent_info>(torrentInfo);
					addTorrentParams.save_path = savePath;
					nativeSession->async_add_torrent(addTorrentParams);
					return true;
				}
				return false;
			}
			catch (const std::exception& e)
			{
				auto message = String::Format("Failed to add torrent from byte array: {0}", gcnew String(e.what()));
				logger->Log(ILogger::LogLevel::Error, message);
				throw gcnew TorrentException(message);
			}
			finally
			{
				lock->ExitWriteLock();
			}
		}

		/// <summary>
		/// Pauses a specific torrent.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to pause.</param>
		/// <returns>True if the torrent was paused, false if it wasn't found or couldn't be paused.</returns>
		virtual bool PauseTorrent(TorrentId^ torrentId)
		{
			ArgumentNullException::ThrowIfNull(torrentId, "torrentId");

			auto torrentIds = gcnew List<TorrentId^>(1);
			torrentIds->Add(torrentId);
			return PerformTorrentOperation(torrentIds, TorrentOperation::Pause, false);
		}

		/// <summary>
		/// Pauses specified torrents.
		/// </summary>
		/// <param name="torrentIds">The identifiers of torrents to pause.</param>
		/// <returns>True if all torrents were paused successfully, false otherwise.</returns>
		virtual bool PauseTorrents(IReadOnlyList<TorrentId^>^ torrentIds)
		{
			ArgumentNullException::ThrowIfNull(torrentIds, "torrentIds");

			return PerformTorrentOperation(torrentIds, TorrentOperation::Pause, false);
		}

		/// <summary>
		/// Resumes a specific torrent.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to resume.</param>
		/// <returns>True if the torrent was resumed, false if it wasn't found or couldn't be resumed.</returns>
		virtual bool ResumeTorrent(TorrentId^ torrentId)
		{
			ArgumentNullException::ThrowIfNull(torrentId, "torrentId");

			auto torrentIds = gcnew List<TorrentId^>(1);
			torrentIds->Add(torrentId);
			return PerformTorrentOperation(torrentIds, TorrentOperation::Resume, false);
		}

		/// <summary>
		/// Resumes specified torrents.
		/// </summary>
		/// <param name="torrentIds">The identifiers of torrents to resume.</param>
		/// <returns>True if all torrents were resumed successfully, false otherwise.</returns>
		virtual bool ResumeTorrents(IReadOnlyList<TorrentId^>^ torrentIds)
		{
			ArgumentNullException::ThrowIfNull(torrentIds, "torrentIds");

			return PerformTorrentOperation(torrentIds, TorrentOperation::Resume, false);
		}

		/// <summary>
		/// Removes a specific torrent from the session.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to remove.</param>
		/// <param name="deleteFiles">Specifies whether to delete all files downloaded by the torrent.</param>
		/// <returns>True if the torrent was removed, false if it wasn't found.</returns>
		virtual bool RemoveTorrent(TorrentId^ torrentId, bool deleteFiles)
		{
			ArgumentNullException::ThrowIfNull(torrentId, "torrentId");

			auto torrentIds = gcnew List<TorrentId^>(1);
			torrentIds->Add(torrentId);
			return PerformTorrentOperation(torrentIds, TorrentOperation::Remove, deleteFiles);
		}

		/// <summary>
		/// Removes specified torrents.
		/// </summary>
		/// <param name="torrentIds">The identifiers of torrents to remove.</param>
		/// <param name="deleteFiles">Specifies whether to delete all files downloaded by a torrent.</param>
		/// <returns>True if all torrents were removed successfully, false otherwise.</returns>
		virtual bool RemoveTorrents(IReadOnlyList<TorrentId^>^ torrentIds, bool deleteFiles)
		{
			ArgumentNullException::ThrowIfNull(torrentIds, "torrentIds");

			return PerformTorrentOperation(torrentIds, TorrentOperation::Remove, deleteFiles);
		}

		/// <summary>
		/// Sets the download rate limit for a specific torrent.
		/// </summary>
		/// <param name="torrentId">The unique identifier of the torrent.</param>
		/// <param name="downloadRateLimit">The download rate limit in bytes per second. Use -1 for unlimited.</param>
		/// <returns>True if the rate limit was set successfully, false if the torrent wasn't found.</returns>
		virtual bool SetTorrentDownloadRateLimit(TorrentId^ torrentId, int downloadRateLimit)
		{
			ArgumentNullException::ThrowIfNull(torrentId, "torrentId");

			lock->EnterReadLock();
			try
			{
				const auto& hash = ParseInfoHash(torrentId);

				if (const auto& handle = nativeSession->find_torrent(hash.get_best()); handle.is_valid())
				{
					handle.set_download_limit(downloadRateLimit);
					return true;
				}
				return false;
			}
			finally
			{
				lock->ExitReadLock();
			}
		}

		/// <summary>
		/// Sets the upload rate limit for a specific torrent.
		/// </summary>
		/// <param name="torrentId">The unique identifier of the torrent.</param>
		/// <param name="uploadRateLimit">The upload rate limit in bytes per second. Use -1 for unlimited.</param>
		/// <returns>True if the rate limit was set successfully, false if the torrent wasn't found.</returns>
		virtual bool SetTorrentUploadRateLimit(TorrentId^ torrentId, int uploadRateLimit)
		{
			ArgumentNullException::ThrowIfNull(torrentId, "torrentId");

			lock->EnterReadLock();
			try
			{
				const auto& hash = ParseInfoHash(torrentId);

				if (const auto& handle = nativeSession->find_torrent(hash.get_best()); handle.is_valid())
				{
					handle.set_upload_limit(uploadRateLimit);
					return true;
				}
				return false;
			}
			finally
			{
				lock->ExitReadLock();
			}
		}

		/// <summary>
		/// Changes the interval at which events are raised. The default interval is once every second.
		/// Adjusting this interval can increase or decrease the frequency of event invocations, which may impact performance.
		/// </summary>
		/// <param name="interval">The new interval to use, specified as a <see cref="TimeSpan"/>. The interval represents the duration between consecutive events.</param>
		/// <remarks>Use caution when setting very short intervals, as it may result in a high frequency of events.</remarks>
		virtual void ChangeEventTimerInterval(TimeSpan interval)
		{
			if (interval == TimeSpan::Zero)
			{
				throw gcnew ArgumentException("Time span must be greater than zero.", "interval");
			}

			alertTimer->Interval = interval.TotalMilliseconds;
		}

		/// <summary>
		/// Gets the status of a specific torrent in the session.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to get the status of.</param>
		/// <returns>The status of the specified torrent.</returns>
		virtual TorrentStatus^ GetTorrentStatus(TorrentId^ torrentId)
		{
			ArgumentNullException::ThrowIfNull(torrentId, "torrentId");

			lock->EnterReadLock();
			try
			{
				const auto& hash = ParseInfoHash(torrentId);

				if (const auto& handle = nativeSession->find_torrent(hash.get_best()); handle.is_valid())
				{
					return CreateTorrentStatus(handle.status(), torrentId);
				}

				throw gcnew InvalidOperationException("Torrent with the specified info hash was not found.");
			}
			finally
			{
				lock->ExitReadLock();
			}
		}


		/// <summary>
		/// Gets the status of all torrents in the session.
		/// </summary>
		/// <returns>A list of the status of all torrents in the session.</returns>
		virtual IReadOnlyList<TorrentStatus^>^ GetTorrentStatuses()
		{
			std::vector<libtorrent::torrent_handle> handles;

			lock->EnterReadLock();
			try
			{
				handles = nativeSession->get_torrents();
			}
			finally
			{
				lock->ExitReadLock();
			}

			auto statuses = gcnew List<TorrentStatus^>((long)handles.size());

			for (const auto& handle : handles)
			{
				if (handle.is_valid())
				{
					const auto status = handle.status();
					TorrentId^ torrentId = InfoHashToTorrentId(handle.info_hashes());
					TorrentStatus^ managedStatus = CreateTorrentStatus(status, torrentId);
					statuses->Add(managedStatus);
				}
			}

			return statuses;
		}

		/// <summary>
		/// Gets all torrents in the session.
		/// </summary>
		/// <returns>A list of all torrents in the session.</returns>
		virtual IReadOnlyList<TorrentInfo^>^ GetTorrents()
		{
			std::vector<libtorrent::torrent_handle> handles;

			lock->EnterReadLock();
			try
			{
				handles = nativeSession->get_torrents();
			}
			finally
			{
				lock->ExitReadLock();
			}

			auto torrents = gcnew List<TorrentInfo^>((long)handles.size());

			for (const auto& handle : handles)
			{
				if (handle.is_valid())
				{
					auto torrent = CreateTorrentInfo(handle);
					torrents->Add(torrent);
				}
			}

			return torrents;
		}

		/// <summary>
		/// Gets the torrent info of a specific torrent in the session.
		/// </summary>
		/// <param name="torrentId">The ID of the torrent to get the torrent info of.</param>
		/// <returns>The torrent info of the specified torrent.</returns>
		virtual TorrentInfo^ GetTorrentInfo(TorrentId^ torrentId)
		{
			ArgumentNullException::ThrowIfNull(torrentId, "torrentId");

			lock->EnterReadLock();
			try
			{
				const auto& hash = ParseInfoHash(torrentId);

				if (const auto& handle = nativeSession->find_torrent(hash.get_best()); handle.is_valid())
				{
					return CreateTorrentInfo(handle);
				}

				throw gcnew InvalidOperationException("Torrent with the specified info hash was not found.");
			}
			finally
			{
				lock->ExitReadLock();
			}
		}

	private:
		enum class TorrentOperation
		{
			Pause,
			Resume,
			Remove
		};

		void Initialize(Optional<TorrentSessionConfig^>^ config, Optional<ILogger^>^ logger)
		{
			if (logger->HasValue)
			{
				this->logger = logger->Value;
			}
			else
			{
				this->logger = gcnew NullLogger();
			}

			nativeSession = new libtorrent::session();
			lock = gcnew Threading::ReaderWriterLockSlim();
			isListeningToAlerts = false;

			if (config->HasValue)
			{
				ApplySettings(config->Value);
			}

			StartListeningToAlerts();

			this->logger->Log(ILogger::LogLevel::Info, "TorrentSession initialized");
		}

		void StartListeningToAlerts()
		{
			if (isListeningToAlerts)
			{
				logger->Log(ILogger::LogLevel::Info, "Already listening to alerts");
				return;
			}

			elapsedEventHandler = gcnew ElapsedEventHandler(this, &TorrentSession::OnAlertTimerElapsed);
			alertTimer = gcnew Timer(1000);
			alertTimer->AutoReset = true;
			alertTimer->Elapsed += elapsedEventHandler;
			isListeningToAlerts = false;

			logger->Log(ILogger::LogLevel::Info, "Starting to listen for alerts");
			isListeningToAlerts = true;
			alertTimer->Start();
		}

		void StopListeningToAlerts()
		{
			if (!isListeningToAlerts)
			{
				return;
			}

			logger->Log(ILogger::LogLevel::Info, "Stopping alert listener");
			isListeningToAlerts = false;
			alertTimer->Elapsed -= elapsedEventHandler;
			alertTimer->Stop();
		}

		bool PerformTorrentOperation(IReadOnlyList<TorrentId^>^ torrentIds, const TorrentOperation operation, const bool deleteFiles)
		{
			ArgumentNullException::ThrowIfNull(torrentIds, "torrentIds");

			std::unordered_set<libtorrent::info_hash_t> infoHashSet;
			for each (TorrentId ^ torrentId in torrentIds)
			{
				infoHashSet.insert(ParseInfoHash(torrentId));
			}

			std::vector<libtorrent::torrent_handle> handles;
			lock->EnterReadLock();
			try
			{
				for (const auto& handle : nativeSession->get_torrents())
				{
					if (handle.is_valid() && infoHashSet.contains(handle.info_hashes()))
					{
						handles.push_back(handle);
					}
				}
			}
			finally
			{
				lock->ExitReadLock();
			}

			bool allSuccessful = true;
			for (const auto& handle : handles)
			{
				bool operationSuccess = false;
				switch (operation)
				{
				case TorrentOperation::Pause:
					handle.pause();
					operationSuccess = true;
					break;
				case TorrentOperation::Resume:
					handle.resume();
					operationSuccess = true;
					break;
				case TorrentOperation::Remove:
					lock->EnterWriteLock();
					try
					{
						if (deleteFiles)
						{
							nativeSession->remove_torrent(handle, libtorrent::session_handle::delete_files);
							operationSuccess = true;
						}
						else
						{
							nativeSession->remove_torrent(handle);
							operationSuccess = true;
						}
					}
					finally
					{
						lock->ExitWriteLock();
					}
					break;
				}
				allSuccessful &= operationSuccess;
			}

			// If no handles were found, consider the operation unsuccessful
			if (handles.empty())
			{
				allSuccessful = false;
			}

			return allSuccessful;
		}

		void ApplySettings(TorrentSessionConfig^ config)
		{
			try
			{
				libtorrent::settings_pack settings;
				marshal_context context;

				if (config->ListenInterfaces->HasValue)
				{
					settings.set_str(libtorrent::settings_pack::listen_interfaces, context.marshal_as<std::string>(config->ListenInterfaces->Value));
				}
				if (config->OutgoingInterfaces->HasValue)
				{
					settings.set_str(libtorrent::settings_pack::outgoing_interfaces, context.marshal_as<std::string>(config->OutgoingInterfaces->Value));
				}
				if (config->EnableUpnp->HasValue)
				{
					settings.set_bool(libtorrent::settings_pack::enable_upnp, config->EnableUpnp->Value);
				}
				if (config->EnableNatPmp->HasValue)
				{
					settings.set_bool(libtorrent::settings_pack::enable_natpmp, config->EnableNatPmp->Value);
				}
				if (config->DhtSettings->EnableDht->HasValue)
				{
					settings.set_bool(libtorrent::settings_pack::enable_dht, config->DhtSettings->EnableDht->Value);
				}
				if (config->DhtSettings->MaxPeers->HasValue)
				{
					settings.set_int(libtorrent::settings_pack::dht_max_peers, config->DhtSettings->MaxPeers->Value);
				}
				if (config->EnableLsd->HasValue)
				{
					settings.set_bool(libtorrent::settings_pack::enable_lsd, config->EnableLsd->Value);
				}
				if (config->BandwidthSettings->MaxConnections)
				{
					settings.set_int(libtorrent::settings_pack::connections_limit, config->BandwidthSettings->MaxConnections->Value);
				}
				if (config->BandwidthSettings->DownloadRateLimit->HasValue)
				{
					settings.set_int(libtorrent::settings_pack::download_rate_limit, config->BandwidthSettings->DownloadRateLimit->Value);
				}
				if (config->BandwidthSettings->UploadRateLimit->HasValue)
				{
					settings.set_int(libtorrent::settings_pack::upload_rate_limit, config->BandwidthSettings->UploadRateLimit->Value);
				}
				if (config->ProxySettings->ProxyType != None)
				{
					settings.set_int(libtorrent::settings_pack::proxy_type, config->ProxySettings->ProxyType);

					if (config->ProxySettings->Host->HasValue)
					{
						settings.set_str(libtorrent::settings_pack::proxy_hostname, context.marshal_as<std::string>(config->ProxySettings->Host->Value));
					}
					if (config->ProxySettings->Username->HasValue)
					{
						settings.set_str(libtorrent::settings_pack::proxy_username, context.marshal_as<std::string>(config->ProxySettings->Username->Value));
					}
					if (config->ProxySettings->Password->HasValue)
					{
						settings.set_str(libtorrent::settings_pack::proxy_password, context.marshal_as<std::string>(config->ProxySettings->Password->Value));
					}
					if (config->ProxySettings->Port->HasValue)
					{
						settings.set_int(libtorrent::settings_pack::proxy_port, config->ProxySettings->Port->Value);
					}
				}

				nativeSession->apply_settings(settings);
			}
			catch (const std::exception& e)
			{
				auto message = String::Format("Error when applying settings: {0}", gcnew String(e.what()));
				logger->Log(ILogger::LogLevel::Error, message);
				throw gcnew TorrentException(message);
			}
		}

		static libtorrent::info_hash_t ParseInfoHash(TorrentId^ torrentId)
		{
			marshal_context context;

			if (const auto& hashStr = context.marshal_as<std::string>(torrentId->ToString()); hashStr.length() == 40 || hashStr.length() == 64)
			{
				const size_t hashSize = hashStr.length() / 2;
				std::vector<char> hashBytes(hashSize);
				for (int i = 0; i < hashSize; i++)
				{
					hashBytes[i] = static_cast<char>(std::stoi(hashStr.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(i) * 2, 2), nullptr, 16));
				}

				if (hashStr.length() == 40)
				{
					return libtorrent::info_hash_t(libtorrent::sha1_hash(hashBytes.data()));
				}

				return libtorrent::info_hash_t(libtorrent::sha256_hash(hashBytes.data()));
			}

			throw gcnew ArgumentException("Invalid info hash format");
		}

		static TorrentId^ InfoHashToTorrentId(const libtorrent::info_hash_t& infoHash)
		{
			const auto& hash = infoHash.get_best();
			const int hashSize = infoHash.has_v2() ? 32 : 20;
			auto hexString = gcnew Text::StringBuilder(hashSize * 2);
			hexString->EnsureCapacity(hashSize * 2);

			for (int i = 0; i < hashSize; i++)
			{
				hexString->AppendFormat("{0:X2}", hash[i]);
			}

			return gcnew TorrentId(hexString->ToString());
		}

		static TorrentStatus^ CreateTorrentStatus(const libtorrent::torrent_status& status, TorrentId^ torrentId)
		{
			TorrentState torrentState;
			switch (status.state)
			{
			case libtorrent::torrent_status::checking_files:
				torrentState = TorrentState::CheckingFiles;
				break;
			case libtorrent::torrent_status::downloading_metadata:
				torrentState = TorrentState::DownloadingMetadata;
				break;
			case libtorrent::torrent_status::downloading:
				torrentState = TorrentState::Downloading;
				break;
			case libtorrent::torrent_status::finished:
				torrentState = TorrentState::Finished;
				break;
			case libtorrent::torrent_status::seeding:
				torrentState = TorrentState::Seeding;
				break;
			case libtorrent::torrent_status::checking_resume_data:
				torrentState = TorrentState::CheckingResumeData;
				break;
			default:
				torrentState = TorrentState::Unknown;
				break;
			}

			auto managedStatus = gcnew TorrentStatus(
				torrentId,
				torrentState,
				status.progress,
				status.total_download,
				status.total_upload,
				status.download_rate,
				status.upload_rate,
				status.num_peers,
				status.num_seeds);

			return managedStatus;
		}

		static TorrentInfo^ CreateTorrentInfo(const libtorrent::torrent_handle& handle)
		{
			TorrentId^ torrentId = InfoHashToTorrentId(handle.info_hashes());
			const auto& status = handle.status();
			auto name = gcnew String(status.name.c_str());
			auto savePath = gcnew String(status.save_path.c_str());
			TorrentStatus^ managedStatus = CreateTorrentStatus(status, torrentId);
			UInt64 totalSize = 0;

			List<TorrentFile^>^ files;
			if (handle.torrent_file())
			{
				files = gcnew List<TorrentFile^>(handle.torrent_file()->num_files());

				for (const auto& index : handle.torrent_file()->files().file_range())
				{
					const auto& torrentFile = handle.torrent_file();
					const auto& path = gcnew String(torrentFile->files().file_path(index, status.save_path).c_str());
					const auto& filename = gcnew String(torrentFile->files().file_name(index).to_string().c_str());
					const auto& size = torrentFile->files().file_size(index);

					files->Add(gcnew TorrentFile(path, filename, size));
				}

				totalSize = handle.torrent_file()->total_size();
			}
			else
			{
				files = gcnew List<TorrentFile^>(0);
			}

			return gcnew TorrentInfo(torrentId, name, managedStatus, files, totalSize, savePath);
		}

		void OnAlertTimerElapsed(Object^ sender, ElapsedEventArgs^ e)
		{
			if (!isListeningToAlerts)
			{
				return;
			}

			nativeSession->post_torrent_updates();

			std::vector<libtorrent::alert*> alerts;

			lock->EnterReadLock();
			try
			{
				nativeSession->pop_alerts(&alerts);
			}
			finally
			{
				lock->ExitReadLock();
			}

			for (libtorrent::alert* alert : alerts)
			{
				ProcessAlert(alert);
			}
		}

		void ProcessAlert(libtorrent::alert* alert)
		{
			try
			{
				if (const auto* addAlert = libtorrent::alert_cast<libtorrent::add_torrent_alert>(alert))
				{
					TorrentId^ torrentId = InfoHashToTorrentId(addAlert->handle.info_hashes());
					TorrentOperationChanged(this, gcnew TorrentOperationEventArgs(torrentId, TorrentOperationEvent::Added));
				}
				else if (const auto* finishAlert = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(alert))
				{
					TorrentId^ torrentId = InfoHashToTorrentId(finishAlert->handle.info_hashes());
					TorrentOperationChanged(this, gcnew TorrentOperationEventArgs(torrentId, TorrentOperationEvent::Finished));
				}
				else if (const auto* removeAlert = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(alert))
				{
					TorrentId^ torrentId = InfoHashToTorrentId(removeAlert->handle.info_hashes());
					TorrentOperationChanged(this, gcnew TorrentOperationEventArgs(torrentId, TorrentOperationEvent::Removed));
				}
				else if (const auto* errorAlert = libtorrent::alert_cast<libtorrent::torrent_error_alert>(alert))
				{
					TorrentId^ torrentId = InfoHashToTorrentId(errorAlert->handle.info_hashes());
					TorrentError(this, gcnew TorrentErrorEventArgs(torrentId, gcnew String(errorAlert->error.message().c_str())));
				}
				else if (const auto* pauseAlert = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(alert))
				{
					TorrentId^ torrentId = InfoHashToTorrentId(pauseAlert->handle.info_hashes());
					TorrentOperationChanged(this, gcnew TorrentOperationEventArgs(torrentId, TorrentOperationEvent::Paused));
				}
				else if (const auto* resumeAlert = libtorrent::alert_cast<libtorrent::torrent_resumed_alert>(alert))
				{
					TorrentId^ torrentId = InfoHashToTorrentId(resumeAlert->handle.info_hashes());
					TorrentOperationChanged(this, gcnew TorrentOperationEventArgs(torrentId, TorrentOperationEvent::Resumed));
				}
				else if (const auto* stateAlert = libtorrent::alert_cast<libtorrent::state_update_alert>(alert))
				{
					auto torrentStats = gcnew List<TorrentStatus^>((long)stateAlert->status.size());

					for (const auto& status : stateAlert->status)
					{
						TorrentId^ torrentId = InfoHashToTorrentId(status.info_hashes);
						torrentStats->Add(CreateTorrentStatus(status, torrentId));
					}

					TorrentStateUpdated(this, gcnew TorrentStateUpdateEventArgs(torrentStats));
				}
				else if (const auto* metadataAlert = libtorrent::alert_cast<libtorrent::metadata_received_alert>(alert))
				{
					if (metadataAlert->handle.is_valid())
					{
						const auto torrentInfo = CreateTorrentInfo(metadataAlert->handle);
						TorrentMetadataReceived(this, gcnew TorrentMetadataEventArgs(torrentInfo));
					}
				}
			}
			catch (const std::exception& e)
			{
				auto message = String::Format("Error when processing alert: {0}", gcnew String(e.what()));
				logger->Log(ILogger::LogLevel::Error, message);
				throw gcnew TorrentException(message);
			}
		}
	};
}