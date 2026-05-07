#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		class Queen : public Piece {
		public:
			Queen() = default;

			Queen(Queen &&) = default;
			Queen(const Queen &) = default;
			Queen &operator=(Queen &&) = default;
			Queen &operator=(const Queen &) = default;
			virtual ~Queen() override = default;

		private:
		};
	} // namespace piece
} // namespace chess
