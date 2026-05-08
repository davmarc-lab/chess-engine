#include "engine/board/board.hpp"

#include <cstddef>

namespace chess {
	namespace board {

		unsigned int Board::maxPieces() const {
			return this->m_settings.pieces * this->m_settings.sides;
		}

		size_t Board::numCurrentPieces() const {
			return this->m_pieces.size();
		}

		Board::Board(const BoardSettings &settings) :
			m_settings(settings) {
			this->m_pieces.reserve(this->maxPieces());
		}

		std::string res = "";
		std::string BoardSettings::toString() {
			res = "";
			for (size_t i = 0; i < this->colors.size() - 1; i++) {
				res += "\t" + std::to_string(i + 1) + " -> (" + this->colors[i].toString() + "),\n";
			}
			res += "\t" + std::to_string(this->colors.size()) +
				" -> (" + this->colors[this->colors.size() - 1].toString() + ")\n";

			return "sides: " + std::to_string(this->sides) + "\n" +
				"sizes: (" + this->size.toString() + ")\n" +
				"pieces: " + std::to_string(this->pieces) + "\n" +
				"colors: [\n" + res + "]";
		}

		std::string Board::toString() {
			return this->m_settings.toString();
		}

	} // namespace board
} // namespace chess
