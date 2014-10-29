#include <iostream>

#include <Orange/logging/Logging.hpp>
#include <Orange/logging/ConsoleLogListener.hpp>
#include <Orange/window/Window.hpp>

#include <Orange/input/Input.hpp>

#include <GL/gl.h>

using namespace orange;
int main(int _argc, char** _argv) {
	// Add log listeners.
	Log::AddListener("Console", new ConsoleLogListener());

	// Create a window
	Window window(400, 400, 32, false);
	window.Run();

	while (window.IsOpen()) {
		glViewport(0, 0, 100, 100);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		window.Display();
	}

	return 0;
}
