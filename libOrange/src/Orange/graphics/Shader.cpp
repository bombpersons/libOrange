#include <Orange/graphics/Shader.hpp>
#include <Orange/logging/Logging.hpp>
#include <Orange/gl/GLContext.hpp>

#include <glm/glm.hpp>
#include <cstring>

namespace orange {

  // Program
  Shader::Shader() {
  }
  Shader::~Shader() {
    if (program)
      glDeleteProgram(program);
  }

  // Set a shader on the program
  void Shader::SetShaderSource(Shader::ShaderType::Type _type, const char* _source) {
    // Delete the existing source
    if (sources[_type])
      delete [] sources[_type];
    sources[_type] = nullptr;

    // If the source is null
    if (!_source)
      return;

    // Copy the source
    int length = strlen(_source) + 1; // Add one to count the null terminating character.
    sources[_type] = new char[length];
    memcpy(sources[_type], _source, length+1);

    // Ok, done
    return;
  }

  // Link the shaders into the program
  bool Shader::LinkProgram() {
    // Make sure we have a context
    GLContext::EnsureContext();

    // Check if we have any sources
    bool exists = false;
    for (int i = 0; i < Shader::ShaderType::Count; ++i) {
      if (sources[i] != nullptr) {
        exists = true;
        break;
      }
    }

    if (!exists) {
      // Cannot compile shader, no sources
      LOG(Log::CRITICAL) << "Error compiling shader, no sources have been set!";
      return false;
    }

    // Create the program
    if (program)
      glDeleteProgram(program);
    program = glCreateProgram();

    // Compile and attach all of the shaders
    for (int i = 0; i < Shader::ShaderType::Count; ++i) {
      if (sources[i] == nullptr)
        break;

      // Create the shader object
      GLuint shader = glCreateShader(GetGlShaderType((Shader::ShaderType::Type)i));
      if (!shader) {
        LOG(Log::CRITICAL) << "Couldn't create shader object! OpenGL Error: " << glGetError();
        return false;
      }

      // Set the shader source
      glShaderSource(shader, 1, (const char**)&(sources[i]), nullptr);

      // Compile it
      glCompileShader(shader);

      // Check the status of the compilation
      GLint result;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
      if (result == GL_FALSE) {
        LOG(Log::CRITICAL) << "Error compiling shader!";

        // Get the error from opengl
        char* log;
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        // Allocate a string to store.
        log = new char[length];

        // Get the error messages
        glGetShaderInfoLog(shader, length, &result, log);
        LOG(Log::CRITICAL) << log;

        // Delete the log
        delete [] log;

        // Delete the uncompiled shader
        glDeleteShader(shader);
        shader = 0;
        return false;
      }

      // Set the shader on the program
      glAttachShader(program, shader);

      // Delete the shader, won't actually be deleted until the program
      // is deleted.
      glDeleteShader(shader);
    }

    // Link the program
    glLinkProgram(program);

    // Get the results
    GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
      GLint length;
      char *log;

      LOG(Log::CRITICAL) << "Error linking shader!";

      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
      log = new char[length];
      glGetProgramInfoLog(program, length, &result, log);

      // Print log
      LOG(Log::CRITICAL) << log;

      // Delete log
      delete [] log;
      return false;
    }

    return true;
  }

  // Set an attribute on the shader.
  void Shader::SetAttribute(const char* _name, int _index) {
    // Make sure we have a context
    GLContext::EnsureContext();

    // Quit if we don't have a program
    if (!program)
      return;

    // Bind the attrib
    glBindAttribLocation(program, _index, _name);
  }

  // Set a uniform on the shader
  template <typename T>
  void Shader::SetUniform(const char* _name, T& _val) {
    // Make sure we have a context
    GLContext::EnsureContext();

    // Can't set a uniform if we don't have a program
    if (!program)
      return;

    // See if we have the uniform cached
    GLuint uniform;
    if (uniforms.find(_name) == uniforms.end()) {
      // Find the uniform
      uniform = glGetUniformLocation(program, _name);
      if (uniform == -1)
        return;

      uniforms[_name] == uniform;
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
  void Shader::InternalSetUniform(GLuint _name, T& _val) {
    LOG(Log::CRITICAL) << "Error setting uniform on shader! There is no generic InternalSetUniform method!";
  }

  template <>
  void Shader::InternalSetUniform(GLuint _name, unsigned int& _val) {
    glUniform1ui(_name, _val);
  }

  template <>
  void Shader::InternalSetUniform(GLuint _name, int& _val) {
    glUniform1i(_name, _val);
  }

  template <>
  void Shader::InternalSetUniform(GLuint _name, float& _val) {
    glUniform1f(_name, _val);
  }

  template <>
  void Shader::InternalSetUniform(GLuint _name, glm::vec2& _val) {
    glUniform2f(_name, _val.x, _val.y);
  }

  template <>
  void Shader::InternalSetUniform(GLuint _name, glm::vec3& _val) {
    glUniform3f(_name, _val.x, _val.y, _val.z);
  }

  template <>
  void Shader::InternalSetUniform(GLuint _name, glm::vec4& _val) {
    glUniform4f(_name, _val.x, _val.y, _val.z, _val.w);
  }

  template <>
  void Shader::InternalSetUniform(GLuint _name, glm::mat4& _val) {
    glUniformMatrix4fv(_name, 16, false, (GLfloat*)&_val);
  }

  // Convert our shader types to opengl shader types
  GLenum Shader::GetGlShaderType(Shader::ShaderType::Type _type) {
    switch (_type) {
      case Shader::ShaderType::Vertex:
        return GL_VERTEX_SHADER;
      case Shader::ShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
      case Shader::ShaderType::Count:
      default:
        LOG(Log::WARNING) << "Invalid shader type!";
    }

    return 0;
  }

  // Bind the program
  void Shader::Bind() {
    // Make sure we have a context
    GLContext::EnsureContext();

    // Bind our program
    glUseProgram(program);
  }

  // Declare valid template instantiations
  template void Shader::SetUniform<unsigned int>(const char* _name, unsigned int& _val);
  template void Shader::SetUniform<int>(const char* _name, int& _val);
  template void Shader::SetUniform<float>(const char* _name, float& _val);
  template void Shader::SetUniform<glm::vec2>(const char* _name, glm::vec2& _val);
  template void Shader::SetUniform<glm::vec3>(const char* _name, glm::vec3& _val);
  template void Shader::SetUniform<glm::vec4>(const char* _name, glm::vec4& _val);
  template void Shader::SetUniform<glm::mat4>(const char* _name, glm::mat4& _val);

  template void Shader::InternalSetUniform<unsigned int>(GLuint _name, unsigned int& _val);
  template void Shader::InternalSetUniform<int>(GLuint _name, int& _val);
  template void Shader::InternalSetUniform<float>(GLuint _name, float& _val);
  template void Shader::InternalSetUniform<glm::vec2>(GLuint _name, glm::vec2& _val);
  template void Shader::InternalSetUniform<glm::vec3>(GLuint _name, glm::vec3& _val);
  template void Shader::InternalSetUniform<glm::vec4>(GLuint _name, glm::vec4& _val);
  template void Shader::InternalSetUniform<glm::mat4>(GLuint _name, glm::mat4& _val);

}
