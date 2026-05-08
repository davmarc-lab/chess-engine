#pragma once

#include <cassert>
#include <string>
namespace chess {
	/**
	 * @brief Represent a square in a chess board.
	 */
	class Location {
	public:
		Location() = delete;

		/**
		 * @brief Instantiate standard location.
		 *
		 * @param letter a char representing the square letter
		 * @param number a short representing the square number
		 */
		Location(const char &letter, const short &number) :
			m_letter(letter), m_number(number) {
		}

		/**
		 * @brief Check the location validity: number > 0, letter = [a, ..., z].
		 *
		 * @return true if the location is valid.
		 */
		bool isValid() const;

		/**
		 * @brief Standard debug string with all object values.
		 *
		 * @return a printable string.
		 */
		std::string toString() const;

		Location(Location &&) = default;
		Location(const Location &) = default;
		Location &operator=(Location &&) = default;
		Location &operator=(const Location &) = default;

		~Location() = default;

	private:
		/// square letter
		char m_letter;
		/// square number
		short m_number;
	};
} // namespace chess
