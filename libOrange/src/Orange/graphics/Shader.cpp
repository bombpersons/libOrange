#include <Orange/graphics/Shader.hpp>
#include <Orange/logging/Logging.hpp>
#include <Orange/gl/GLContext.hpp>

#include <glm/glm.hpp>
#include <cstring>
#include <cstdio>

namespace orange {

  // Program
  Shader::Shader() {
    program = 0;
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

  void Shader::SetShaderSourceFromFile(Shader::ShaderType::Type _type, const char* _path) {
    // Try and open the file
    FILE* file = fopen(_path, "r");
    if (!file) {
      LOG(Log::CRITICAL) << "Error loading shader. " << _path << " could not be found!";
      return;
    }

    // Find the size of the file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory to load the file
    char* data = new char[size];
    fread(data, size, 1, file);

    // Close the file
    fclose(file);

    // Set the shader source
    SetShaderSource(_type, data);

    // Delete the source now that we've loaded
    delete [] data;
  }

  // Link the shaders into the program
  bool Shader::Compile() {
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

    LOG(Log::DEFAULT) << "Successfully compiled shader!";

    return true;
  }

  bool Shader::Link() {
    if (!program)
      return false;

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

    LOG(Log::DEFAULT) << "Successfully linked shader!";

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

  // Convert our shader types to opengl shader types
  GLenum Shader::GetGlShaderType(Shader::ShaderType::Type _type) {
    switch (_type) {
      case Shader::ShaderType::Vertex:
        return GL_VERTEX_SHADER;
      case Shader::ShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
      case Shader::ShaderType::Geometry:
        return GL_GEOMETRY_SHADER;
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

}
