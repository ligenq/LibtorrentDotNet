#pragma once

using namespace System;

namespace LibtorrentDotNet
{
	public ref class TorrentException sealed : public Exception
	{
	public:
		TorrentException(String^ message) : Exception(message) {}
	};

	public interface class ILogger
	{
		enum class LogLevel { Debug, Info, Warning, Error };
		void Log(LogLevel level, String^ message);
	};
}

