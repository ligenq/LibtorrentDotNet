#pragma once

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents various events that can occur during torrent operations.
	/// </summary>
	public enum class TorrentOperationEvent
	{
		Added,
		Finished,
		Paused,
		Resumed,
		Removed
	};
}