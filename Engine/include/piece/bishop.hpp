#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		class Bishop : public Piece {
		public:
			Bishop() = default;

			Bishop(Bishop &&) = default;
			Bishop(const Bishop &) = default;
			Bishop &operator=(Bishop &&) = default;
			Bishop &operator=(const Bishop &) = default;
			virtual ~Bishop() override = default;

		private:
		};
	} // namespace piece
} // namespace chess
