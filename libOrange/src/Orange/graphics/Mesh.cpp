#include <Orange/graphics/Mesh.hpp>
#include <Orange/gl/GLContext.hpp>
#include <Orange/logging/Logging.hpp>
#include <Orange/maths/Maths.hpp>
#include <cstring>

namespace orange {
  Mesh::Mesh() {
    vao = 0;
    vbo = 0;
    ibo = 0;
    elementCount = 0;
    drawmode = GL_TRIANGLES;

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
  }

  Mesh::~Mesh() {
    if (vbo)
      glDeleteBuffers(1, &vbo);
    if (vao)
      glDeleteVertexArrays(1, &vao);
    if (ibo)
      glDeleteBuffers(1, &ibo);
  }

  // Set the mesh as a cube
  // Set's positions and UV coords
  static Mesh* cube = nullptr;
  const Mesh& Mesh::Cube() {
    static float verts[] = {
      -0.5f, -0.5f, -0.5f,
      0.0f, 0.0f,

      0.5f, -0.5f, -0.5f,
      1.0f, 0.0f,

      0.5f,  0.5f, -0.5f,
      1.0f, 1.0f,

      0.5f,  0.5f, -0.5f,
      1.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,
      0.0f, 1.0f,

      -0.5f, -0.5f, -0.5f,
      0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,
      0.0f, 0.0f,

      0.5f, -0.5f,  0.5f,
      1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,
      1.0f, 1.0f,

      0.5f,  0.5f,  0.5f,
      1.0f, 1.0f,

      -0.5f,  0.5f,  0.5f,
      0.0f, 1.0f,

      -0.5f, -0.5f,  0.5f,
      0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,
      1.0f, 0.0f,

      -0.5f,  0.5f, -0.5f,
      1.0f, 1.0f,

      -0.5f, -0.5f, -0.5f,
      0.0f, 1.0f,

      -0.5f, -0.5f, -0.5f,
      0.0f, 1.0f,

      -0.5f, -0.5f,  0.5f,
      0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,
      1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,
      1.0f, 0.0f,

      0.5f,  0.5f, -0.5f,
      1.0f, 1.0f,

      0.5f, -0.5f, -0.5f,
      0.0f, 1.0f,

      0.5f, -0.5f, -0.5f,
      0.0f, 1.0f,

      0.5f, -0.5f,  0.5f,
      0.0f, 0.0f,

      0.5f,  0.5f,  0.5f,
      1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,
      0.0f, 1.0f,

      0.5f, -0.5f, -0.5f,
      1.0f, 1.0f,

      0.5f, -0.5f,  0.5f,
      1.0f, 0.0f,

      0.5f, -0.5f,  0.5f,
      1.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,
      0.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,
      0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,
      0.0f, 1.0f,

      0.5f,  0.5f, -0.5f,
      1.0f, 1.0f,

      0.5f,  0.5f,  0.5f,
      1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,
      1.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,
      0.0f, 0.0f,

      -0.5f,  0.5f, -0.5f,
      0.0f, 1.0f

    };

    if (!cube) {
      cube = new Mesh();
      cube->SetData(verts, sizeof(verts));
      cube->SetBuffer(0, 3, GL_FLOAT, sizeof(float) * 5, 0);
      cube->SetBuffer(1, 2, GL_FLOAT, sizeof(float) * 5, sizeof(float) * 3);
      cube->SetVertexCount(36);
      cube->SetDrawMode(GL_TRIANGLES);
    }

    return *cube;
  }

  // Set the vertex data.
  bool Mesh::SetData(void* _data, unsigned int _totalSize) {
	  // Bind the buffer
	  glBindBuffer(GL_ARRAY_BUFFER, vbo);
	  glBufferData(GL_ARRAY_BUFFER, _totalSize, _data, GL_STATIC_DRAW);
	  glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
  }

  // Set an attribute
  bool Mesh::SetBuffer(unsigned int _index, unsigned int _attribSize, GLenum _type, unsigned int _stride, unsigned int _offset) {
	  GLContext::EnsureContext();

	  glBindVertexArray(vao);

    // Bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

	  // Set up the attrib settings.
	  glVertexAttribPointer(_index, _attribSize, _type, GL_FALSE, _stride, (void*)_offset);
	  glEnableVertexAttribArray(_index);

    // Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	  // Unbind the vertex array
	  glBindVertexArray(0);

	  return true;
  }

  bool Mesh::SetIndices(unsigned int* _data, unsigned int _count) {
    GLContext::EnsureContext();

    if (!_data) {
      if (ibo)
        glDeleteBuffers(1, &ibo);
      ibo = 0;
    }

    if (!ibo)
      glGenBuffers(1, &ibo);

    // Bind the buffer and buffer the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(unsigned int), (unsigned int*)_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	  // Create a vertex array object if we don't have one.
	  glBindVertexArray(vao);

	  // Bind the ibo so that we vertex array can use it.
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	  // Unbind the vertex array
	  glBindVertexArray(0);

    return true;
  }

  // Set draw mode
  void Mesh::SetDrawMode(GLenum _drawmode) {
    drawmode = _drawmode;
  }

  // Draw
  void Mesh::Draw() const {
    GLContext::EnsureContext();

    if (!vao)
      return;

    glBindVertexArray(vao);
	  if (ibo)
		  glDrawElements(drawmode, elementCount, GL_UNSIGNED_INT, 0);
	  else
		  glDrawArrays(drawmode, 0, elementCount);
    glBindVertexArray(0);
  }
}
