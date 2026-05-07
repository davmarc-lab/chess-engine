#pragma once

#include "piece.hpp"

namespace chess {
	namespace piece {

		/**
		 * @class Pawn
		 * @brief Pawn piece wrapper.
		 */
		class Pawn : public Piece {
		public:
			Pawn() = delete;

			/**
			 * @brief Instantiate standard Pawn piece.
			 *
			 * @param location the piece location
			 */
			Pawn(const Location &location);

			Pawn(Pawn &&) = default;
			Pawn(const Pawn &) = default;
			Pawn &operator=(Pawn &&) = default;
			Pawn &operator=(const Pawn &) = default;

			virtual ~Pawn() override = default;

			std::string toString() override;
		};
	} // namespace piece
} // namespace chess
