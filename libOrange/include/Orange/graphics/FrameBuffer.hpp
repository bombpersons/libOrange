#ifndef ORANGE_FRAMEBUFFER
#define ORANGE_FRAMEBUFFER

#include <Orange/gl/GLResource.hpp>
#include <Orange/gl/GLContext.hpp>
#include <Orange/maths/Maths.hpp>
#include <Orange/graphics/Texture.hpp>

namespace orange {
  class FrameBuffer : public GLResource {
  public:
    FrameBuffer(int _width, int _height);
    ~FrameBuffer();

    // Add a texture
    bool AddColor(int _index);
    bool AddDepthStencilBuffer();

    // Bind ourselves.
    void Bind();
    void UnBind();

    // Get the color texture
    Texture* GetColor(int _index);

  private:
    // The frame buffer object
    GLuint framebuffer;
    GLint prevFramebuffer;

    // Color texture
    Texture* textures[GL_MAX_COLOR_ATTACHMENTS] = {};

    // The renderbuffer for depth and stencil
    GLuint depthstencilbuffer;

    // The width and height of the framebuffer
    int width, height;
  };
}

#endif