#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		/**
		 * @class Bishop
		 * @brief Bishop piece wrapper.
		 */
		class Bishop : public Piece {
		public:
			Bishop() = delete;

			/**
			 * @brief Instantiate standard Bishop piece.
			 *
			 * @param location the piece location
			 */
			Bishop(const Location &location);

			Bishop(Bishop &&) = default;
			Bishop(const Bishop &) = default;
			Bishop &operator=(Bishop &&) = default;
			Bishop &operator=(const Bishop &) = default;

			virtual ~Bishop() override = default;

			std::string toString() override;
		};
	} // namespace piece
} // namespace chess
