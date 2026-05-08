#include "graphics/core/event.hpp"
#include "graphics/core/window.hpp"

using namespace ogl;

#define BIG

const auto ed = EventManager::instance();

int main(int argc, char *argv[]) {
	WindowSettings s{};
	s.position = {40, 40};
	s.decorated = false;
	s.vsync = true;
	s.size = {600, 600};
	s.bgColor = {0};
	Window w{s};
	w.onAttach();

	// add window function to execute every frame
	ed->subscribe(event::loop::LOOP_UPDATE, [&w]() { w.onUpdate(); });
	ed->subscribe(event::loop::LOOP_BEGIN_RENDER, [&w]() { w.begin(); });
	ed->subscribe(event::loop::LOOP_RENDER, [&w]() { w.onRender(); });

	while (!glfwWindowShouldClose(w.getContext())) {
		// for each event it executes all the registered callbacks
		ed->post(event::loop::LOOP_INPUT);
		ed->post(event::loop::LOOP_UPDATE);
		ed->post(event::loop::LOOP_BEGIN_RENDER);
		ed->post(event::loop::LOOP_RENDER);
		ed->post(event::loop::LOOP_END_RENDER);
	}

	// clear all window data
	w.onDetach();
}
