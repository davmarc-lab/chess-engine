#pragma once

#include "engine/location.hpp"
#include "piece.hpp"

namespace chess {
	namespace piece {

		class King : public Piece {
		public:
			King() = delete;

			/**
			 * @brief Instantiate standard King piece.
			 *
			 * @param location the piece location
			 */
			King(const Location &location);

			King(King &&) = default;
			King(const King &) = default;
			King &operator=(King &&) = default;
			King &operator=(const King &) = default;

			virtual ~King() override = default;

			std::string toString() override;
		};
	} // namespace piece
} // namespace chess
