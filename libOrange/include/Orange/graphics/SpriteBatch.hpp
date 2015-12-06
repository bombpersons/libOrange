#ifndef ORANGE_SPRITEBATCH
#define ORANGE_SPRITEBATCH

#include <Orange/gl/GLContext.hpp>
#include <Orange/gl/GLResource.hpp>
#include <Orange/graphics/Shader.hpp>
#include <Orange/graphics/Texture.hpp>
#include <Orange/maths/Maths.hpp>
#include <Orange/graphics/Mesh.hpp>

namespace orange {
  class SpriteBatch : public GLResource {
  public:
  public:
    SpriteBatch(int _maxbatch=512);
    ~SpriteBatch();

    // Set the matrices to use
    void SetProjection(const glm::mat4& _mat);
    void SetView(const glm::mat4& _mat);

    // Draw something
    void Draw(Texture* _texture, glm::vec2 _pos, glm::vec2 _scale, float _rot, glm::vec2 _origin = glm::vec2(0.0, 0.0), glm::vec2 _uvTopLeft = glm::vec2(0.0, 0.0), glm::vec2 _uvBottomRight = glm::vec2(1.0, 1.0));
    void Draw(const TexturePortion*, glm::vec2 _pos, glm::vec2 _scale, float _rot, glm::vec2 _origin = glm::vec2(0.0, 0.0));

    // Flush
    void Flush();

  private:
    // Compile shaders
    static Shader* GetShader();

  private:
    struct SpritePoint {
      glm::vec2 position;
      glm::vec2 origin;
      float rotation;
      glm::vec2 scale;

      glm::vec2 uvTop;
      glm::vec2 uvBottom;
    };

    SpritePoint* spriteData;
    unsigned int spriteDataTotalCount;
    unsigned int spriteDataCount;
    Texture* texture;

    // The mesh
    Mesh mesh;
  };
}

#endif
