#include "engine/piece/bishop.hpp"

namespace chess {
	namespace piece {

		Bishop::Bishop(const Location &location) :
			Piece(location, 0) {
		}

		std::string Bishop::toString() {
			return "Bishop = [ " + Piece::toString() + " ]";
		}

	} // namespace piece
} // namespace chess
