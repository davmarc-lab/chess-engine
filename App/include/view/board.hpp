#pragma once

#include "engine/board/board.hpp"
namespace view {
	class Board {
	public:
		Board() = default;

		void onAttach();

		unsigned int getId() const;

		Board(Board &&) = default;
		Board(const Board &) = default;
		Board &operator=(Board &&) = default;
		Board &operator=(const Board &) = default;

		~Board() = default;

	private:
		unsigned int m_id;
        chess::board::BoardSettings m_settings{};
		chess::board::Board m_board{m_settings};
	};
} // namespace view
