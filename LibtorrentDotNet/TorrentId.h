#pragma once

using namespace System;

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents a unique identifier for a torrent.
	/// </summary>
	public ref class TorrentId sealed
	{
	public:
		/// <summary>
		/// Initializes a new instance of the TorrentId class.
		/// </summary>
		/// <param name="infoHash">The info hash of the torrent. Must be either 40 (SHA1) or 64 (SHA256) characters long.</param>
		/// <exception cref="System::ArgumentNullException">Thrown when infoHash is null or empty.</exception>
		/// <exception cref="System::ArgumentException">Thrown when infoHash is not 40 or 64 characters long.</exception>
		TorrentId(String^ infoHash)
		{
			if (String::IsNullOrEmpty(infoHash))
				throw gcnew ArgumentNullException("infoHash");

			if (infoHash->Length != 40 && infoHash->Length != 64)
				throw gcnew ArgumentException("Info hash must be either 40 (SHA1) or 64 (SHA256) characters long.");

			this->infoHash = infoHash;
			cachedHashCode = infoHash->GetHashCode();
		}

		/// <summary>
		/// Returns a string representation of the TorrentId.
		/// </summary>
		/// <returns>The info hash as a string.</returns>
		String^ ToString() override
		{
			return infoHash;
		}

		/// <summary>
		/// Determines whether two TorrentId instances are equal.
		/// </summary>
		/// <param name="left">The first TorrentId to compare.</param>
		/// <param name="right">The second TorrentId to compare.</param>
		/// <returns>true if the TorrentIds are equal; otherwise, false.</returns>
		static bool operator ==(TorrentId^ left, TorrentId^ right)
		{
			if (ReferenceEquals(left, nullptr))
				return ReferenceEquals(right, nullptr);

			if (ReferenceEquals(right, nullptr))
				return false;

			return left->Equals(right);
		}

		/// <summary>
		/// Determines whether two TorrentId instances are not equal.
		/// </summary>
		/// <param name="left">The first TorrentId to compare.</param>
		/// <param name="right">The second TorrentId to compare.</param>
		/// <returns>true if the TorrentIds are not equal; otherwise, false.</returns>
		static bool operator !=(TorrentId^ left, TorrentId^ right)
		{
			return !(left == right);
		}

		/// <summary>
		/// Determines whether the specified object is equal to the current TorrentId.
		/// </summary>
		/// <param name="obj">The object to compare with the current TorrentId.</param>
		/// <returns>true if the specified object is equal to the current TorrentId; otherwise, false.</returns>
		bool Equals(Object^ obj) override
		{
			if (obj == nullptr || GetType() != obj->GetType())
				return false;

			auto other = safe_cast<TorrentId^>(obj);
			return infoHash == other->infoHash;
		}

		/// <summary>
		/// Returns the hash code for this TorrentId.
		/// </summary>
		/// <returns>A 32-bit signed integer hash code.</returns>
		int GetHashCode() override
		{
			return cachedHashCode;
		}

	private:
		initonly String^ infoHash;
		initonly int cachedHashCode;
	};
}