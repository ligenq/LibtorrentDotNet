#pragma once

using namespace System;

namespace LibtorrentDotNet
{
    /// <summary>
    /// Represents a request to add a torrent from a magnet link.
    /// </summary>
    public ref class AddTorrentFromMagnetLinkRequest sealed
    {
    public:
        /// <summary>
        /// Gets the magnet link for the torrent.
        /// </summary>
        property String^ MagnetLink
        {
            String^ get() { return magnetLink; }
        }

        /// <summary>
        /// Gets the save path for the torrent files.
        /// </summary>
        property String^ SavePath
        {
            String^ get() { return savePath; }
        }

        /// <summary>
        /// Initializes a new instance of the AddTorrentFromMagnetLinkRequest class.
        /// </summary>
        /// <param name="magnetLink">The magnet link for the torrent.</param>
        /// <param name="savePath">The path where torrent files will be saved.</param>
        /// <exception cref="ArgumentException">Thrown when magnetLink or savePath is null, empty, or invalid.</exception>
        AddTorrentFromMagnetLinkRequest(String^ magnetLink, String^ savePath)
        {
            if (String::IsNullOrWhiteSpace(magnetLink))
                throw gcnew ArgumentException("Magnet link cannot be null or empty.", "magnetLink");

            if (String::IsNullOrWhiteSpace(savePath))
                throw gcnew ArgumentException("Save path cannot be null or empty.", "savePath");

            if (!IO::Directory::Exists(IO::Path::GetDirectoryName(savePath)))
                throw gcnew ArgumentException("Save path directory does not exist.", "savePath");

            if (!magnetLink->StartsWith("magnet:"))
                throw gcnew ArgumentException("Invalid magnet link format.", "magnetLink");

            this->magnetLink = magnetLink;
            this->savePath = savePath;
        }

    private:
        String^ magnetLink;
        String^ savePath;
    };

    /// <summary>
    /// Represents a request to add a torrent from a torrent file.
    /// </summary>
    public ref class AddTorrentFromTorrentFileRequest sealed
    {
    public:
        /// <summary>
        /// Gets the file path of the torrent file.
        /// </summary>
        property String^ TorrentFilePath
        {
            String^ get() { return torrentFilePath; }
        }

        /// <summary>
        /// Gets the save path for the torrent files.
        /// </summary>
        property String^ SavePath
        {
            String^ get() { return savePath; }
        }

        /// <summary>
        /// Initializes a new instance of the AddTorrentFromTorrentFileRequest class.
        /// </summary>
        /// <param name="torrentFilePath">The file path of the torrent file.</param>
        /// <param name="savePath">The path where torrent files will be saved.</param>
        /// <exception cref="ArgumentException">Thrown when torrentFilePath or savePath is null, empty, or invalid.</exception>
        /// <exception cref="IO::FileNotFoundException">Thrown when the torrent file does not exist.</exception>
        AddTorrentFromTorrentFileRequest(String^ torrentFilePath, String^ savePath)
        {
            if (String::IsNullOrWhiteSpace(torrentFilePath))
                throw gcnew ArgumentException("Torrent file path cannot be null or empty.", "torrentFilePath");

            if (String::IsNullOrWhiteSpace(savePath))
                throw gcnew ArgumentException("Save path cannot be null or empty.", "savePath");

            if (!IO::File::Exists(torrentFilePath))
                throw gcnew IO::FileNotFoundException("Torrent file does not exist.", torrentFilePath);

            if (!IO::Directory::Exists(IO::Path::GetDirectoryName(savePath)))
                throw gcnew ArgumentException("Save path directory does not exist.", "savePath");

            if (!torrentFilePath->EndsWith(".torrent"))
                throw gcnew ArgumentException("File is not a torrent file.", "torrentFilePath");

            this->torrentFilePath = torrentFilePath;
            this->savePath = savePath;
        }

    private:
        String^ torrentFilePath;
        String^ savePath;
    };

    /// <summary>
    /// Represents a request to add a torrent from a byte array containing torrent data.
    /// </summary>
    public ref class AddTorrentFromByteArrayRequest sealed
    {
    public:
        /// <summary>
        /// Gets the byte array containing the torrent data.
        /// </summary>
        property array<Byte>^ TorrentData
        {
            array<Byte>^ get() { return torrentData; }
        }

        /// <summary>
        /// Gets the save path for the torrent files.
        /// </summary>
        property String^ SavePath
        {
            String^ get() { return savePath; }
        }

        /// <summary>
        /// Initializes a new instance of the AddTorrentFromByteArrayRequest class.
        /// </summary>
        /// <param name="torrentData">The byte array containing the torrent data.</param>
        /// <param name="savePath">The path where torrent files will be saved.</param>
        /// <exception cref="ArgumentException">Thrown when torrentData is null or empty, or when savePath is null, empty, or invalid.</exception>
        AddTorrentFromByteArrayRequest(array<Byte>^ torrentData, String^ savePath)
        {
            if (torrentData == nullptr || torrentData->Length == 0)
                throw gcnew ArgumentException("Torrent data cannot be null or empty.", "torrentData");

            if (String::IsNullOrWhiteSpace(savePath))
                throw gcnew ArgumentException("Save path cannot be null or empty.", "savePath");

            if (!IO::Directory::Exists(IO::Path::GetDirectoryName(savePath)))
                throw gcnew ArgumentException("Save path directory does not exist.", "savePath");

            this->torrentData = torrentData;
            this->savePath = savePath;
        }

    private:
        array<Byte>^ torrentData;
        String^ savePath;
    };
}