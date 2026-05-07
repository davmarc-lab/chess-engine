#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		/**
		 * @class Queen
		 * @brief Queen piece wrapper.
		 */
		class Queen : public Piece {
		public:
			Queen() = delete;

			/**
			 * @brief Instantiate standard Queen piece.
			 *
			 * @param location the piece location
			 */
			Queen(const Location &location);

			Queen(Queen &&) = default;
			Queen(const Queen &) = default;
			Queen &operator=(Queen &&) = default;
			Queen &operator=(const Queen &) = default;

			virtual ~Queen() override = default;

			std::string toString() override;
		};
	} // namespace piece
} // namespace chess
