#ifndef ORANGE_MESH
#define ORANGE_MESH

#include <GL/glew.h>
#include <GL/gl.h>
#include <Orange/gl/GLResource.hpp>

namespace orange {
  #define ORANGE_MESH_MAX_VBOS 16
  class Mesh : public GLResource {
  public:
  public:
    Mesh();
    ~Mesh();

    // Set the mesh as a cube
    // Set's positions and UV coords
    static const Mesh& Cube();

    // Set data in a buffer
    bool SetBuffer(int _index, unsigned char* _data, unsigned int _componentsize, unsigned int _componentcount, unsigned int _totalsize, GLenum _type);
    bool SetBufferIndices(unsigned int* _data, unsigned int _totalsize);

    // Set draw mode
    void SetDrawMode(GLenum _drawmode);

    // Write the vbo into a vao
    bool Finish();

    // Draw the mesh
    void Draw() const;

  private:
    // Info for one buffer
    struct BufferInfo {
      char* data;
      unsigned int componentcount;
      unsigned int componentsize;
      GLenum type;
    };
  private:
    // How many elements
    unsigned int elementcount;

    // buffer info
    BufferInfo bufferInfo[ORANGE_MESH_MAX_VBOS] = {};

    // VBO
    GLuint vbo;

    // Indices
    GLuint ibo;

    // The vao
    GLuint vao;

    // The draw mode
    GLenum drawmode;
  };
}

#endif
