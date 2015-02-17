#include <Orange/graphics/FrameBuffer.hpp>
#include <Orange/logging/Logging.hpp>

namespace orange {
  FrameBuffer::FrameBuffer(int _width, int _height) {
    GLContext::EnsureContext();
    
    // Set defaults
    depthstencilbuffer = 0;

    // Record the width / height
    width = _width;
    height = _height;

    // Create the framebuffer
    framebuffer = 0;
    glGenFramebuffers(1, &framebuffer);

    // Add a color, depth and stencil by default
    AddColor(0);
    AddDepthStencilBuffer();
  }

  FrameBuffer::~FrameBuffer() {
    GLContext::EnsureContext();

    if (framebuffer)
      glDeleteFramebuffers(1, &framebuffer);
  }
  
  bool FrameBuffer::AddColor(int _index) {
    if (_index >= GL_MAX_COLOR_ATTACHMENTS)
      return false;
    if (_index < 0)
      return false;

    // Create a texture
    if (textures[_index])
      delete textures[_index];

    // Create a texture
    textures[_index] = new Texture();
    textures[_index]->Create(width, height);

    // Set this on the framebuffer
    GLint prev = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index, GL_TEXTURE_2D, textures[_index]->GetId(), 0
    );
    glBindFramebuffer(GL_FRAMEBUFFER, prev);
  }

  bool FrameBuffer::AddDepthStencilBuffer() {
    if (depthstencilbuffer)
      return true;

    // Bind the framebuffer
    GLint prev = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create the render buffer
    glGenRenderbuffers(1, &depthstencilbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthstencilbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthstencilbuffer
    );

    glBindFramebuffer(GL_FRAMEBUFFER, prev);

    return true;
  }

  void FrameBuffer::Bind() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);

    // Check the status of the frame buffer
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
      LOG(Log::CRITICAL) << "Tried to bind incomplete frame buffer! Status: " << status;
    }
  }

  void FrameBuffer::UnBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);

    // Generate mip maps for the colors
    for (int i = 0; i < GL_MAX_COLOR_ATTACHMENTS; ++i) {
      if (textures[i])
        textures[i]->GenMipMaps();
    }
  }

  // Get the color texture
  Texture* FrameBuffer::GetColor(int _index) {
    if (_index >= GL_MAX_COLOR_ATTACHMENTS)
      return nullptr;
    if (_index < 0)
      return nullptr;

    return textures[_index];
  }
}