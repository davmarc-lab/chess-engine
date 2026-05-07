#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		class Pawn : public Piece {
		public:
			Pawn() = default;

			Pawn(Pawn &&) = default;
			Pawn(const Pawn &) = default;
			Pawn &operator=(Pawn &&) = default;
			Pawn &operator=(const Pawn &) = default;
			virtual ~Pawn() override = default;

		private:
		};
	} // namespace piece
} // namespace chess
