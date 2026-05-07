#include "engine/piece/king.hpp"

namespace chess {
	namespace piece {

		King::King(const Location &location) :
			Piece(location, 1) {
		}

		std::string King::toString() {
			return "King = [ " + Piece::toString() + " ]";
		}

	} // namespace piece
} // namespace chess
