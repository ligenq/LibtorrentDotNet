#pragma once

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents the current state of a torrent.
	/// </summary>
	public enum class TorrentState
	{
		Unknown,
		CheckingFiles,
		DownloadingMetadata,
		Downloading,
		Finished,
		Seeding,
		CheckingResumeData, 
	};
}