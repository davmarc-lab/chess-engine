#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		/**
		 * @class Rook
		 * @brief Rook piece wrapper.
		 */
		class Rook : public Piece {
		public:
			Rook() = delete;

			/**
			 * @brief Instantiate standard Rook piece.
			 *
			 * @param location the piece location
			 */
			Rook(const Location &location);

			Rook(Rook &&) = default;
			Rook(const Rook &) = default;
			Rook &operator=(Rook &&) = default;
			Rook &operator=(const Rook &) = default;

			virtual ~Rook() override = default;

			std::string toString() override;
		};
	} // namespace piece
} // namespace chess
