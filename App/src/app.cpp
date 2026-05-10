#include "graphics/buffer/uniform_buffer.hpp"
#include "graphics/core/event.hpp"
#include "graphics/core/imgui.hpp"
#include "graphics/core/window.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "graphics/shader/shader.hpp"

using namespace ogl;

const auto ed = EventManager::instance();

int main(int argc, char *argv[]) {
	WindowSettings s{};
	s.position = {40, 40};
	s.decorated = false;
	s.vsync = true;
	s.size = {600, 600};
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

	auto a = im.addPanel<ImGuiPanel>();
	// define the render function for the imgui panel
	a->setRenderFunc([]() {
		ImGui::Begin("Foo");
		ImGui::Text("Hello ImGui!!");
		ImGui::End();
	});

	auto b = im.addPanel<ImGuiPanel>();
	// define the render function for the imgui panel
	b->setRenderFunc([]() {
		ImGui::Begin("BBB");
		ImGui::Text("Other panel");
		ImGui::End();
	});

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
	ShaderProgram def{"vert_shader.glsl", "frag_shader.glsl"};
	def.createShaderProgram();

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
