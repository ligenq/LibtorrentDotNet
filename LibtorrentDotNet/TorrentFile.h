#pragma once

using namespace System;

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents a file within a torrent.
	/// </summary>
	public ref class TorrentFile sealed
	{
	public:
		/// <summary>
		/// Initializes a new instance of the TorrentFile class.
		/// </summary>
		/// <param name="path">The full path of the file.</param>
		/// <param name="name">The name of the file.</param>
		/// <param name="size">The size of the file in bytes.</param>
		TorrentFile(String^ path, String^ name, const UInt64 size)
			: path(path), name(name), size(size) {}

		/// <summary>
		/// Gets the full path of the file.
		/// </summary>
		property String^ Path { String^ get() { return path; } }

		/// <summary>
		/// Gets the name of the file.
		/// </summary>
		property String^ Name { String^ get() { return name; } }

		/// <summary>
		/// Gets the size of the file in bytes.
		/// </summary>
		property UInt64 Size { UInt64 get() { return size; } }

	private:
		String^ path;
		String^ name;
		UInt64 size;
	};
}