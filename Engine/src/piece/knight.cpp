#include "engine/piece/knight.hpp"

namespace chess {
	namespace piece {

		Knight::Knight(const Location &location) :
			Piece(location, 3) {
		}

		std::string Knight::toString() {
			return "Knight = [ " + Piece::toString() + " ]";
		}

	} // namespace piece
} // namespace chess
