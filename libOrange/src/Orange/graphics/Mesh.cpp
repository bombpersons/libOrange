#include <Orange/graphics/Mesh.hpp>
#include <Orange/gl/GLContext.hpp>

namespace orange {
  Mesh::Mesh() {
    vao = 0;
  }

  Mesh::~Mesh() {
    for (int i = 0; i < ORANGE_MESH_MAX_VBOS; ++i) {
      if (vbos[_index])
        glDeleteBuffers(1, &(vbos[i]));
    }
    if (vao)
      glDeleteVertexArrays(1, &vao);
  }

  // Set data in a buffer
  bool Mesh::SetBuffer(int _index, float* _data, unsigned int _elementsize, unsigned int _totalsize) {
    if (vbos[_index]) {
      // Delete the vbo
      glDeleteBuffers(1, &(vbos[_index]));
    }

    // Create an vbo
    vbos[_index] = glGenBuffers(1, &(vbos[_index]));
    if (!vbos[_index]) {
      LOG(Log::CRITICAL) << "Error generating VBO! OpenGL Error: " << glGetError();
      return false;
    }

    // Bind the buffer and buffer the data
    glBindBuffer(GL_ARRAY_BUFFER, vbos[_index]);
    glBufferData(GL_ARRAY_BUFFER, _totalsize, _data, GL_STATIC_DRAW);

    // Set the elementsize
    sizes[_index] = _elementsize;
    return true;
  }

  // Set draw mode
  bool Mesh::SetDrawMode(Mesh::DrawMode::Type _drawmode) {
    drawmode = _drawmode;
  }

  // Write the vbo into a vao
  bool Mesh::Finish() {
    if (vao)
      glDeleteVertexArrays(1, &vao);

    // Create the vertex array
    glGenVertexArrays(1, &vao);

    // Bind it
    glBindVertexArray(vao);

    // Loop through all of our vbo's
    for (int i = 0; i < ORANGE_MESH_MAX_VBOS; ++i) {
      if (vbos[i]) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(i);
      }
    }

    return true;
  }

  // Draw
  void Mesh::Draw() {
    if (!vao)
      return;

    glBindVertexArray(vao);
    glDrawArrays(drawmode, 0, indicecount);
  }
}
