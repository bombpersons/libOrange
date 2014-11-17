#include <iostream>
#include <sstream>

#include <Orange/logging/Logging.hpp>
#include <Orange/logging/ConsoleLogListener.hpp>
#include <Orange/window/Window.hpp>
#include <Orange/input/Input.hpp>
#include <Orange/timing/Timer.hpp>
#include <Orange/graphics/Shader.hpp>
#include <Orange/graphics/Mesh.hpp>
#include <Orange/maths/Maths.hpp>
#include <Orange/graphics/Texture.hpp>
#include <Orange/graphics/FrameBuffer.hpp>

#include <Orange/util/FPSDebugCamera.hpp>

#include <GL/gl.h>

#include <cmath>

#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

using namespace orange;
int main(int _argc, char** _argv) {
	// Add log listeners.
	Log::AddListener("Console", new ConsoleLogListener());

	// Create a window
	Window window(400, 400, 32, false);
	window.Run();

	// Create a mesh
  const Mesh& mesh = Mesh::Cube();

	// Create a simple shader.
  const char* fragment =
    "#version 400\n"
    "in vec2 texcoord;\n"
    "out vec4 frag_color;\n"

    "uniform sampler2D tex;\n"

    "void main() {\n"
    "	frag_color = texture2D(tex, texcoord);\n"
		"}\n";

  const char* vertex =
    "#version 400\n"
    "layout(location = 0) in vec3 vertex_position;\n"
    "layout(location = 1) in vec2 vertex_texcoord;\n"

    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"

    "out vec3 color;\n"
    "out vec2 texcoord;\n"
    "void main() {\n"
    " texcoord = vertex_texcoord;\n"
		"	gl_Position = projection * view * vec4(vertex_position, 1.0);\n"
		"}\n";

	Shader shader;
	shader.SetShaderSource(Shader::ShaderType::Vertex, vertex);
	shader.SetShaderSource(Shader::ShaderType::Fragment, fragment);
	shader.Compile();
  shader.SetAttribute("vertex_position", 0);
  shader.SetAttribute("vertex_texcoord", 1);
	shader.Link();

	// Create a camera
	FPSDebugCamera camera;
  camera.SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));

  // Load a texture
  Texture texture;
  texture.LoadFromFile("test.jpg");

  // Create a framebuffer
  //FrameBuffer framebuffer(512, 512);
  //framebuffer.Bind();
  //framebuffer.UnBind();

	// TODO:
	// MAKE A SPRITE BATCH CLASS THAT USES GEOMETRY SHADERS
	// BIND AS MANY TEXTURES AS POSSIBLE BEFORE FLUSHING

	// Make a timer for delta time.
	Timer timer;
	while (window.Update()) {
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);

		glViewport(0, 0, (float)window.GetWidth(), (float)window.GetHeight());
		glClearColor(0.8f, 0.6f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double delta = timer.Reset();
		std::stringstream ss;
		ss << 1.0 / delta;
		window.SetTitle(ss.str().c_str());

		// Create a projection matrix
		glm::mat4 projection = glm::perspectiveFov(M_PI / 4.0f, (float)window.GetWidth(), (float)window.GetHeight(), 0.01f, 1000.0f);
		shader.SetUniform("projection", projection);

		// Update the camera
		camera.Update(delta, &window);
		shader.SetUniform("view", camera.GetCameraMatrix());

		// Draw
    texture.Bind();
		shader.Bind();
    shader.SetUniform("tex", 0);
		mesh.Draw();

		// Flip the display
		window.Display();
	}

	return 0;
}
