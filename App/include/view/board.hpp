#pragma once

#include "engine/board/board.hpp"
namespace view {

	class Board {
	public:
		Board() = default;

		/**
		 * @brief Init the board view.
		 */
		void onAttach();

		/**
		 * @brief Retrieves the board ecs id.
		 *
		 * @return the ecs id.
		 */
		unsigned int getId() const;

		/**
		 * @brief Retrieves the cell index in the table from the given letter and number.
		 * If the location is not valid it return 0.
		 *
		 * @param letter the cell letter
		 * @param num the cell number
		 *
		 * @return the cell ecs id or 0 if the location is not valid.
		 */
		unsigned int getCellFromCoord(const char &letter, const short &num) const;

		/**
		 * @brief Retrieves cell coordinates, in board space, of the given cell.
		 *
		 * @param id the cell ecs id
		 *
		 * @return a Pair with cell letter as number and cell number.
		 *
		 * @see chess::Location
		 */
		Pair<unsigned int> getCoordFromCell(const unsigned int &id) const;

		int getSquaresNum() const;

		Board(Board &&) = default;
		Board(const Board &) = default;
		Board &operator=(Board &&) = default;
		Board &operator=(const Board &) = default;

		~Board() { delete m_references; }

	private:
		unsigned int m_id;
		chess::board::BoardSettings m_settings{};
		chess::board::Board m_board{m_settings};
		unsigned int *m_references;

		// c friendly naming
		int index(const int &col, const int &row) const;
		Pair<unsigned int> coord(const unsigned int &index) const;
	};

} // namespace view

namespace systems {
	namespace board {

		void updateCellPresence(const unsigned int &id, const bool &hasPiece);

        bool isPieceInCell(const unsigned int& id);

	}
} // namespace systems
