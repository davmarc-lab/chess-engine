#include "utils.hpp"

#include "ecs/component.hpp"
#include "ecs/ecs_scene.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

const auto em = EntityManager::instance();
const auto scene = BasicScene::instance();

const auto BOUNDING_BOX_COLOR = glm::vec4{1, 0, 0, 1};

/**
 * @brief Data structure to store bounding box information for rendering purpose.
 */
struct BoundingBox {
	/// bounding box shader program
	ogl::ShaderProgram program = ogl::ShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
	/// bounding box vao
	ogl::VertexArray vao{};
	/// bounding box geometry vbo
	ogl::VertexBuffer vbog{};
	/// bounding box colors vbo
	ogl::VertexBuffer vboc{};
	/// bounding box vertex coords
	std::vector<glm::vec3> coords{};
	/// bounding box colors
	std::vector<glm::vec4> colors{};
	/// tells if the bounding box shaders is created
	bool init = false;

	/**
	 * @brief Clears all graphics data for the bounding box
	 *
	 * @note This method should be called when detaching a State.
	 */
	void clear() {
		vbog.onDetach();
		vboc.onDetach();
		vao.onDetach();
		program.clear();
	}
} defaultShader;

ogl::ShaderProgram stencil = ogl::ShaderProgram("vertexShader.glsl", "stencilShader.glsl");

namespace systems {
	namespace ecs {
		bool removeEntityFromManager(const unsigned int &id) {
			auto c = em->getComponentFromId<ParentComponent>(id);
			if (c != nullptr) {
				for (auto child : c->children) {
					removeEntityFromManager(child);
				}
			}
			return em->removeEntity(id);
		}

		void removeEntityFromScene(const unsigned int &id) {
			auto c = em->getComponentFromId<ParentComponent>(id);
			if (c != nullptr) {
				for (auto child : c->children) {
					scene->removeEntity(child);
				}
			}
			scene->removeEntity(id);
		}

		bool removeEntityFromAll(const unsigned int &id) {
			removeEntityFromScene(id);
			return em->removeEntity(id);
		}

		void cleanAll() {
			// remove all entities from memory
			for (const auto e : em->getEntities()) {
				removeEntityFromAll(e);
			}
			// reset ecs index
			em->resetIndex();
		}

		std::string getEntityName(const unsigned int &id) {
			return em->isEntityValid(id) ? em->getEntityName(id) : "INVALID";
		}

