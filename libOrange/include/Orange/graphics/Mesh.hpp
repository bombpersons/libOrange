#ifndef ORANGE_MESH
#define ORANGE_MESH

#include <GL/glew.h>
#include <GL/gl.h>
#include <Orange/gl/GLResource.hpp>

namespace orange {
  #define ORANGE_MESH_MAX_VBOS 16
  class Mesh : public GLResource {
  public:
    struct DrawMode {
      enum Type {
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        LineLoop = GL_LINE_LOOP,
        Triangles = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleFan = GL_TRIANGLE_FAN
      };
    };
  public:
    Mesh();
    ~Mesh();

    // Set data in a buffer
    bool SetBuffer(int _index, float* _data, unsigned int _elementsize, unsigned int _totalsize);
    bool SetBufferIndices(unsigned int* _data, unsigned int _totalsize);

    // Set draw mode
    bool SetDrawMode(DrawMode::Type _drawmode);

    // Write the vbo into a vao
    bool Finish();

    // Draw the mesh
    void Draw();

  private:
    // Opengl vbos
    GLuint vbos[ORANGE_MESH_MAX_VBOS] = {};

    // Indices
    GLuint ibo;

    // Sizes
    unsigned int size;
    unsigned int elementsizes[ORANGE_MESH_MAX_VBOS] = {};

    // The vao
    GLuint vao;

    // The draw mode
    DrawMode::Type drawmode;
  };
}

#endif
