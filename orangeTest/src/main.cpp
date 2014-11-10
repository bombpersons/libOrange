#include <iostream>
#include <sstream>

#include <Orange/logging/Logging.hpp>
#include <Orange/logging/ConsoleLogListener.hpp>
#include <Orange/window/Window.hpp>
#include <Orange/input/Input.hpp>
#include <Orange/timing/Timer.hpp>
#include <Orange/graphics/Shader.hpp>
#include <Orange/graphics/Mesh.hpp>

#include <GL/gl.h>

#include <cmath>

#include <glm/glm.hpp>

using namespace orange;
int main(int _argc, char** _argv) {
	// Add log listeners.
	Log::AddListener("Console", new ConsoleLogListener());

	// Create a window
	Window window(400, 400, 32, false);
	window.Run();

	// Create a mesh
	glm::vec3 positions[] {
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(-0.0f, -0.5f, 0.0f)
	};

	glm::vec3 colors[] {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

	//LOG(Log::WARNING) << glGetError();

	Mesh mesh;
	mesh.SetBuffer(0, (float*)positions, 3, 3);
	mesh.SetBuffer(1, (float*)colors, 3, 3);
	mesh.Finish();

	// Create a simple shader.
	const char* fragment =
		"in vec3 color;\n"
		"out vec4 frag_color;\n"
		"void main() {\n"
		"	frag_color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	const char* vertex =
		"layout(location = 0) in vec3 vertex_position;\n"
		"layout(location = 1) in vec3 vertex_color;\n"

		"out vec3 color;\n"
		"void main() {\n"
		"	color = vertex_color;\n"
		"	gl_Position = vec4(vertex_position, 1.0);\n"
		"}\n";

	//LOG(Log::WARNING) << glGetError();

	Shader shader;
	shader.SetShaderSource(Shader::ShaderType::Vertex, vertex);
	shader.SetShaderSource(Shader::ShaderType::Fragment, fragment);
	shader.Compile();
	shader.SetAttribute("vertex_color", 1);
	shader.SetAttribute("vertex_position", 0);
	shader.Link();

	LOG(Log::WARNING) << glGetError();

	// Make a timer for delta time.
	Timer timer;
	while (window.Update()) {
		glViewport(0, 0, 100, 100);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double delta = timer.Reset();
		std::stringstream ss;
		ss << 1.0 / delta;
		window.SetTitle(ss.str().c_str());

		// Draw
		shader.Bind();
		//mesh.Draw();

		// Flip the display
		window.Display();
	}

	return 0;
}
