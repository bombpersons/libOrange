#include <Orange/graphics/Texture.hpp>
#include <Orange/priv/util/stb_image.h>
#include <Orange/logging/Logging.hpp>

namespace orange {
  Texture::Texture() {
    // Generate a texture
    texture = 0;
    glGenTextures(1, &texture);
  }

  Texture::~Texture() {
    if (texture)
      glDeleteTextures(1, &texture);
  }

  bool Texture::Create(int _width, int _height) {
    return LoadFromMemory(nullptr, _width, _height);
  }

  bool Texture::LoadFromMemory(const unsigned char* _buf, int _width, int _height) {
    if (!texture)
      return false;

    // Read the data into opengl
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _buf);
    GenMipMaps();

    // Set the width and height
    width = _width;
    height = _height;

    return true;
  }

  bool Texture::LoadFromFile(const char* _buf) {
    if (!_buf)
      return false;

    LOG(Log::DEFAULT) << "Loading image at " << _buf;

    // Attempt to open the file and load the image data
    int comp;
    unsigned char* data = stbi_load(_buf, &width, &height, &comp, 4);
    if (!data) {
      LOG(Log::CRITICAL) << "Couldn't load image at " << _buf;
      return false;
    }

    // Use the load from memory function
    bool ret = LoadFromMemory(data, width, height);
    if (ret) {
      LOG(Log::DEFAULT) << "Succesfully uploaded texture!";
    }

    // Free the memory stbi made
    stbi_image_free(data);

    return ret;
  }

  void Texture::SetWrap(WrapAxis::Type _axis, WrapMode::Type _wrapMode) {
    if (!texture)
      return;

    Bind();
    glTexParameteri(GL_TEXTURE_2D, (GLenum)_axis, (GLenum)_wrapMode);
  }

  void Texture::SetMagFilter(FilterMode::Type _filter) {
    if (!texture)
      return;

    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)_filter);
  }

  void Texture::SetMinFilter(FilterMode::Type _filter) {
    if (!texture)
      return;

    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)_filter);
  }

  void Texture::Bind(int _num) const {
    if (!texture)
      return;
    if (_num > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
      return;

    glActiveTexture(GL_TEXTURE0 + _num);
    glBindTexture(GL_TEXTURE_2D, texture);
  }

  void Texture::GenMipMaps() {
    Bind();
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  GLuint Texture::GetId() {
    return texture;
  }
}
