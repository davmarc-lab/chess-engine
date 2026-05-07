#include "engine/piece/piece.hpp"

#include "engine/utils.hpp"

namespace chess {
	namespace piece {

		Location Piece::getLocation() { return this->m_location; }

		void Piece::setLocation(const Location &loc) {
			if (loc.isValid())
				this->m_location = loc;
			// TODO : else LOG
		}

		short Piece::getSteps() { return this->m_steps; }

		void Piece::setSteps(const short &steps) {
			if (steps >= 1)
				this->m_steps = steps;
			// TODO : else LOG
		}

		std::string Piece::toString() {
			return this->m_location.toString() + ", valid: " + pcond(this->m_location.isValid()) + ", steps: " + std::to_string(this->m_steps);
		}

	} // namespace piece
} // namespace chess
