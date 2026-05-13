#include "view/pawn.hpp"

#include "ecs/entity_manager.hpp"
#include "ecs/system.hpp"
#include "factory.hpp"

const auto em = EntityManager::instance();

namespace view {

	void Pawn::onAttach() {
		this->m_id = factory::factorySquare({{0, 0, 1}, {20, 20, 0}, {}});
		systems::ecs::updateEntityName(this->m_id, "Piece");
	}

	unsigned int Pawn::getId() const { return this->m_id; }

} // namespace view