		void updateEntityName(const unsigned int &id, const std::string &name) {
			ASSERT(em->isEntityValid(id));
			em->setEntityName(id, name);
		}
	} // namespace ecs
	namespace transform {
		void updatePosition(const unsigned int &id, const glm::vec3 &position) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					auto cpos = getPosition(child);
					auto offset = cpos - c->getPosition();
					offset.z = 0;
					updatePosition(child, position + offset);
				}
			}

			c->setPosition(position);
		}

		void updateScale(const unsigned int &id, const glm::vec3 &scale) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->setScale(scale);
		}

		void updateRotation(const unsigned int &id, const glm::vec3 &rotation) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					updateRotation(child, rotation);
				}
			}

			c->setRotation(rotation);
		}

		void addPosition(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			auto p = em->getComponentFromId<ParentComponent>(id);
			if (p != nullptr) {
				for (auto child : p->children) {
					auto cpos = getPosition(child);
					auto modelOffset = cpos - c->getPosition();
					modelOffset.z = 0;
					updatePosition(child, c->getPosition() + modelOffset + offset);
				}
			}

			c->addPosition(offset);
		}

		void addScale(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->addScale(offset);
		}

		void addRotation(const unsigned int &id, const glm::vec3 &offset) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			c->addRotation(offset);
		}

		glm::vec3 getPosition(const unsigned int &id) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			return c->position;
		}

		glm::vec3 getScale(const unsigned int &id) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			return c->scale;
		}

		glm::vec3 getRotation(const unsigned int &id) {
			auto c = em->getComponentFromId<Transform>(id);
			ASSERT(c != nullptr);

			return c->rotation;
		}

		glm::mat4 getModelMatrix(const unsigned int &id) {
			auto tc = em->getComponentFromId<Transform>(id);
			ASSERT(tc != nullptr);

			if (tc->isDirty())
				updateModelMatrix(id);
			return tc->model;
		}

		void updateModelMatrix(const unsigned int &id) {
			auto tc = em->getComponentFromId<Transform>(id);
			if (tc == nullptr)
				return;

			tc->updateModelMatrix();
			::systems::collision::updateCollider(id);
		}

		void updateAllModelMatrix() {
			for (auto id : em->getEntitiesFromComponent<Transform>()) {
				updateModelMatrix(id);
			}
		}
	} // namespace transform

	namespace collision {
		void updateParentCollider(const unsigned int &id) {
			auto box = em->getComponentFromId<ColliderComponent>(id);
			auto tc = em->getComponentFromId<Transform>(id);
			auto offset = tc->position - box->position;
			box->position = tc->position;
			box->botLeft += offset;
			box->topRight += offset;
		}

		void updateCollider(const unsigned int &id) {
			auto bc = em->getComponentFromId<ColliderComponent>(id);
			if (bc == nullptr)
				return;

			auto pc = em->getComponentFromId<ParentComponent>(id);
			// has children
			if (pc != nullptr && !pc->children.empty()) {
				updateParentCollider(id);
				return;
			}

			// default collider update
			auto cc = em->getComponentFromId<VertexComponent>(id);
			if (cc != nullptr) {
				bc->updateCollider(cc->getVertexCoords(), ::systems::transform::getModelMatrix(id));
			}
		}

		void updateAllColliders() {
			auto ids = em->getEntitiesFromComponent<ColliderComponent>();
			for (auto id : ids) {
				updateCollider(id);
			}
		}

		bool isColliding(const unsigned int &first, const unsigned int &second) {
			if (first == second)
				return false;
			auto fc = em->getComponentFromId<ColliderComponent>(first);
			auto sc = em->getComponentFromId<ColliderComponent>(second);
			ASSERT(fc != nullptr);
			ASSERT(sc != nullptr);

			return fc->isColliding(*sc);
		}

		Pair<glm::vec3> getCollider(const unsigned int &id) {
			auto c = em->getComponentFromId<ColliderComponent>(id);
			ASSERT(c != nullptr);

			return {c->botLeft, c->topRight};
		}

		ColliderType getColliderType(const unsigned int &id) {
			auto c = em->getComponentFromId<ColliderComponent>(id);
			ASSERT(c != nullptr);

			return c->type;
		}

		void updateColliderType(const unsigned int &id, const ColliderType &type) {
			auto c = em->getComponentFromId<ColliderComponent>(id);
			ASSERT(c != nullptr);

			c->type = type;
		}

		void updateSimulated(const unsigned int &id, const bool &simulated) {
			auto c = em->getComponentFromId<ColliderComponent>(id);
			ASSERT(c != nullptr);

			c->isStatic = !simulated;
		}

		void compressBoundingBox() {
			for (auto id : em->getEntitiesFromComponent<ParentComponent>()) {
				auto c = em->getComponentFromId<ParentComponent>(id);
				auto pv = em->getComponentFromId<VertexComponent>(id);
				auto box = em->getComponentFromId<ColliderComponent>(id);
				// remove ColliderComponent component from each child and update the parent
				for (auto child : c->children) {
					if (em->entityHasComponent<ColliderComponent>(child)) {
						auto cv = em->getComponentFromId<VertexComponent>(child);
						auto model = ::systems::transform::getModelMatrix(child);
						auto bot = glm::vec3(1, 1, 1);
						bool first = true;

						for (auto vertex : cv->getVertexCoords()) {
							auto elem = model * glm::vec4(vertex, 1);
							if (first) {
								bot = elem;
								first = false;
							}
							bot.x = bot.x >= elem.x ? elem.x : bot.x;
							bot.y = bot.y >= elem.y ? elem.y : bot.y;
							bot.z = bot.z >= elem.z ? elem.z : bot.z;
						}

						auto top = glm::vec3(1, 1, 1);
						first = true;
						for (auto vertex : cv->getVertexCoords()) {
							auto elem = model * glm::vec4(vertex, 1);
							if (first) {
								top = elem;
								first = false;
							}
							top.x = top.x <= elem.x ? elem.x : top.x;
							top.y = top.y <= elem.y ? elem.y : top.y;
							top.z = top.z <= elem.z ? elem.z : top.z;
						}
						box->botLeft.x = box->botLeft.x < bot.x ? box->botLeft.x : bot.x;
						box->botLeft.y = box->botLeft.y < bot.y ? box->botLeft.y : bot.y;
						box->botLeft.z = box->botLeft.z < bot.z ? box->botLeft.z : bot.z;
						box->topRight.x = box->topRight.x > top.x ? box->topRight.x : top.x;
						box->topRight.y = box->topRight.y > top.y ? box->topRight.y : top.y;
						box->topRight.z = box->topRight.z > top.z ? box->topRight.z : top.z;
						em->removeComponent<ColliderComponent>(child);
					}
				}
			}
		}

		std::vector<Pair<unsigned int>> getCollisions() {
			std::vector<Pair<unsigned int>> coll{};
			auto collidable = em->getEntitiesFromComponent<ColliderComponent>();
			for (auto first : collidable) {
				for (auto other : collidable) {
					if (first == other)
						continue;
					{
						auto fc = em->getComponentFromId<ParentComponent>(first);
						if (fc != nullptr) {
							if (std::find(ALL(fc->children), other) != fc->children.end())
								continue;
						}
						auto oc = em->getComponentFromId<ParentComponent>(other);
						if (oc != nullptr) {
							if (std::find(ALL(oc->children), first) != oc->children.end())
								continue;
						}
					}
					if (isColliding(first, other)) {
						if (!(std::find(ALL(coll), Pair<unsigned int>{first, other}) != coll.end() ||
							  std::find(ALL(coll), Pair<unsigned int>{other, first}) != coll.end())) {
							coll.push_back({first, other});
						}
					}
				}
			}
			return coll;
		}

	} // namespace collision

	namespace camera {
		Shared<ogl::Camera> getCamera(const unsigned int &id) {
			auto c = em->getComponentFromId<CameraComponent>(id);
			ASSERT(c != nullptr);
			return c->camera;
		}

		void updateCameraCollider(const unsigned int &id, const glm::vec3 &position, const glm::vec3 &size) {
			auto c = em->getComponentFromId<ColliderComponent>(id);
			if (c != nullptr)
				c->updateCollider(position, size);
		}
	} // namespace camera

	namespace parent {
		void addChild(const unsigned int &parent, const unsigned int &child) {
			auto c = em->getComponentFromId<ParentComponent>(parent);
			ASSERT(c != nullptr);

			c->children.push_back(child);
		}

		void removeChild(const unsigned int &parent, const unsigned int &child) {
			auto c = em->getComponentFromId<ParentComponent>(parent);
			ASSERT(c != nullptr);

			c->children.erase(std::find(ALL(c->children), child));
		}
	} // namespace parent

	namespace input {
		// it can be optimized merging from each InputComponent all callbacks.
		std::vector<std::pair<unsigned int, std::function<void()>>> getKeysCallback(const unsigned int &id) {
			// std::cerr << "Optimize this method (read comment above implementation): LINE -> " << __LINE__ << ", FILE -> " << __FILE__ << "\n";
			std::vector<std::pair<unsigned int, std::function<void()>>> res{};
			auto ic = em->getComponentFromId<InputComponent>(id);
			ASSERT(ic != nullptr);

			for (auto k : ic->getAllKeys()) {
				res.emplace_back(k, ic->callbacks.at(k));
			}
			return res;
		}

		void setKeyCallback(const unsigned int &id, const unsigned int &key, std::function<void()> func) {
			auto ic = em->getComponentFromId<InputComponent>(id);
			if (ic == nullptr)
				return;

			ic->registerAction(key, func);
		}
	} // namespace input

	namespace animation {
		void executeNextFrame(const float &currentTime) {
			std::vector<unsigned int> rmv{};
			for (auto ett : em->getEntitiesFromComponent<TimeAnimation>()) {
				auto c = em->getComponentFromId<TimeAnimation>(ett);
				ASSERT(c != nullptr);

				if (c->dead)
					continue;

				c->updateTick(currentTime);
				if (c->dead)
					rmv.push_back(ett);
			}
			for (auto e : rmv) {
				::systems::ecs::removeEntityFromAll(e);
			}
		}
	} // namespace animation

	namespace texture {
		ogl::Texture getTexture(const unsigned int &id) {
			auto c = em->getComponentFromId<TextureComponent>(id);
			ASSERT(c != nullptr);
			return c->texture;
		}

		void setTexture(const unsigned int &id, const ogl::Texture &texture) {
			auto c = em->getComponentFromId<TextureComponent>(id);
			ASSERT(c != nullptr);

			c->texture = texture;
		}

		std::string getTexturePath(const unsigned int &id) {
			auto c = em->getComponentFromId<TextureComponent>(id);
			ASSERT(c != nullptr);
			return c->path;
		}

		void updateTexturePath(const unsigned int &id, const std::string &path) {
			auto c = em->getComponentFromId<TextureComponent>(id);
			ASSERT(c != nullptr);
			c->path = path;

			int width, height, nrChannels;
			auto data = readImageData(path, width, height, nrChannels);
			c->texture.setSize({(unsigned int)width, (unsigned int)height});
			c->texture.createTexture2D(data);
			c->texture.unbind();
		}

		bool isMeshReflective(const unsigned int &id) {
			auto c = em->getComponentFromId<ShaderComponent>(id);
			ASSERT(c != nullptr);
			return c->reflective;
		}

		void setMeshReflective(const unsigned int &id, const bool &val) {
			auto c = em->getComponentFromId<ShaderComponent>(id);
			ASSERT(c != nullptr);
			c->reflective = val;
		}

	} // namespace texture

	namespace material {
		Material getMaterial(const unsigned int &id) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			return c->material;
		}

		std::string getMaterialName(const unsigned int &id) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			return c->material.name;
		}

		void updateAmbient(const unsigned int &id, const glm::vec3 &val) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material.ambient = val;
		}

		void updateDiffuse(const unsigned int &id, const glm::vec3 &val) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material.diffuse = val;
		}

		void updateSpecular(const unsigned int &id, const glm::vec3 &val) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material.specular = val;
		}

		void updateShininess(const unsigned int &id, const float &val) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material.shininess = val;
		}

		void updateMaterial(const unsigned int &id, const Material &material) {
			auto c = em->getComponentFromId<MaterialComponent>(id);
			ASSERT(c != nullptr);

			c->material = material;
		}

	} // namespace material

	namespace render {
		void clear() {
			stencil.clear();
		}

		void initStencilShader() {
			stencil.createShaderProgram();
		}

		void prepareInstancedMesh(const std::vector<glm::mat4> &models, const std::vector<glm::vec4> &colors) {
			ogl::Renderer::instance()->prepareBuffers(models, colors);
		}

		void renderSkybox(const unsigned int &id, const Shared<ogl::ShaderProgram> &shader) {
			if (!em->getEntitiesFromComponent<SkyboxComponent>().size()) {
				std::cerr << "No skybox detected\n";
				return;
			}
			shader->use();
			auto rc = em->getComponentFromId<RenderComponent>(id);
			shader->setMat4("view", glm::mat4(glm::mat3(scene->getCamera()->getViewMatrix())));
			shader->setMat4("proj", scene->getCamera()->getProjMatrix());
			rc->call();
		}

		void renderAllMeshes() {
			// auto sid = em->getEntitiesFromComponent<SkyboxComponent>();
			// if (sid.empty())
			// 	return;
			// auto skybox = em->getComponentFromId<SkyboxComponent>(sid[0]);
			// auto skyboxTexture = em->getComponentFromId<TextureComponent>(sid[0]);
			for (auto [shader, etts] : scene->getShaderEntityMap()) {
				shader->use();
				for (auto id : etts) {
					auto rc = em->getComponentFromId<RenderComponent>(id);
					if (em->entityHasComponent<Transform>(id)) {
						shader->setMat4("model", ::systems::transform::getModelMatrix(id));
					}
					if (rc != nullptr)
						rc->call();

					auto pc = em->getComponentFromId<ParentComponent>(id);
					if (pc != nullptr) {
						for (auto child : pc->children) {
							if (em->entityHasComponent<Transform>(child)) {
								shader->setMat4("model", ::systems::transform::getModelMatrix(child));
							}
							auto dc = em->getComponentFromId<RenderComponent>(child);
							if (dc != nullptr) {
								dc->call();
							}
						}
					}
				}
			}
		}

		void renderInstancedMeshes() {
			ogl::Renderer::instance()->drawAllInstanced();
		}

		std::vector<glm::vec3> getBoxLines(glm::vec3 &botLeft, glm::vec3 &topRight) {
			std::vector<glm::vec3> lines{};
			lines.push_back(topRight);
			lines.push_back({botLeft.x, topRight.y, topRight.z});
			lines.push_back({botLeft.x, topRight.y, topRight.z});
			lines.push_back({botLeft.x, botLeft.y, topRight.z});
			lines.push_back({botLeft.x, botLeft.y, topRight.z});
			lines.push_back({topRight.x, botLeft.y, topRight.z});
			lines.push_back({topRight.x, botLeft.y, topRight.z});
			lines.push_back(topRight);

			// "BACK" face
			lines.push_back({topRight.x, topRight.y, botLeft.z});
			lines.push_back({botLeft.x, topRight.y, botLeft.z});
			lines.push_back({botLeft.x, topRight.y, botLeft.z});
			lines.push_back({botLeft.x, botLeft.y, botLeft.z});
			lines.push_back({botLeft.x, botLeft.y, botLeft.z});
			lines.push_back({topRight.x, botLeft.y, botLeft.z});
			lines.push_back({topRight.x, botLeft.y, botLeft.z});
			lines.push_back({topRight.x, topRight.y, botLeft.z});

			// "TOP" face
			lines.push_back(topRight);
			lines.push_back({topRight.x, topRight.y, botLeft.z});
			lines.push_back({botLeft.x, topRight.y, botLeft.z});
			lines.push_back({botLeft.x, topRight.y, topRight.z});

			// "BOT" face
			lines.push_back(botLeft);
			lines.push_back({botLeft.x, botLeft.y, topRight.z});
			lines.push_back({topRight.x, botLeft.y, topRight.z});
			lines.push_back({topRight.x, botLeft.y, botLeft.z});
			return lines;
		}

		void renderBoundingBox() {
			if (!defaultShader.init) {
				defaultShader.program.createShaderProgram();
				defaultShader.vao.onAttach();
				defaultShader.vbog.onAttach();
				defaultShader.vboc.onAttach();
				defaultShader.init = true;
				defaultShader.vao.bind();
				defaultShader.vbog.setup(defaultShader.coords.data(), defaultShader.coords.size(), GL_STATIC_DRAW);
				defaultShader.vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

				defaultShader.vboc.setup(defaultShader.colors.data(), defaultShader.colors.size(), GL_STATIC_DRAW);
				defaultShader.vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			}

			defaultShader.program.use();
			for (auto ett : em->getEntitiesFromComponent<ColliderComponent>()) {
				auto box = em->getComponentFromId<ColliderComponent>(ett);
				defaultShader.coords.clear();

				for (auto points : getBoxLines(box->botLeft, box->topRight)) {
					defaultShader.coords.push_back(points);
				}

				for (auto i = 0; i < defaultShader.coords.size(); i++)
					defaultShader.colors.push_back(BOUNDING_BOX_COLOR);

				defaultShader.program.setMat4("model", glm::mat4(1));
				glDrawArrays(GL_LINES, 0, defaultShader.coords.size());
			}
		}
	} // namespace render

} // namespace systems
