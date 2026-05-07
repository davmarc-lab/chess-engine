#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		class Rook : public Piece {
		public:
			Rook() = default;

			Rook(Rook &&) = default;
			Rook(const Rook &) = default;
			Rook &operator=(Rook &&) = default;
			Rook &operator=(const Rook &) = default;
			virtual ~Rook() override = default;

		private:
		};
	} // namespace piece
} // namespace chess
