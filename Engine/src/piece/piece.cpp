#include "../../include/piece/piece.hpp"

#include "../../include/utils.hpp"

namespace chess {
	std::string Piece::toString() {
		return this->m_location.toString() + ", valid: " + pcond(this->m_location.isValid());
	}
} // namespace chess
