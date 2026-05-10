#include "graphics/core/event.hpp"
#include "graphics/core/imgui.hpp"
#include "graphics/core/window.hpp"

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
