#ifndef ORANGE_TEXTUREATLAS
#define ORANGE_TEXTUREATLAS

#include <Orange/gl/GLContext.hpp>
#include <Orange/gl/GLResource.hpp>
#include <Orange/maths/Maths.hpp>
#include <Orange/gl/Texture.hpp>

#include <map>

namespace orange {
  class TextureAtlas : public GLResource {
  public:
    struct Block {
      Texture* page;
      float x, y, width, height;
    };

  public:
    TextureAtlas(int _pageWidth=2048, int _pageHeight=2048);
    virtual ~TextureAtlas();

    void Insert(const std::string& _name, const std::string& _filename);
    void Insert(const std::string& _name, int _width, int _height, int _depth, unsigned char* _data);

    bool Get(const std::string& _name, Block& _block);

  private:
    // Width and height of the pages
    int pageWidth, pageHeight;

    // A map of names to block definitions.
    std::map<std::string, Block>

    // Atlas pages.
    std::vector<Texture*> pages;
  };
}

#endif
