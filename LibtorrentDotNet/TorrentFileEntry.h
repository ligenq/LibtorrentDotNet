#pragma once

using namespace System;

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents a file within a torrent.
	/// </summary>
	public ref class TorrentFileEntry sealed
	{
	public:
		/// <summary>
		/// Initializes a new instance of the TorrentFileEntry class.
		/// </summary>
		/// <param name="id">The index of the file.</param>
		/// <param name="path">The full path of the file.</param>
		/// <param name="name">The name of the file.</param>
		/// <param name="size">The size of the file in bytes.</param>
		TorrentFileEntry(const int fileIndex, String^ path, String^ name, const UInt64 size)
			: fileIndex(fileIndex), path(path), name(name), size(size) {}

		/// <summary>
		/// Gets the index of the file within the torrent.
		/// </summary>
		property int FileIndex { int get() { return fileIndex; } }
		
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
		int fileIndex;
		String^ path;
		String^ name;
		UInt64 size;
	};
}