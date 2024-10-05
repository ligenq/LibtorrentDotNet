#pragma once

#include "TorrentFile.h"
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
		/// <param name="torrentFiles">A list of files in the torrent.</param>
		/// <param name="totalSize">The total size of all files in the torrent, in bytes.</param>
		TorrentInfo(
			TorrentId^ torrentId,
			String^ name,
			TorrentStatus^ status,
			IReadOnlyList<TorrentFile^>^ torrentFiles,
			const Int64 totalSize) : torrentId(torrentId), name(name), status(status), torrentFiles(torrentFiles), totalSize(totalSize) {}

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
		property IReadOnlyList<TorrentFile^>^ TorrentFiles { IReadOnlyList<TorrentFile^>^ get() { return torrentFiles; } }

	private:
		TorrentId^ torrentId;
		String^ name;
		TorrentStatus^ status;
		IReadOnlyList<TorrentFile^>^ torrentFiles;
		System::Int64 totalSize;
	};
}
