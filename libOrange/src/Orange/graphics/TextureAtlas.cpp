#include <Orange/graphics/TextureAtlas.hpp>

namespace orange {
  TextureAtlas::TextureAtlas(int _pageWidth, int _pageHeight) {
    pageWidth = _pageWidth;
    pageHeight = _pageHeight;
  }

  TextureAtlas::~TextureAtlas() {
  }

  // Inserting
  void TextureAtlas::Insert(const std::string& _name, const std::string& _filename) {
    
  }

  void TextureAtlas::Insert(const std::string& _name, int _width, int _height, int _depth, unsigned char* _data) {

  }

  // Get a block
  bool TextureAtlas::Get(const std::string& _name, Block& _block) {

  }
}
