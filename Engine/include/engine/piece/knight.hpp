#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		/**
		 * @class Knight
		 * @brief Knight piece wrapper.
		 */
		class Knight : public Piece {
		public:
			Knight() = delete;

			/**
			 * @brief Instantiate standard Knight piece.
			 *
			 * @param location the piece location
			 */
			Knight(const Location &location);

			Knight(Knight &&) = default;
			Knight(const Knight &) = default;
			Knight &operator=(Knight &&) = default;
			Knight &operator=(const Knight &) = default;

			virtual ~Knight() override = default;

			std::string toString() override;
		};
	} // namespace piece
} // namespace chess

