#include <GLFW/glfw3.h>
#include <string>
#include "common/logger.hpp"

#include "common/utils.hpp"
#include "ecs/ecs_scene.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system.hpp"

#include "engine/location.hpp"
#include "graphics/buffer/uniform_buffer.hpp"
#include "graphics/core/event.hpp"
#include "graphics/core/imgui.hpp"
#include "graphics/core/window.hpp"
#include "graphics/shader/shader.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "ui/app_gui.hpp"
#include "view/board.hpp"
#include "view/pawn.hpp"

using namespace ogl;

const auto ed = EventManager::instance();
const auto em = EntityManager::instance();
const auto ecs = BasicScene::instance();

int selected = -1;
int srcCell = -1;
int dstCell = -1;

int getCellUnderCursor(Pair<double> &pos) {
	for (auto cell : em->getEntitiesFromComponent<CellComponent>()) {
		auto coll = em->getComponentFromId<AABBCollider>(cell);
		if (coll->isColliding({pos.x, pos.y, 0})) {
			return cell;
		}
	}
	return -1;
}

void pressCell(Pair<double> &pos, int &button) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		srcCell = getCellUnderCursor(pos);
		// INFO("Clicked: " + std::to_string(selected));
	}
}

void releaseCell(Pair<double> &pos, int &button) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		dstCell = getCellUnderCursor(pos);
		// INFO("Released: " + std::to_string(selected));
	}
}

int main(int argc, char *argv[]) {
	INFO("Start application\n");

	// window layer
	INFO("INIT window");
	WindowSettings s{};
	s.position = {200, 100};
	s.decorated = false;
	s.vsync = true;
	s.size = {1600, 900};
	s.bgColor = {.4f, .4f, .4f, 1.f};
	Window window{s};
	INFO("--- attaching window");
	window.onAttach();
	INFO("--- window attached");

	INFO("--- subscribing window events");
	// add window function to execute every frame
	ed->subscribe(event::loop::LOOP_UPDATE, [&window]() { window.onUpdate(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&window]() { window.begin(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&window]() { window.onRender(); });

	INFO("INIT window complete\n");

	// imgui manager layer
	INFO("INIT imgui");
	ImGuiManager im{&window};
	INFO("--- attaching imgui manager");
	im.onAttach();
	INFO("--- imgui manager attached");

	INFO("--- subscribing imgui events");
	// add imgui operations to execute every frame in different phase
	ed->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });
	INFO("--- subscribing events done");

	INFO("--- add ImGuiEntityTree panel");
	im.addPanel<ImGuiEntityTree>();

	INFO("INIT imgui completed\n");

	// Uniform buffers
	INFO("INIT unfiform buffer");
	UniformBuffer ubo{"Matrices"};
	glm::mat4 proj = glm::ortho(0.f, window.getWidth(), 0.f, window.getHeight());
	INFO("--- attaching uniform buffer");
	ubo.onAttach();
	INFO("--- uniform buffer attached");
	INFO("--- setup uniform buffer");
	ubo.setup(sizeof(glm::mat4), 0, 0, 0);
	ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	INFO("--- uniform buffer filled");

	INFO("--- subscribing uniform buffer events");
	ed->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [&window, &ubo]() {
		auto proj = glm::ortho(0.f, window.getWidth(), 0.f, window.getHeight());
		ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	});

	INFO("INIT unfiform buffer completed\n");

	// shaders
	Shared<ShaderProgram> def = CreateShared<ShaderProgram>("vert_shader.glsl", "frag_shader.glsl");
	def->createShaderProgram();

	auto board = view::Board();
	board.onAttach();
	ecs->addEntity(def, board.getId());

	auto sample = view::Pawn();
	sample.onAttach();
	ecs->addEntity(def, sample.getId());

	{
		auto ett = board.getCellFromCoord('A', 3);
		if (ett > 0) {
			auto pos = systems::transform::getPosition(ett);
			pos.z = 1;
			systems::transform::updatePosition(sample.getId(), pos);
		}
	}

	// mouse callback
	window.setMouseButtonCallback(
		[&window, &board, &sample](GLFWwindow *ctx, int button, int action, int mods) {
			auto pos = window.getMousePos();
			switch (action) {
				case GLFW_PRESS: {
					pressCell(pos, button);
					break;
				}
				case GLFW_RELEASE: {
					releaseCell(pos, button);
					if (srcCell > 0 && dstCell > 0) {
						// move piece
						// coords for the engine (view don't need it, could use ecs id - dstCell)
						auto coords = board.getCoordFromCell(dstCell);
                        // update the board model
						auto boardLocation = chess::Location(coords.x, coords.y);
						INFO(boardLocation.toString());
                        // update the view -> src to dst
                        auto pos = systems::transform::getPosition(dstCell);
                        pos.z = 1;
                        systems::transform::updatePosition(sample.getId(), pos);
					}
					break;
				}
			}
		});

	ed->subscribe(event::loop::LOOP_RENDER, []() { systems::render::renderAllMeshes(); });

	while (!glfwWindowShouldClose(window.getContext())) {
		ed->post(event::loop::LOOP_INPUT);
		ed->post(event::loop::LOOP_UPDATE);
		ed->post(event::loop::LOOP_BEGIN_RENDER);
		ed->post(event::loop::LOOP_RENDER);
		ed->post(event::loop::LOOP_END_RENDER);
	}
	// clear all window data
	window.onDetach();
}
