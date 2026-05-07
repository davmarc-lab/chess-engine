#include "engine/piece/queen.hpp"

namespace chess {
	namespace piece {

		Queen::Queen(const Location &location) :
			Piece(location, 0) {
		}

		std::string Queen::toString() {
			return "Queen = [ " + Piece::toString() + " ]";
		}

	} // namespace piece
} // namespace chess
