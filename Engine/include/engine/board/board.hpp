#pragma once

#include "engine/piece/piece.hpp"
#include "engine/structs.hpp"
#include "engine/utils.hpp"

#include <concepts>
#include <string>
#include <vector>

using namespace chess::piece;

namespace chess {
	namespace board {

		template <typename T>
		concept ext_piece = std::derived_from<T, Piece> || std::same_as<T, Piece>;

		/**
		 * @brief Defines Board settings for each type of board.
		 */
		struct BoardSettings {
			/// number of players
			unsigned int sides = 2;
			/// pieces per side
			unsigned int pieces = 8;
			/// board sizes in squares
			Pair<unsigned int> size = {8, 8};
			/// each side color
			std::vector<Color> colors = {SQUARE_BLACK, SQUARE_WHITE};

			/**
			 * @brief Standard debug string with all object values.
			 *
			 * @return a printable string.
			 */
			std::string toString();
		};

		/**
		 * @brief Base class to wrap a generic chess board.
		 */
		class Board {
		public:
			Board() = delete;

			/**
			 * @brief Default constructor for each extended class.
			 *
			 * @param settings the board settings
			 */
			Board(const BoardSettings &settings);

			template <ext_piece T>
			void addPiece();

			/**
			 * @brief Standard debug string with all object values.
			 *
			 * @return a printable string.
			 */
			virtual std::string toString();

			Board(Board &&) = default;
			Board(const Board &) = default;
			Board &operator=(Board &&) = default;
			Board &operator=(const Board &) = default;

			virtual ~Board() = default;

		private:
			/// board settings
			BoardSettings m_settings;
			/// board pieces
			std::vector<Unique<piece::Piece>> m_pieces;
		};
	} // namespace board
} // namespace chess
