#include "ecs/ecs_scene.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system.hpp"

#include "graphics/buffer/uniform_buffer.hpp"
#include "graphics/core/event.hpp"
#include "graphics/core/imgui.hpp"
#include "common/utils.hpp"
#include "graphics/core/window.hpp"
#include "graphics/shader/shader.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "factory.hpp"
#include "ui/app_gui.hpp"
#include "view/board.hpp"

using namespace ogl;

const auto ed = EventManager::instance();
const auto em = EntityManager::instance();
const auto ecs = BasicScene::instance();

int main(int argc, char *argv[]) {
	WindowSettings s{};
	s.position = {200, 100};
	s.decorated = false;
	s.vsync = true;
	s.size = {1600, 900};
	s.bgColor = {.4f, .4f, .4f, 1.f};
	Window w{s};
	w.onAttach();

	// add window function to execute every frame
	ed->subscribe(event::loop::LOOP_UPDATE, [&w]() { w.onUpdate(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&w]() { w.begin(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&w]() { w.onRender(); });

	ImGuiManager im{&w};
	im.onAttach();
	// add imgui operations to execute every frame in different phase
	ed->subscribe(event::loop::LOOP_UPDATE, [&im]() { im.onUpdate(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&im]() { im.onRender(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&im]() { im.begin(); });
	ed->subscribe(event::loop::LOOP_END_RENDER, [&im]() { im.end(); });

	im.addPanel<ImGuiEntityTree>();

	UniformBuffer ubo{"Matrices"};
	glm::mat4 proj = glm::ortho(0.f, w.getWidth(), 0.f, w.getHeight());
	ubo.onAttach();
	ubo.setup(sizeof(glm::mat4), 0, 0, 0);
	ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));

	ed->subscribe(event::shader::SHADER_PROJECTION_CHANGED, [&w, &ubo]() {
		auto proj = glm::ortho(0.f, w.getWidth(), 0.f, w.getHeight());
		ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
	});

	// shaders
	Shared<ShaderProgram> def = CreateShared<ShaderProgram>("vert_shader.glsl", "frag_shader.glsl");
	def->createShaderProgram();

	auto board = view::Board();   
    board.onAttach();
    ecs->addEntity(def, board.getId());

	ed->subscribe(event::loop::LOOP_RENDER, []() { systems::render::renderAllMeshes(); });

	while (!glfwWindowShouldClose(w.getContext())) {
		ed->post(event::loop::LOOP_INPUT);
		ed->post(event::loop::LOOP_UPDATE);
		ed->post(event::loop::LOOP_BEGIN_RENDER);
		ed->post(event::loop::LOOP_RENDER);
		ed->post(event::loop::LOOP_END_RENDER);
	}
	// clear all window data
	w.onDetach();
}
