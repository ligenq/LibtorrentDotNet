#pragma once

#include "TorrentOperationEvent.h"
#include "TorrentId.h"
#include "TorrentInfo.h"
#include "TorrentStatus.h"

using namespace System;
using namespace System::Collections::Generic;

namespace LibtorrentDotNet
{
    /// <summary>
    /// Represents the arguments for a torrent operation event.
    /// </summary>
    public ref class TorrentOperationEventArgs sealed : EventArgs
    {
    public:
        TorrentOperationEventArgs(TorrentId^ torrentId, const TorrentOperationEvent operationEvent)
            : torrentId(torrentId), operationEvent(operationEvent) {}

        /// <summary>
        /// Gets the ID of the torrent associated with this event.
        /// </summary>
        property TorrentId^ Id { TorrentId^ get() { return torrentId; } }

        /// <summary>
        /// Gets the type of operation that occurred.
        /// </summary>
        property TorrentOperationEvent OperationEvent { TorrentOperationEvent get() { return operationEvent; } }

    private:
        TorrentId^ torrentId;
        TorrentOperationEvent operationEvent;
    };

    /// <summary>
    /// Represents the arguments for a torrent error event.
    /// </summary>
    public ref class TorrentErrorEventArgs sealed : EventArgs
    {
    public:
        TorrentErrorEventArgs(TorrentId^ torrentId, String^ error)
            : torrentId(torrentId), error(error) {}

        /// <summary>
        /// Gets the ID of the torrent associated with this error.
        /// </summary>
        property TorrentId^ Id { TorrentId^ get() { return torrentId; } }

        /// <summary>
        /// Gets the error message.
        /// </summary>
        property String^ Error { String^ get() { return error; } }

    private:
        TorrentId^ torrentId;
        String^ error;
    };

    /// <summary>
    /// Represents the arguments for a torrent state update event.
    /// </summary>
    public ref class TorrentStateUpdateEventArgs sealed : EventArgs
    {
    public:
        TorrentStateUpdateEventArgs(IReadOnlyList<TorrentStatus^>^ updatedTorrents)
            : updatedTorrents(updatedTorrents) {}

        /// <summary>
        /// Gets a list of updated torrent statuses.
        /// </summary>
        property IReadOnlyList<TorrentStatus^>^ UpdatedTorrents { IReadOnlyList<TorrentStatus^>^ get() { return updatedTorrents; } }

    private:
        IReadOnlyList<TorrentStatus^>^ updatedTorrents;
    };

    /// <summary>
    /// Represents the arguments for a torrent metadata received event.
    /// </summary>
    public ref class TorrentMetadataEventArgs sealed : EventArgs
    {
    public:
        TorrentMetadataEventArgs(TorrentInfo^ torrentInfo)
            : torrentInfo(torrentInfo) {}

        /// <summary>
        /// Gets the detailed information about the torrent.
        /// </summary>
        property TorrentInfo^ Torrent { TorrentInfo^ get() { return torrentInfo; } }

    private:
        TorrentInfo^ torrentInfo;
    };
}
