#ifndef ORANGE_SHADER
#define ORANGE_SHADER

#include <GL/glew.h>
#include <GL/gl.h>
#include <Orange/gl/GLResource.hpp>
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
    void SetUniform(const char* _name, T& _val);

    // Bind this program
    void Bind();
  private:
    // Internal function to set uniform
    template <typename T>
    void InternalSetUniform(GLuint _name, T& _val);

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
}

#endif
