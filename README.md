# LibtorrentDotNet

## Overview

LibtorrentDotNet is a C++/CLI wrapper around libtorrent, designed for use in .NET applications. This wrapper allows .NET developers to leverage many of the core features of libtorrent in their projects.

## Features

- Access to common libtorrent functionality through a .NET-friendly interface
- Easy integration with existing .NET projects targeting Windows
- High-performance BitTorrent operations

## Installation

You can install this package using the NuGet Package Manager:

```bash
Install-Package LibtorrentDotNet
```

## Usage

Here's a basic example of how to use LibtorrentDotNet:

```C#
using LibtorrentDotNet;

class Program
{
    static void Main(string[] args)
    {
        using (var torrentSession = TorrentSession.Create())
        {
            torrentSession.TorrentStateUpdated += (sender, torrentStateUpdateEventArgs) =>
            {
                foreach (var status in torrentStateUpdateEventArgs.UpdatedTorrents)
                {
                    if (status.State == TorrentState.Downloading)
                    {
                        Console.WriteLine($"Downloading: {Math.Round(status.Progress * 100, 2)}%");
                    }
                    else
                    {
                        Console.WriteLine(status.State);
                    }
                }
            };

            torrentSession.AddTorrent(new AddTorrentFromMagnetLinkRequest("your magnet link goes here", "a save directory path goes here"));

            Console.WriteLine("Press any key to exit.");
            Console.ReadLine();
        }
    }
}
```

## Requirements

- .NET 8
- Windows 10 and later, x64 only

## Dependencies

This project uses the following third-party library:

### [libtorrent](https://libtorrent.org/)
libtorrent is a C++ library that provides a feature-complete implementation of the BitTorrent protocol.

- **Author**: Arvid Norberg and contributors
- **License**: [BSD License](https://opensource.org/licenses/BSD-3-Clause)
- **Copyright**: (c) 2003-2024 Arvid Norberg, all rights reserved.

The libtorrent library is used to handle torrent functionalities within this project. You can find more information about libtorrent on the [official website](https://libtorrent.org/) or view its [source code on GitHub](https://github.com/arvidn/libtorrent).

## License

This project is licensed under the BSD License. For more information, see the LICENSE.txt file in the root of this repository.

This project includes or depends on libtorrent, which is subject to its own license terms. See LICENSE.txt for details.

## Special Thanks

Special thanks to the open-source community and all contributors of libtorrent for providing an excellent library that made this project possible.
