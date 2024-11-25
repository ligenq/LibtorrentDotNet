#pragma once

#include "TorrentFileEntry.h"
#include "TorrentId.h"
#include "TorrentStatus.h"

using namespace System;
using namespace System::Collections::Generic;

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents detailed information about a torrent.
	/// </summary>
	public ref class TorrentInfo sealed
	{
	public:
		/// <summary>
		/// Initializes a new instance of the TorrentInfo class.
		/// </summary>
		/// <param name="torrentId">The unique identifier of the torrent.</param>
		/// <param name="name">The name of the torrent.</param>
		/// <param name="status">The current status of the torrent.</param>
		/// <param name="torrentFileEntries">A list of files in the torrent.</param>
		/// <param name="totalSize">The total size of all files in the torrent, in bytes.</param>
		/// <param name="savePath">The path to where the downloaded torrent file(s) are saved.</param>
		TorrentInfo(
			TorrentId^ torrentId,
			String^ name,
			TorrentStatus^ status,
			IReadOnlyList<TorrentFileEntry^>^ torrentFileEntries,
			const Int64 totalSize,
			String^ savePath) : torrentId(torrentId), name(name), savePath(savePath), status(status), torrentFileEntries(torrentFileEntries), totalSize(totalSize) {}

		/// <summary>
		/// Gets the unique identifier of the torrent.
		/// </summary>
		property TorrentId^ Id { TorrentId^ get() { return torrentId; } }

		/// <summary>
		/// Gets the name of the torrent.
		/// </summary>
		property String^ Name { String^ get() { return name; } }

		/// <summary>
		/// Gets the total size of all files in the torrent, in bytes.
		/// </summary>
		property Int64 TotalSize { Int64 get() { return totalSize; } }

		/// <summary>
		/// Gets the current status of the torrent.
		/// </summary>
		property TorrentStatus^ Status { TorrentStatus^ get() { return status; } }

		/// <summary>
		/// Gets the files downloaded by the torrent.
		/// </summary>
		property IReadOnlyList<TorrentFileEntry^>^ TorrentFileEntries { IReadOnlyList<TorrentFileEntry^>^ get() { return torrentFileEntries; } }

		/// <summary>
		/// Gets the path to where the downloaded torrent file(s) are saved.
		/// </summary>
		property String^ SavePath { String^ get() { return savePath; } }

	private:
		TorrentId^ torrentId;
		String^ name;
		String^ savePath;
		TorrentStatus^ status;
		IReadOnlyList<TorrentFileEntry^>^ torrentFileEntries;
		Int64 totalSize;
	};
}
