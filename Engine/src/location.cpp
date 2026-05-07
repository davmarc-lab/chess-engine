#include "engine/location.hpp"

namespace chess {
	bool Location::isValid() const {
		return this->m_letter >= 'a' && this->m_letter <= 'z' &&
			this->m_number >= 1;
	}

	std::string Location::toString() const {
		return this->m_letter + std::to_string(this->m_number);
	}
} // namespace chess
