#ifndef ORANGE_SPRITEBATCH
#define ORANGE_SPRITEBATCH

#include <Orange/gl/GLContext.hpp>
#include <Orange/gl/GLResource.hpp>
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
    SpriteBatch();
    ~SpriteBatch();

    // Draw something
    void Draw(const Texture& _texture, glm::vec2 _pos, glm::vec2 _scale, float _rot, glm::vec2 _origin, glm::vec2 _origin, float _rot, glm::vec2 _uvTopLeft, glm::vec2 _uvBottomRight);

  private:

  };
}

#endif