#ifndef ORANGE_SPRITEBATCH
#define ORANGE_SPRITEBATCH

#include <Orange/gl/GLContext.hpp>
#include <Orange/gl/GLResource.hpp>
#include <Orange/graphics/Shader.hpp>
#include <Orange/graphics/Texture.hpp>
#include <Orange/maths/Maths.hpp>

namespace orange {
  class SpriteBatch : public GLResource {
  public:
    struct SpriteInfo {
      glm::vec2 position;
      glm::vec2 origin;
      float rotation;
      glm::vec2 scale;
      glm::vec2 uvTop, uvBottom;
      unsigned char texture;
    };
  public:
    SpriteBatch(int _maxbatch=512, int _maxTextures=1);
    ~SpriteBatch();

    // Draw something
    void Draw(const Texture& _texture, glm::vec2 _pos, glm::vec2 _scale, float _rot, glm::vec2 _origin, glm::vec2 _uvTopLeft, glm::vec2 _uvBottomRight);

    // Flush
    void Flush();

  private:
    // Compile shaders
    static Shader StaticShader;
    static void CompileShaders();

  private:
    // A list of things to draw
    int maxBatch;
    int spriteCount;
    SpriteInfo* sprites;

    // List of textures
    int maxTextures;
    int textureCount;
    const Texture** textures; // OpenGL 3.0 spec says that there needs to be at least 16 different texture units for a shader.
  };
}

#endif