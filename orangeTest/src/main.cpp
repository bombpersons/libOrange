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
#include <Orange/graphics/TextureAtlas.hpp>
#include <Orange/graphics/FrameBuffer.hpp>
#include <Orange/maths/Maths.hpp>
#include <Orange/graphics/SpriteBatch.hpp>

#include <Orange/util/Filesystem.hpp>
#include <Orange/util/FPSDebugCamera.hpp>

#include <GL/gl.h>

#include <cmath>


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
	"in vec3 vertex_position;\n"
	"in vec2 vertex_texcoord;\n"

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
	texture.LoadFromFile("data/1.jpg");
  Texture texture2;
  texture2.LoadFromFile("data/2.jpg");

	TextureAtlas textureAtlas;
	for (int i = 0; i < 5; ++i) {
		filesystem::IterateFiles("data", [&textureAtlas](const char* _path) -> bool {
			textureAtlas.Insert(_path, _path);
			return true;
		});
	}
	textureAtlas.Build();

  // Create a spriteBatch
  SpriteBatch spriteBatch;

	// Make a timer for delta time.
	Timer timer;
	while (window.Update()) {
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

		// Draw to screen
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glViewport(0, 0, (float)window.GetWidth(), (float)window.GetHeight());
		glClearColor(0.8f, 0.6f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static int page = 0;
		if (window.Input()->IsKeyPressed(InputCode::Key::Right)) page++;
		if (window.Input()->IsKeyPressed(InputCode::Key::Left)) page--;
		if (page < 0) page = 0;
		if (page >= textureAtlas.GetPageCount()) page = textureAtlas.GetPageCount() - 1;

	  textureAtlas.GetPage(page)->Bind();
		shader.Bind();
		shader.SetUniform("tex", 0);
		mesh.Draw();

    // Draw some sprites
    glm::vec2 pos(100.0, 100.0);// = window.Input()->GetMousePos();

    static float rotate = 0;
    //rotate = fmod(rotate + M_PI * delta, 2 * M_PI);

    spriteBatch.SetProjection(glm::ortho(0.0f, (float)window.GetWidth(), (float)window.GetHeight(), 0.0f));
    spriteBatch.SetView(glm::mat4());
		//spriteBatch.Draw(textureAtlas.GetPage(0), pos, glm::vec2(0.2, 0.2), rotate, glm::vec2(0.0, 0.0), glm::vec2(0.0, 0.0), glm::vec2(0.5, 0.5));
    spriteBatch.Draw(textureAtlas.Get("data/2.jpg"), pos, glm::vec2(0.2, 0.2), rotate);
    spriteBatch.Flush();

		// Flip the display
		window.Display();
	}

	return 0;
}
