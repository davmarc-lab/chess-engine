#include "engine/piece/pawn.hpp"

namespace chess {
	namespace piece {

		Pawn::Pawn(const Location &location) :
			Piece(location, 1) {
		}

		std::string Pawn::toString() {
			return "Pawn = [ " + Piece::toString() + " ]";
		}

	} // namespace piece
} // namespace chess
