#ifndef ORANGE_TEXTUREATLAS
#define ORANGE_TEXTUREATLAS

#include <Orange/gl/GLContext.hpp>
#include <Orange/gl/GLResource.hpp>
#include <Orange/maths/Maths.hpp>
#include <Orange/graphics/Texture.hpp>

#include <vector>
#include <map>

namespace orange {
  class TextureAtlas : public GLResource {
  public:
    struct BlockToBuild {
      std::string name;
      int width, height;
      unsigned char* data = nullptr;
      bool packed = false;
    };

  public:
    TextureAtlas(int _pageWidth=2048, int _pageHeight=2048, int _padding=1);
    virtual ~TextureAtlas();

    void Insert(const std::string& _name, const std::string& _filename);
    void Insert(const std::string& _name, int _width, int _height, unsigned char* _data);

    void Build();

    int GetPageCount();
    Texture* GetPage(int id);
    const TexturePortion* Get(const std::string& _name);

  private:
    // Width and height of the pages
    int pageWidth, pageHeight;
    // Padding between images
    int padding;

    // A list of stuff to build into the texture atlas.
    std::vector<BlockToBuild> blocksToBuild;

    // A map of names to block definitions.
    // This is filled out when build is called.
    std::map<std::string, TexturePortion> blocks;

    // Atlas pages.
    std::vector<Texture*> pages;
  };
}

#endif
