#pragma once

#include <cassert>
#include <string>
namespace chess {
	class Location {
	public:
		Location() = delete;

		Location(const char &letter, const short number) :
			m_letter(letter), m_number(number) {
		}

		bool isValid();

		std::string toString();

		Location(Location &&) = default;
		Location(const Location &) = default;
		Location &operator=(Location &&) = default;
		Location &operator=(const Location &) = default;
		~Location() = default;

	private:
		char m_letter;
		short m_number;
	};
} // namespace chess
