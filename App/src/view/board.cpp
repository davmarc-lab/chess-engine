#include "view/board.hpp"
#include <cctype>
#include <glm/ext/vector_float4.hpp>

#include "common/logger.hpp"
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

	int Board::index(const int &x, const int &y) const { return this->m_settings.size.x * x + y; }

	int Board::getSquaresNum() const { return this->m_settings.size.x * this->m_settings.size.y; }

	void Board::onAttach() {
		// init board reference
		this->m_references = new unsigned int[this->getSquaresNum()]();

		auto board = em->createEntity();
		em->addComponent<ParentComponent>(board);
		em->addComponent<Transform>(board);
		for (auto j = 0; j < this->m_settings.size.y; j++) {
			auto rowCheck = j % 2 == 0;
			glm::vec4 col = SQUARE_BLACK;
			for (auto i = 0; i < this->m_settings.size.x; i++) {
				auto pos = SQUARE_SIZE + SQUARE_SIZE +
					(glm::vec3{2, 2, 0} * SQUARE_SIZE * glm::vec3{i, j, 0});
				if (rowCheck)
					col = i % 2 == 0 ? SQUARE_BLACK : SQUARE_WHITE;
				else
					col = i % 2 != 0 ? SQUARE_BLACK : SQUARE_WHITE;
				pos += glm::vec3{2 * i, 2 * j, 0};
				auto sq = factory::factorySquare({pos, SQUARE_SIZE, {}}, col);
				em->addComponent<ClickableComponent>(sq);
				em->addComponent<CellComponent>(sq);
				em->addComponent<AABBCollider>(sq, pos, SQUARE_SIZE);
				systems::parent::addChild(board, sq);

				// storing consecutively in memory each "view" row
				this->m_references[this->index(j, i)] = sq;
				INFO(std::to_string(this->index(j, i)) + " => " + std::to_string(sq));
			}
		}
		this->m_id = board;
	}

	unsigned int Board::getCellFromCoord(const char &letter, const short &num) const {
		auto letterIndex = tolower(letter) - 97;
		auto index = this->index(letterIndex, num - 1);
		if (index >= this->getSquaresNum() || index < 0) {
			ERROR("Index out of bound: actual -> " + std::to_string(index) + ", max -> " +
				  std::to_string(this->getSquaresNum()));
            return 0;
		}
		INFO(std::to_string(this->m_references[index]));
		return this->m_references[index];
	}

	unsigned int Board::getId() const { return this->m_id; }

} // namespace view
