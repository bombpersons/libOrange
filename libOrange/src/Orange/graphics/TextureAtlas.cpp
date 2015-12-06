#include <Orange/graphics/TextureAtlas.hpp>
#include <Orange/logging/Logging.hpp>

#include <cstring>

#define STB_RECT_PACK_IMPLEMENTATION
#include <Orange/priv/util/stb_rect_pack.h>

#include <Orange/priv/util/stb_image.h>

namespace orange {
  TextureAtlas::TextureAtlas(int _pageWidth, int _pageHeight, int _padding) {
    pageWidth = _pageWidth;
    pageHeight = _pageHeight;
    padding = _padding;
  }

  TextureAtlas::~TextureAtlas() {
    for (auto it = pages.begin(); it != pages.end(); ++it) {
      delete *it;
    }
  }

  // Inserting
  void TextureAtlas::Insert(const std::string& _name, const std::string& _filename) {
    // Load the texture data from the file.
    LOG(Log::DEFAULT) << "Loading image at " << _filename;

    int comp;
    int width, height;
    unsigned char* data = stbi_load(_filename.c_str(), &width, &height, &comp, 4);
    if (!data) {
      LOG(Log::CRITICAL) << "Couldn't load image at " << _filename;
      return;
    }

    Insert(_name, width, height, data);

    // Free the memory
    stbi_image_free(data);
  }

  void TextureAtlas::Insert(const std::string& _name, int _width, int _height, unsigned char* _data) {
    // If the image is larger than a single page, then we shouldn't add this.
    if (_width > pageWidth || _height > pageHeight) {
      LOG(Log::WARNING) << "Couldn't add image " << _name << " (" << _width << "x" << _height << ")"
                        << " because it is larger than the page size (" << pageWidth << "x" << pageHeight << ")";
      return;
    }

    BlockToBuild block;
    block.name = _name;
    block.width = _width;
    block.height = _height;

    // Copy the data so that it isn't deleted..
    block.data = new unsigned char[_width * _height * 4];
    memcpy(block.data, _data, sizeof(unsigned char) * _width * _height * 4);

    blocksToBuild.push_back(block);
  }

  void TextureAtlas::Build() {
    // Pack the rects using stb_rect_pack

    // Keep making new pages until we fit everything.
    int page = 0;
    int blocksProcessed = 0;
    while (blocksProcessed < blocksToBuild.size()) {
      // Make sure we have textures for each page.
      while (pages.size() <= page) {
        Texture* texture = new Texture();
        pages.push_back(texture);
      }

      // Allocate memory for this page..
      unsigned char* data = new unsigned char[pageWidth * pageHeight * 4];
      memset(data, 0, sizeof(unsigned char) * pageWidth * pageHeight * 4);

      // Create an array of rects to pack.
      int rectCount = blocksToBuild.size();
      stbrp_rect* rects = new stbrp_rect[rectCount];
      memset(rects, 0, sizeof(stbrp_rect) * rectCount);

      // Copy our rects details into it.
      for (int i = 0; i < blocksToBuild.size(); ++i) {
        if (!blocksToBuild[i].packed) {
          rects[i].id = i; // Keep track of which image this is.
          rects[i].w = blocksToBuild[i].width;
          rects[i].h = blocksToBuild[i].height;
        }
      }

      // Create a rectangle packer context
      stbrp_context context;
      stbrp_node* nodes = new stbrp_node[pageWidth];
      stbrp_init_target(&context, pageWidth, pageHeight, nodes, pageWidth);

      // Pack the rectangles.
      stbrp_pack_rects(&context, rects, blocksToBuild.size());

      // Keep track of the blocks that we processed.
      std::vector<int> processedBlocks;

      // Find all the rects that were packed.
      for (int i = 0; i < rectCount; ++i) {
        if (rects[i].was_packed) {
          // Get the associated block
          BlockToBuild& blockToBuild = blocksToBuild[rects[i].id];

          // Add the block to our list.
          TexturePortion block;
          block.texture = pages[page];
          block.topLeft.x = (float)rects[i].x / (float)pageWidth;
          block.topLeft.y = (float)rects[i].y / (float)pageHeight;
          block.bottomRight.x = (float)(rects[i].x + rects[i].w) / (float)pageWidth;
          block.bottomRight.y = (float)(rects[i].y + rects[i].h) / (float)pageHeight;
          blocks[blockToBuild.name] = block;

          auto Blit = [](unsigned char* _dest, int _destWidth, int _destHeight, unsigned char* _source, int _x, int _y, int _width, int _height) {
            // Blit the image to our texture data...
            for (int x = 0; x < _width; ++x) {
              for (int y = 0; y < _height; ++y) {
                // Ugly.. but uh.. mh..
                int destX = x + _x;
                int destY = y + _y;

                for (int offset = 0; offset <= 4; ++offset) {
                  _dest[((destX + (destY * _destWidth)) * 4) + offset] = _source[((x + (y * _width)) * 4) + offset];
                }
              }
            }
          };
          Blit(data, pageWidth, pageHeight, blockToBuild.data, rects[i].x, rects[i].y, rects[i].w, rects[i].h);

          blockToBuild.packed = true;
          blocksProcessed++;
        }
      }

      // Actually upload the texture.
      pages[page]->LoadFromMemory(data, pageWidth, pageHeight);

      // Free the memory we allocated.
      delete [] data;
      delete [] nodes;
      delete [] rects;

      // Increment the page
      page++;
    }

    // Destroy all of the blocks to build
    for (auto it = blocksToBuild.begin(); it != blocksToBuild.end(); ++it) {
      delete [] (*it).data;
    }
    blocksToBuild.clear();
  }

  int TextureAtlas::GetPageCount() {
    return pages.size();
  }

  // Get a page
  Texture* TextureAtlas::GetPage(int id) {
    if (id < 0 || id >= pages.size())
      return nullptr;
    return pages[id];
  }

  // Get a block
  const TexturePortion* TextureAtlas::Get(const std::string& _name) {
    if (blocks.find(_name) != blocks.end()) {
      return &blocks[_name];
    }
    return nullptr;
  }
}
