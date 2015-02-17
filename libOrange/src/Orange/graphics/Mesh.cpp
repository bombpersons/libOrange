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
    elementcount = 0;
    drawmode = GL_TRIANGLES;
  }

  Mesh::~Mesh() {
    if (vbo)
        glDeleteBuffers(1, &vbo);
    if (vao)
      glDeleteVertexArrays(1, &vao);
  }

  // Set the mesh as a cube
  // Set's positions and UV coords
  static Mesh* cube = nullptr;
  const Mesh& Mesh::Cube() {
    static float verts[] = {
      -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,

      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,

      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,

      -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f, -0.5f,

      -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f
    };

    static float uvs[] = {
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,

      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,

      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 0.0f,

      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 0.0f,

      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,

      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f
    };

    if (!cube) {
      cube = new Mesh();
      cube->SetBuffer(0, (unsigned char*)verts, sizeof(float), 3, 36, GL_FLOAT);
      cube->SetBuffer(1, (unsigned char*)uvs, sizeof(float), 2, 36, GL_FLOAT);
      cube->SetDrawMode(GL_TRIANGLES);
      cube->Finish();
    }

    return *cube;
  }

  // Set data in a buffer
  bool Mesh::SetBuffer(int _index, unsigned char* _data, unsigned int _componentsize, unsigned int _componentcount, unsigned int _totalsize, GLenum _type) {
    GLContext::EnsureContext();
    
    if (_index >= ORANGE_MESH_MAX_VBOS || _index < 0) {
      LOG(Log::CRITICAL) << "Tried to set a buffer with an out of bounds index " << _index;
      return false;
    }

    // If the total size is not the same as previous buffers, complain and error
    if (_totalsize != elementcount) {
      if (elementcount != 0) {
        LOG(Log::CRITICAL) << "Cannot set buffers on mesh that don't have the same size!";
        return false;
      }
    }
    elementcount = _totalsize;

    // Grab the info for this index.
    BufferInfo& info = bufferInfo[_index];

    // If there is data there, delete it.
    if (info.data)
      delete [] info.data;

    // If there was no data, set the data to nullptr
    if (!_data) {
      info.data = nullptr;
      info.componentcount = 0;
      info.componentsize = 0;
      info.type = _type;
      return false;
    }

    // Copy the data if there is any
    info.data = new char[_componentcount * _componentsize * _totalsize];
    memcpy(info.data, _data, _componentcount * _componentsize * _totalsize);

    info.componentcount = _componentcount;
    info.componentsize = _componentsize;
    info.type = _type;
    return true;
  }

  bool Mesh::SetBufferIndices(unsigned int* _data, unsigned int _totalsize) {
    GLContext::EnsureContext();
    
    // If the total size is not the same as previous buffers, complain and error
    if (_totalsize != elementcount) {
      if (elementcount != 0) {
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
    elementcount = _totalsize;
    return true;
  }

  // Set draw mode
  void Mesh::SetDrawMode(GLenum _drawmode) {
    drawmode = _drawmode;
  }

  // Write the vbo into a vao
  bool Mesh::Finish() {
    GLContext::EnsureContext();

    // We need to create a singal vbo to contain all of the buffers,
    // Then interweave the buffers into it.
    // First get the size of this new buffers
    int totalElementSize = 0;
    for (int i = 0; i < ORANGE_MESH_MAX_VBOS; ++i) {
      totalElementSize += bufferInfo[i].componentcount * bufferInfo[i].componentsize;
    }
    
    // Create the buffer
    unsigned int buffersize = totalElementSize * elementcount;

    // If create the buffer if it doesn't exist already
    if (!vbo)
      glGenBuffers(1, &vbo);
    if (!vbo) {
      LOG(Log::CRITICAL) << "Error generating VBO! OpenGL Error: " << glGetError();
      return false;
    }

    // Bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buffersize, nullptr, GL_STATIC_DRAW);

    // Map the buffer
    unsigned char* buffer = (unsigned char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, buffersize, GL_MAP_WRITE_BIT);
    if (!buffer) {
      LOG(Log::CRITICAL) << "Error mapping vertex buffer! OpenGL Error: " << glGetError();
      return false;
    }

    // Loop through all of our buffers and interwieve them.
    unsigned char* pointer = buffer;
    for (int i = 0; i < elementcount; ++i) {
      for (int j = 0; j < ORANGE_MESH_MAX_VBOS; ++j) {
        if (bufferInfo[j].data) {
          memcpy(pointer, (void*)(bufferInfo[j].data + bufferInfo[j].componentcount * bufferInfo[j].componentsize * i), bufferInfo[j].componentsize * bufferInfo[j].componentcount);
          pointer += bufferInfo[j].componentsize * bufferInfo[j].componentcount;
        }
      }
    }
    
    // Unmap the buffer
    glUnmapBuffer(GL_ARRAY_BUFFER);

    // Create the vertex array object.
    if (vao)
      glDeleteVertexArrays(1, &vao);

    // If there isn't an ibo, then make a default one.
    if (!ibo) {
      unsigned int* buf = new unsigned int[elementcount];
      for (unsigned int i = 0; i < elementcount; ++i) {
        buf[i] = i;
      }
      if (!SetBufferIndices(buf, elementcount)) {
        LOG(Log::CRITICAL) << "Couldn't automatically generate an IBO for mesh!";
        return false;
      }
    }

    // Create the vertex array
    glGenVertexArrays(1, &vao);

    // Bind it
    glBindVertexArray(vao);

    // Bind vertex attrib pointers with the correct stride.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    unsigned int offset = 0;
    for (int i = 0; i < ORANGE_MESH_MAX_VBOS; ++i) {
      glVertexAttribPointer(i, bufferInfo[i].componentcount, bufferInfo[i].type, GL_FALSE, totalElementSize, (GLvoid*)offset);
      glEnableVertexAttribArray(i);

      offset += bufferInfo[i].componentcount * bufferInfo[i].componentsize;
    }

    // Bind the ibo so that we vertex array can use it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Unbind the vertex array
    glBindVertexArray(0);
    return true;
  }

  // Draw
  void Mesh::Draw() const {
    GLContext::EnsureContext();

    if (!vao)
      return;

    glBindVertexArray(vao);
    glDrawElements(drawmode, elementcount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}
