#include <Orange/graphics/Mesh.hpp>
#include <Orange/gl/GLContext.hpp>
#include <Orange/logging/Logging.hpp>

namespace orange {
  Mesh::Mesh() {
    vao = 0;
    ibo = 0;
    size = 0;
    drawmode = Mesh::DrawMode::Triangles;
  }

  Mesh::~Mesh() {
    for (int i = 0; i < ORANGE_MESH_MAX_VBOS; ++i) {
      if (vbos[i])
        glDeleteBuffers(1, &(vbos[i]));
    }
    if (vao)
      glDeleteVertexArrays(1, &vao);
  }

  // Set data in a buffer
  bool Mesh::SetBuffer(int _index, float* _data, unsigned int _elementsize, unsigned int _totalsize) {
    // If the total size is not the same as previous buffers, complain and error
    if (_totalsize != size) {
      if (size != 0) {
        LOG(Log::CRITICAL) << "Cannot set buffers on mesh that don't have the same size!";
        return false;
      }
    }

    if (vbos[_index]) {
      // Delete the vbo
      glDeleteBuffers(1, &(vbos[_index]));
    }

    // Create an vbo
    glGenBuffers(1, &(vbos[_index]));
    if (!vbos[_index]) {
      LOG(Log::CRITICAL) << "Error generating VBO! OpenGL Error: " << glGetError();
      return false;
    }

    // Bind the buffer and buffer the data
    glBindBuffer(GL_ARRAY_BUFFER, vbos[_index]);
    glBufferData(GL_ARRAY_BUFFER, _totalsize * _elementsize * sizeof(float), _data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Set the elementsize
    size = _totalsize;
    elementsizes[_index] = _elementsize;
    return true;
  }

  bool Mesh::SetBufferIndices(unsigned int* _data, unsigned int _totalsize) {
    // If the total size is not the same as previous buffers, complain and error
    if (_totalsize != size) {
      if (size != 0) {
        LOG(Log::CRITICAL) << "Cannot set buffers on mesh that don't have the same size!";
        return false;
      }
    }

    if (ibo)
      glDeleteBuffers(1, &ibo);

    // Create an vbo
    glGenBuffers(1, &ibo);
    if (!ibo) {
      LOG(Log::CRITICAL) << "Error generating VBO! OpenGL Error: " << glGetError();
      return false;
    }

    // Bind the buffer and buffer the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _totalsize * sizeof(unsigned int), _data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Set the elementsize
    size = _totalsize;
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

    // If there isn't an ibo, then make a default one.
    if (!ibo) {
      unsigned int* buf = new unsigned int[size];
      for (unsigned int i = 0; i < size; ++i) {
        buf[i] = i;
      }
      if (!SetBufferIndices(buf, size)) {
        LOG(Log::CRITICAL) << "Couldn't automatically generate an IBO for mesh!";
        return false;
      }
    }

    // Create the vertex array
    glGenVertexArrays(1, &vao);

    // Bind it
    glBindVertexArray(vao);

    // Loop through all of our vbo's
    for (int i = 0; i < ORANGE_MESH_MAX_VBOS; ++i) {
      if (vbos[i]) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glVertexAttribPointer(i, elementsizes[i], GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(i);
      }
    }

    // Bind the ibo so that we vertex array can use it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Unbind the vertex array
    glBindVertexArray(0);
    return true;
  }

  // Draw
  void Mesh::Draw() {
    if (!vao)
      return;

    glBindVertexArray(vao);
    glDrawElements(drawmode, size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}
