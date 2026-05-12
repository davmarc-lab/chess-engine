#include "view/board.hpp"
#include <glm/ext/vector_float4.hpp>

#include "ecs/ecs_scene.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system.hpp"

#include "factory.hpp"

const auto em = EntityManager::instance();
const auto ecs = BasicScene::instance();

const glm::vec3 SQUARE_SIZE{40, 40, 0};
const glm::vec4 SQUARE_BLACK{0, 0, 0, 1};
const glm::vec4 SQUARE_WHITE{1, 1, 1, 1};

namespace view {

	void Board::onAttach() {
		auto board = em->createEntity();
		em->addComponent<ParentComponent>(board);
		em->addComponent<Transform>(board);
		for (auto j = 0; j < this->m_settings.size.y; j++) {
			auto rowCheck = j % 2 == 0;
			glm::vec4 col = SQUARE_BLACK;
			for (auto i = 0; i < this->m_settings.size.x; i++) {
				auto pos = SQUARE_SIZE + SQUARE_SIZE + (glm::vec3{2, 2, 0} * SQUARE_SIZE * glm::vec3{i, j, 0});
				if (rowCheck)
					col = i % 2 == 0 ? SQUARE_BLACK : SQUARE_WHITE;
				else
					col = i % 2 != 0 ? SQUARE_BLACK : SQUARE_WHITE;
				auto sq = factory::factorySquare({pos + glm::vec3{2 * i, 2 * j, 0}, SQUARE_SIZE, {}}, col);
				systems::parent::addChild(board, sq);
			}
		}
		this->m_id = board;
	}

	unsigned int Board::getId() const { return this->m_id; }

} // namespace view
