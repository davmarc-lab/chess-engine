#include "engine/piece/piece.hpp"

#include "engine/utils.hpp"

namespace chess {
	std::string Piece::toString() {
		return this->m_location.toString() + ", valid: " + pcond(this->m_location.isValid());
	}
} // namespace chess
