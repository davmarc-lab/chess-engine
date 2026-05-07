#pragma once

#include <string>
#include "../location.hpp"

namespace chess {

	class Piece {
	public:
		Piece() = delete;
		Piece(Location &location, const short &steps) :
			m_location(location), m_steps(steps) {}

		virtual ~Piece() = default;

		std::string toString();

		Piece(Piece &&) = default;
		Piece(const Piece &) = default;
		Piece &operator=(Piece &&) = default;
		Piece &operator=(const Piece &) = default;

		Location getLocation();

		void setLocation(const Location &loc);

		short getSteps();

		void setSteps(const short &steps);

	protected:
		Location m_location;
		short m_steps;
	};
} // namespace chess
