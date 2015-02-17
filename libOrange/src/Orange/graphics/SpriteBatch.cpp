#include <Orange/graphics/SpriteBatch.hpp>
namespace orange {
  // Constructors
  SpriteBatch::SpriteBatch(int _maxBatch, int _maxTextures) {
    // How many sprites to store before flushing.
    maxBatch = _maxBatch;
    spriteCount = 0;

    // How different textures to store before flushing
    maxTextures = _maxTextures;
    textures = new Texture*[maxTextures];
    memset(textures, 0, maxTextures * sizeof(Texture*));

    // Create the memory for the list
    sprites = new SpriteInfo[maxBatch];

    // Compile the shaders if they aren't already.
    CompileShaders();
  }

  SpriteBatch::~SpriteBatch() {
    if (sprites) {
      delete [] sprites;
      sprites = nullptr;
    }
  }

  // Draw
  void SpriteBatch::Draw(const Texture& _texture, glm::vec2 _pos, glm::vec2 _scale, float _rot, glm::vec2 _origin, glm::vec2 _uvTopLeft, glm::vec2 _uvBottomRight) {
    // Check whether or not we already have this texture
    int textureId = -1;
    for (int i = 0; i < maxTextures; ++i) {
      if (textures[i] == &_texture) {
        textureId = i;
      }
    }

    // Do we have a free slot?
    if (textureCount >= maxTextures) {
      // We don't have a free slot, flush, then continue;
      Flush();
    }

    // Find a free slot.
    if (textureId < 0) {
      textures[textureCount] = &_texture;
      textureCount++;
    }

    // Insert a sprite
    sprites[spriteCount].texture = textureId;
    sprites[spriteCount].position = _pos;
    sprites[spriteCount].scale = _scale;
    sprites[spriteCount].rotation = _rot;
    sprites[spriteCount].origin = _origin;
    sprites[spriteCount].uvTop = _uvTopLeft;
    sprites[spriteCount].uvBottom = _uvBottomRight;
    spriteCount++;
  }

  // Flush
  void SpriteBatch::Flush() {
    // Ensure a context
    GLContext::EnsureContext();

    // Draw all of the stuff we've cached.

  }

  // Compile the shaders
  void SpriteBatch::CompileShaders() {
    // Fragment shader.
    const char* fragment =
      "#version 400\n"
      "in vec2 texcoord;\n"
      "out vec4 frag_color;\n"

      "uniform sampler2D tex;\n"

      "void main() {\n"
      "	frag_color = texture2D(tex, texcoord);\n"
      "}\n";

    // Vertex shader
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

    const char* geometry =
      "#version 400\n"
      "layout (points) in;\n"
      "layout (triangle_strip) out;\n"
      "layout (max_vertices = 4) out;\n"

      uniform mat4 projection;
      uniform mat4 view;

      out vec2 texcoord;

      void main() {

      }

      void main()
      {

      }
      ;


  }
}