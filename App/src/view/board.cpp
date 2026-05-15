#include "view/board.hpp"
#include <cctype>
#include <glm/ext/vector_float4.hpp>
#include <string>

#include "common/logger.hpp"
#include "common/utils.hpp"
#include "ecs/ecs_scene.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system.hpp"

#include "factory.hpp"

const auto em = EntityManager::instance();
const auto ecs = BasicScene::instance();

const glm::vec3 SQUARE_SIZE{40, 40, 0};
const glm::vec4 SQUARE_BLACK{0, 0, 0, 1};
const glm::vec4 SQUARE_WHITE{1, 1, 1, 1};

#define CHAR_OFFSET 97
#define NUM_OFFSET 1

namespace view {

	// macro => fun(N, col, row)
	int Board::index(const int &col, const int &row) const {
		return this->m_settings.size.x * row + col;
	}

	// macro => fun(N, index)
	Pair<unsigned int> Board::coord(const unsigned int &index) const {
		// letter - number
		return {index % this->m_settings.size.x, index / this->m_settings.size.x};
	}

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
				// each letter represent a column
				this->m_references[this->index(i, j)] = sq;
			}
		}
		this->m_id = board;
	}

	unsigned int Board::getCellFromCoord(const char &letter, const short &num) const {
		auto letterIndex = tolower(letter) - CHAR_OFFSET;
		auto index = this->index(letterIndex, num - NUM_OFFSET);
		if (index >= this->getSquaresNum() || index < 0) {
			ERROR("Index out of bound: actual -> " + std::to_string(index) + ", max -> " +
				  std::to_string(this->getSquaresNum()));
			return 0;
		}
		return this->m_references[index];
	}

	Pair<unsigned int> Board::getCoordFromCell(const unsigned int &id) const {
		// TODO binary search could be better
		for (auto i = 0; i < this->getSquaresNum(); i++) {
			if (this->m_references[i] == id) {
				auto arrCoords = this->coord(i);
				arrCoords.x += CHAR_OFFSET;
				arrCoords.y += NUM_OFFSET;
				return arrCoords;
			}
		}
		return {};
	}

	unsigned int Board::getId() const { return this->m_id; }

} // namespace view

namespace systems {
	namespace board {

		void updateCellPresence(const unsigned int &id, const bool &hasPiece) {
			auto c = em->getComponentFromId<CellComponent>(id);
			ASSERT(c != nullptr);

			c->hasPiece = hasPiece;
		}

		bool isPieceInCell(const unsigned int &id) {
			auto c = em->getComponentFromId<CellComponent>(id);
			return c == nullptr ? false : c->hasPiece;
		}

	} // namespace board
} // namespace systems
