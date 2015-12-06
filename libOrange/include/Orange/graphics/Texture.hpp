#ifndef ORANGE_TEXTURE
#define ORANGE_TEXTURE

#include <Orange/gl/GLContext.hpp>
#include <Orange/gl/GLResource.hpp>
#include <Orange/maths/Maths.hpp>

namespace orange {
  class Texture : public GLResource {
  public:
    struct WrapAxis {
      enum Type {
        S = GL_TEXTURE_WRAP_S,
        T = GL_TEXTURE_WRAP_T
      };
    };
    struct WrapMode {
      enum Type {
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        Repeat = GL_REPEAT,
        MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
      };
    };
    struct FilterMode {
      enum Type {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
        NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
        LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
      };
    };
  public:
    Texture();
    ~Texture();

    // Create a blank texture
    bool Create(int _width, int _height);

    // Load the texture from memory or file
    bool LoadFromMemory(const unsigned char* _buf, int _width, int _height);
    bool LoadFromFile(const char* _path);

    // Set the wrapping mode
    void SetWrap(WrapAxis::Type _axis, WrapMode::Type _wrapMode);

    // Set magnify mode
    void SetMagFilter(FilterMode::Type _filter);
    void SetMinFilter(FilterMode::Type _filter);

    // Bind the texture
    void Bind(int _num=0) const;

    // Generate mip maps
    void GenMipMaps();

    // Get the texture id
    GLuint GetId();

    // Get the size of the texture
    glm::ivec2 GetDimensions() { return glm::ivec2(width, height); }
    int GetWidth() { return width; }
    int GetHeight() { return height; }

  private:
    // The texture id
    GLuint texture;

    // Width and height of the texture
    int width, height;
  };
  struct TexturePortion {
    Texture* texture;
    glm::vec2 topLeft;
    glm::vec2 bottomRight;
  };
}

#endif
