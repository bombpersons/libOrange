#ifndef ORANGE_MESH
#define ORANGE_MESH

#include <GL/glew.h>
#include <GL/gl.h>
#include <Orange/gl/GLResource.hpp>

namespace orange {
  class Mesh : public GLResource {
  public:
  public:
    Mesh();
    ~Mesh();

    // Set the mesh as a cube
    // Set's positions and UV coords
    static const Mesh& Cube();
	
	bool SetData(void* _data, unsigned int _totalSize);
	bool SetBuffer(unsigned int _id, unsigned int _elementSize, GLenum _type, unsigned int _stride, unsigned int _offset);
	bool SetIndices(unsigned int* _data, unsigned int _count);
	void SetVertexCount(unsigned int _count) { elementCount = _count; }

	void SetDrawMode(GLenum _drawmode);

    // Draw the mesh
    void Draw() const;

  private:
    // VBO
    GLuint vbo;

    // Indices
    GLuint ibo;

    // The vao
    GLuint vao;

    // The draw mode
    GLenum drawmode;

	// The amount of elements to draw.
	unsigned int elementCount;
  };
}

#endif
