#include <iostream>
#include <sstream>

#include <Orange/logging/Logging.hpp>
#include <Orange/logging/ConsoleLogListener.hpp>
#include <Orange/window/Window.hpp>

#include <Orange/input/Input.hpp>

#include <Orange/timing/Timer.hpp>

#include <GL/gl.h>

#include <cmath>

using namespace orange;
int main(int _argc, char** _argv) {
	// Add log listeners.
	Log::AddListener("Console", new ConsoleLogListener());

	// Create a window
	Window window(400, 400, 32, false);
	window.Run();

	Timer timer;
	double red = 0;
	double green = 0;
	double blue = 0;
	while (window.Update()) {
		glViewport(0, 0, 100, 100);
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double delta = timer.Reset();
		std::stringstream ss;
		ss << 1.0 / delta;
		window.SetTitle(ss.str().c_str());

		double remainder = 0.0f;
		red = modf(red + (window.Input()->IsKeyDown(InputCode::Key::Left) ? 1.0f : 0.0f) * delta, &remainder);
		green = modf(green + (window.Input()->IsKeyDown(InputCode::Key::Up) ? 1.0f : 0.0f) * delta, &remainder);
		blue = modf(blue + (window.Input()->IsKeyDown(InputCode::Key::Right) ? 1.0f : 0.0f) * delta, &remainder);

		window.Display();
	}

	return 0;
}
