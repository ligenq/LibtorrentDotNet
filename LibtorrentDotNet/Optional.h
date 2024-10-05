#pragma once

namespace LibtorrentDotNet
{
	/// <summary>
	/// Represents an optional value of type T.
	/// </summary>
	/// <typeparam name="T">The type of the optional value.</typeparam>
	generic<typename T>
	public ref struct Optional sealed
	{
	private:
		bool hasValue;
		T value;

	public:
		/// <summary>
		/// Initializes a new instance of the Optional struct without a value.
		/// </summary>
		Optional() : hasValue(false) { }

		/// <summary>
		/// Initializes a new instance of the Optional struct with a value.
		/// </summary>
		/// <param name="value">The value to store.</param>
		Optional(T value) : hasValue(true), value(value) { }

		/// <summary>
		/// Gets a value indicating whether the Optional has a value.
		/// </summary>
		property bool HasValue
		{
			bool get() { return hasValue; }
		}

		/// <summary>
		/// Gets the stored value. Throws an exception if no value is present.
		/// </summary>
		property T Value
		{
			T get()
			{
				if (!hasValue)
				{
					throw gcnew System::InvalidOperationException("No value present");
				}
				return value;
			}
		}

		/// <summary>
		/// Creates an Optional instance without a value.
		/// </summary>
		/// <returns>An Optional instance with no value.</returns>
		static Optional<T>^ None()
		{
			return gcnew Optional<T>();
		}

		/// <summary>
		/// Creates an Optional instance with a value.
		/// </summary>
		/// <param name="value">The value to store.</param>
		/// <returns>An Optional instance containing the specified value.</returns>
		static Optional<T>^ Some(T value)
		{
			return gcnew Optional<T>(value);
		}

		/// <summary>
		/// Tries to get the value stored in the Optional.
		/// </summary>
		/// <param name="result">When this method returns, contains the value stored in the Optional, if present; otherwise, the default value for the type.</param>
		/// <returns>true if the Optional has a value; otherwise, false.</returns>
		bool TryGetValue([System::Runtime::InteropServices::Out] T% result)
		{
			if (hasValue)
			{
				result = value;
				return true;
			}
			result = T(); // Defaults to T() if no value is present
			return false;
		}

		/// <summary>
		/// Gets the value if present; otherwise, returns the specified default value.
		/// </summary>
		/// <param name="defaultValue">The default value to return if no value is present.</param>
		/// <returns>The stored value if present; otherwise, the default value.</returns>
		T GetValueOrDefault(T defaultValue)
		{
			return hasValue ? value : defaultValue;
		}

		/// <summary>
		/// Resets the Optional to have no value.
		/// </summary>
		void Reset()
		{
			hasValue = false;
			value = T(); // Resets value to default (nullptr for reference types)
		}

		/// <summary>
		/// Explicitly converts the Optional to its underlying type.
		/// </summary>
		/// <param name="optional">The Optional to convert.</param>
		/// <returns>The stored value.</returns>
		static explicit operator T(Optional<T> optional)
		{
			return optional.Value; // Will throw if no value is present
		}

		/// <summary>
		/// Explicitly converts the Optional to a boolean indicating whether it has a value.
		/// </summary>
		/// <param name="optional">The Optional to convert.</param>
		/// <returns>true if the Optional has a value; otherwise, false.</returns>
		static explicit operator bool(Optional<T> optional)
		{
			return optional.HasValue;
		}
	};
}