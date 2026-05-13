#pragma once

#include "engine/board/board.hpp"
namespace view {
	class Board {
	public:
		Board() = default;

		void onAttach();

		unsigned int getId() const;

		unsigned int getCellFromCoord(const char &letter, const short &num) const;

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
