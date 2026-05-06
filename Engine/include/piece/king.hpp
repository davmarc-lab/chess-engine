#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		class King : public Piece{
		public:
			King() = default;

			King(King &&) = default;
			King(const King &) = default;
			King &operator=(King &&) = default;
			King &operator=(const King &) = default;
			virtual ~King() override = default;

		private:
		};
	} // namespace piece
} // namespace chess
