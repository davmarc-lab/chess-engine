#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		class Knight : public Piece {
		public:
			Knight() = default;

			Knight(Knight &&) = default;
			Knight(const Knight &) = default;
			Knight &operator=(Knight &&) = default;
			Knight &operator=(const Knight &) = default;
			virtual ~Knight() override = default;

		private:
		};
	} // namespace piece
} // namespace chess
