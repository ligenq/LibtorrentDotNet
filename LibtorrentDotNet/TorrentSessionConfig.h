#pragma once
#include "Optional.h"
using namespace System;

namespace LibtorrentDotNet
{
    /// <summary>
	/// Represents the type of proxy server to connect to.
	/// </summary>
    public enum ServerProxyType
    {
        /// <summary>
        /// No proxy server is used. All other proxy settings will be ignored.
        /// </summary>
        None,

        /// <summary>
		/// The server is assumed to be a SOCKS4 server that requires a username.
		/// </summary>
        Socks4,

        /// <summary>
		/// The server is assumed to be a SOCKS5 server that does not require any authentication. Username and password will be ignored.
		/// </summary>
        Socks5,

        /// <summary>
		/// The server is assumed to be a SOCKS5 server that that supports plain text username and password authentication.
		/// </summary>
        Socks5Password,

        /// <summary>
		/// The server is assumed to be an HTTP proxy that does not require any authentication. Username and password will be ignored.
		/// </summary>
        Http,

        /// <summary>
		/// The server is assumed to be an HTTP proxy that requires authentication. Username and password will be sent to the proxy.
		/// </summary>
        HttpPassword
    };

    /// <summary>
    /// Represents the configuration for a proxy server.
    /// </summary>
    public ref class ProxyConfig sealed
    {
    public:
        /// <summary>
        /// Gets or sets the host of the proxy server.
        /// </summary>
        property Optional<String^>^ Host;

        /// <summary>
        /// Gets or sets the port of the proxy server.
        /// </summary>
        property Optional<int>^ Port;

        /// <summary>
        /// Gets or sets the username for proxy authentication.
        /// </summary>
        property Optional<String^>^ Username;

        /// <summary>
        /// Gets or sets the password for proxy authentication.
        /// </summary>
        property Optional<String^>^ Password;

        /// <summary>
		/// Gets or sets the proxy type of the server to connect to.
		/// </summary>
        property ServerProxyType ProxyType;

        /// <summary>
        /// Initializes a new instance of the ProxyConfig class with default values.
        /// </summary>
        ProxyConfig()
        {
            Host = Optional<String^>::None();
            Port = Optional<int>::None();
            Username = Optional<String^>::None();
            Password = Optional<String^>::None();
            ProxyType = None;
        }
    };

    /// <summary>
    /// Represents the configuration for bandwidth settings.
    /// </summary>
    public ref class BandwidthConfig sealed
    {
    public:
        /// <summary>
        /// Gets or sets the download rate limit in bytes per second.
        /// </summary>
        property Optional<int>^ DownloadRateLimit;

        /// <summary>
        /// Gets or sets the upload rate limit in bytes per second.
        /// </summary>
        property Optional<int>^ UploadRateLimit;

        /// <summary>
        /// Gets or sets the maximum number of connections.
        /// </summary>
        property Optional<int>^ MaxConnections;

        /// <summary>
        /// Initializes a new instance of the BandwidthConfig class with default values.
        /// </summary>
        BandwidthConfig()
        {
            DownloadRateLimit = Optional<int>::None();
            UploadRateLimit = Optional<int>::None();
            MaxConnections = Optional<int>::None();
        }
    };

    /// <summary>
    /// Represents the configuration for DHT (Distributed Hash Table) settings.
    /// </summary>
    public ref class DhtConfig sealed
    {
    public:
        /// <summary>
        /// Gets or sets whether DHT is enabled.
        /// </summary>
        property Optional<bool>^ EnableDht;

        /// <summary>
        /// Gets or sets the maximum number of peers in the DHT.
        /// </summary>
        property Optional<int>^ MaxPeers;

        /// <summary>
        /// Initializes a new instance of the DhtConfig class with default values.
        /// </summary>
        DhtConfig()
        {
            EnableDht = Optional<bool>::None();
            MaxPeers = Optional<int>::None();
        }
    };

    /// <summary>
    /// Represents the configuration for a torrent session.
    /// </summary>
    public ref class TorrentSessionConfig sealed
    {
    public:
        /// <summary>
        /// Gets or sets the listen interfaces for the torrent session.
        /// </summary>
        property Optional<String^>^ ListenInterfaces;

        /// <summary>
        /// Gets or sets the outgoing interfaces for the torrent session.
        /// </summary>
        property Optional<String^>^ OutgoingInterfaces;

        /// <summary>
        /// Gets or sets the proxy settings for the torrent session.
        /// </summary>
        property ProxyConfig^ ProxySettings;

        /// <summary>
        /// Gets or sets the bandwidth settings for the torrent session.
        /// </summary>
        property BandwidthConfig^ BandwidthSettings;

        /// <summary>
        /// Gets or sets the DHT settings for the torrent session.
        /// </summary>
        property DhtConfig^ DhtSettings;

        /// <summary>
        /// Gets or sets whether UPnP is enabled.
        /// </summary>
        property Optional<bool>^ EnableUpnp;

        /// <summary>
        /// Gets or sets whether NAT-PMP is enabled.
        /// </summary>
        property Optional<bool>^ EnableNatPmp;

        /// <summary>
        /// Gets or sets whether LSD (Local Service Discovery) is enabled.
        /// </summary>
        property Optional<bool>^ EnableLsd;

        /// <summary>
        /// Initializes a new instance of the TorrentSessionConfig class with default values.
        /// </summary>
        TorrentSessionConfig()
        {
            ListenInterfaces = Optional<String^>::None();
            OutgoingInterfaces = Optional<String^>::None();
            ProxySettings = gcnew ProxyConfig();
            BandwidthSettings = gcnew BandwidthConfig();
            DhtSettings = gcnew DhtConfig();
            EnableUpnp = Optional<bool>::None();
            EnableNatPmp = Optional<bool>::None();
            EnableLsd = Optional<bool>::None();
        }
    };
}