#ifndef ORANGE_SHADER
#define ORANGE_SHADER

#include <GL/glew.h>
#include <GL/GL.h>
#include <Orange/gl/GLResource.hpp>
#include <Orange/gl/GLContext.hpp>
#include <Orange/logging/Logging.hpp>
#include <glm/glm.hpp>
#include <map>

namespace orange {
  class Shader : public GLResource {
  public:
    struct ShaderType {
      enum Type {
        Fragment,
        Vertex,
        Count
      };
    };
  public:
    Shader();
    virtual ~Shader();

    // Set a shader on the program
    void SetShaderSource(ShaderType::Type _type, const char* _source);
    void SetShaderSourceFromFile(Shader::ShaderType::Type _type, const char* _path);

    // Link the shaders into the program
    bool Compile();
    bool Link();

    // Set an attribute on the shader.
    void SetAttribute(const char* _name, int _index);

    // Set a uniform on the shader
    template <typename T>
    void SetUniform(const char* _name, T _val);

    // Bind this program
    void Bind();
  private:
    // Internal function to set uniform
    template <typename T>
    void InternalSetUniform(GLuint _name, T _val);

    // Convert our shader types to opengl shader types
    GLenum GetGlShaderType(ShaderType::Type _type);
  private:
    // The program
    GLuint program;

    // Shader sources
    char* sources[ShaderType::Count] = {};

    // Map of uniforms
    std::map<std::string, GLuint> uniforms;
  };

  // Set a uniform on the shader
  template <typename T>
  void Shader::SetUniform(const char* _name, T _val) {
    // Make sure we have a context
    GLContext::EnsureContext();

    // Can't set a uniform if we don't have a program
    if (!program)
      return;

    // See if we have the uniform cached
    GLuint uniform;
    auto uniformIterator = uniforms.find(_name);
    if (uniformIterator == uniforms.end()) {
      // Find the uniform
      uniform = glGetUniformLocation(program, _name);
      if (uniform == -1)
        return;

      uniforms[_name] = uniform;
    } else {
      uniform = uniformIterator->second;
    }

    // Get the currently bound shader
    GLint prevProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);

    // Bind our program
    glUseProgram(program);

    // Actually set the uniform value.
    InternalSetUniform(uniforms[_name], _val);

    // Bind the previously bound program
    glUseProgram((GLuint)prevProgram);
  }

  // Internal function to set uniform
  template <typename T>
  void Shader::InternalSetUniform(GLuint _name, T _val) {
    LOG(Log::CRITICAL) << "Error setting uniform on shader! There is no generic InternalSetUniform method!";
  }

  template <>
  inline void Shader::InternalSetUniform<unsigned int>(GLuint _name, unsigned int _val) {
    glUniform1ui(_name, _val);
  }

  template <>
  inline void Shader::InternalSetUniform<int>(GLuint _name, int _val) {
    glUniform1i(_name, _val);
  }

  template <>
  inline void Shader::InternalSetUniform<float>(GLuint _name, float _val) {
    glUniform1f(_name, _val);
  }

  template <>
  inline void Shader::InternalSetUniform<glm::vec2>(GLuint _name, glm::vec2 _val) {
    glUniform2f(_name, _val.x, _val.y);
  }

  template <>
  inline void Shader::InternalSetUniform<glm::vec3>(GLuint _name, glm::vec3 _val) {
    glUniform3f(_name, _val.x, _val.y, _val.z);
  }

  template <>
  inline void Shader::InternalSetUniform<glm::vec4>(GLuint _name, glm::vec4 _val) {
    glUniform4f(_name, _val.x, _val.y, _val.z, _val.w);
  }

  template <>
  inline void Shader::InternalSetUniform<glm::mat4>(GLuint _name, glm::mat4 _val) {
    glUniformMatrix4fv(_name, 16, false, (GLfloat*)&_val);
  }
}

#endif
