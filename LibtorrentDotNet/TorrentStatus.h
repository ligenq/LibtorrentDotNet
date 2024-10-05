#pragma once
#include "TorrentId.h"
#include "TorrentState.h"

using namespace System;

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents the status of a torrent.
	/// </summary>
	public ref class TorrentStatus sealed {
	public:
		/// <summary>
		/// Initializes a new instance of the TorrentStatus class.
		/// </summary>
		/// <param name="torrentId">The unique identifier of the torrent.</param>
		/// <param name="state">The current state of the torrent.</param>
		/// <param name="progress">The download progress of the torrent (0.0 to 1.0).</param>
		/// <param name="totalDownload">The total number of bytes downloaded.</param>
		/// <param name="totalUpload">The total number of bytes uploaded.</param>
		/// <param name="downloadRate">The current download rate in bytes per second.</param>
		/// <param name="uploadRate">The current upload rate in bytes per second.</param>
		/// <param name="numPeers">The number of peers connected to.</param>
		/// <param name="numSeeds">The number of seeds connected to.</param>
		TorrentStatus(
			TorrentId^ torrentId,
			const TorrentState state,
			const double progress,
			const Int64 totalDownload,
			const Int64 totalUpload,
			const int downloadRate,
			const int uploadRate,
			const int numPeers,
			const int numSeeds) :
			torrentId(torrentId),
			state(state),
			progress(progress),
			totalDownload(totalDownload),
			totalUpload(totalUpload),
			downloadRate(downloadRate),
			uploadRate(uploadRate),
			numPeers(numPeers),
			numSeeds(numSeeds) {}

		/// <summary>
		/// Gets the unique identifier of the torrent.
		/// </summary>
		property TorrentId^ Id { TorrentId^ get() { return torrentId; } }

		/// <summary>
		/// Gets the current state of the torrent.
		/// </summary>
		property TorrentState State { TorrentState get() { return state; } };

		/// <summary>
		/// Gets the download progress of the torrent (0.0 to 1.0).
		/// </summary>
		property double Progress { double get() { return progress; } };

		/// <summary>
		/// Gets the total number of bytes downloaded.
		/// </summary>
		property Int64 TotalDownload { Int64 get() { return totalDownload; } };

		/// <summary>
		/// Gets the total number of bytes uploaded.
		/// </summary>
		property Int64 TotalUpload { Int64 get() { return totalUpload; } };

		/// <summary>
		/// Gets the current download rate in bytes per second.
		/// </summary>
		property int DownloadRate { int get() { return downloadRate; } };

		/// <summary>
		/// Gets the current upload rate in bytes per second.
		/// </summary>
		property int UploadRate { int get() { return uploadRate; } };

		/// <summary>
		/// Gets the number of peers connected to.
		/// </summary>
		property int NumPeers { int get() { return numPeers; } };

		/// <summary>
		/// Gets the number of seeds connected to.
		/// </summary>
		property int NumSeeds { int get() { return numSeeds; } };

	private:
		TorrentId^ torrentId;
		TorrentState state;
		double progress;
		Int64 totalDownload;
		Int64 totalUpload;
		int downloadRate;
		int uploadRate;
		int numPeers;
		int numSeeds;
	};
}
