#pragma once

#include "engine/piece/piece.hpp"
#include "engine/structs.hpp"
#include "common/utils.hpp"

#include <concepts>
#include <iostream>
#include <string>
#include <utility>
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
			Pair<unsigned int> size = {3, 3};
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

			/**
			 * @brief Retrieves the maximum pieces that can be on the board.
			 *
			 * @return the maximum pieces.
			 */
			unsigned int maxPieces() const;

			/**
			 * @brief Retrieves the current number of pieces on the board.
			 *
			 * @return the number of pieces.
			 */
			size_t numCurrentPieces() const;

			/// testing custom template param
			template <ext_piece T>
			inline void checkConcept() {
				// TODO - find a way to retrieve bool if T extends Piece
				std::cout << std::string(CLASSNAME(T)) + " - OK!\n";
			}

			/**
			 * @brief Add a Piece to the board.
			 *
			 * @tparam T the type of Piece, it must extend Piece
			 * @tparam Args list of arguments passed to T constructor
			 * @param args arguments list
			 */
			template <ext_piece T, typename... Args>
			inline void addPiece(Args &&...args) {
				if (this->m_pieces.size() < this->maxPieces()) {
					auto piece = CreateUnique<T>(std::forward<Args>(args)...);
					this->m_pieces.push_back(std::move(piece));
				}
			}

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
