#include "engine/piece/rook.hpp"

namespace chess {
	namespace piece {

		Rook::Rook(const Location &location) :
			Piece(location, 0) {
		}

		std::string Rook::toString() {
			return "Rook = [ " + Piece::toString() + " ]";
		}

	} // namespace piece
} // namespace chess
