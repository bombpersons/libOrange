#include <Orange/graphics/SpriteBatch.hpp>
#include <Orange/graphics/Mesh.hpp>
#include <Orange/maths/Maths.hpp>

namespace orange {
  // Constructors
  SpriteBatch::SpriteBatch(int _maxBatch, int _maxTextures) {
    // Create memory for sprite data.
    spriteData = new SpritePoint[_maxBatch];
    spriteDataCount = 0;
    spriteDataTotalCount = _maxBatch;

    textures = new Texture*[_maxTextures];
    texturesCount = 0;
    maxTexturesCount = _maxTextures; // Max number of different textures to store before flushing.

    // Create a buffer.
    mesh.SetBuffer(0, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, position));
    mesh.SetBuffer(1, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, origin));
    mesh.SetBuffer(2, 1, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, rotation));
    mesh.SetBuffer(3, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, scale));
    mesh.SetBuffer(4, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, uvTop));
    mesh.SetBuffer(5, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, uvBottom));
    mesh.SetBuffer(6, 1, GL_UNSIGNED_INT, sizeof(SpritePoint), offsetof(SpritePoint, texture));
    mesh.SetDrawMode(GL_POINTS);
  }

  SpriteBatch::~SpriteBatch() {
    // Delete the data allocated for the sprite data
    delete [] spriteData;
    delete [] textures;
  }

  // Draw
  void SpriteBatch::Draw(Texture* _texture, glm::vec2 _pos, glm::vec2 _scale, float _rot, glm::vec2 _origin, glm::vec2 _uvTopLeft, glm::vec2 _uvBottomRight) {
    // If we have a full buffer, then flush it first.
    if (spriteDataCount >= spriteDataTotalCount)
      Flush();

    // Either find a texture or insert a new one.
    bool found = false;
    unsigned int textureId = 0;
    for (int i = 0; i < texturesCount; ++i) {
      if (textures[i] == _texture) {
        textureId = i;
        found = true;
        break;
      }
    }

    // Store the texture and get the texture id.
    if (!found) {
      // If we are using too many textures
      if (texturesCount >= maxTexturesCount)
        Flush();

      textureId = texturesCount;
      textures[texturesCount] = _texture;
      texturesCount++;
    }

    // Get a reference to a new sprite data.
    SpritePoint& point = spriteData[spriteDataCount];
    spriteDataCount++;

    // Set the position.
    point.position = _pos;
    point.scale = glm::vec2(1.0f, 1.0f); //glm::vec2(_scale.x * _texture->GetWidth(), _scale.y * _texture->GetHeight()); 
    point.rotation = _rot;
    point.origin = _origin;
    point.uvTop = _uvTopLeft;
    point.uvBottom = _uvBottomRight;
    point.texture = textureId;
  }

  // Flush
  void SpriteBatch::Flush() {
    // Ensure a context
    GLContext::EnsureContext();

    // Bind our shader
    GetShader()->Bind();

    // Bind textures.
    for (int i = 0; i < texturesCount; ++i) {
      textures[i]->Bind(i);
    }

    // Draw our mesh.
    mesh.SetData((void*)spriteData, sizeof(SpritePoint) * spriteDataCount);
    mesh.SetVertexCount(spriteDataCount);
    mesh.Draw();
    
    // Clear our buffer.
    spriteDataCount = 0;
    texturesCount = 0;
  }

  // Compile the shaders
  Shader* SpriteBatch::GetShader() {
    static bool compiled = false;
    static Shader shader;

    if (compiled)
      return &shader;

    // Vertex shader
    const char* vertex = R"END(
      #version 400
      uniform mat4 projection;
      uniform mat4 view;

      in vec2 position;
      in vec2 origin;
      in float rotation;
      in vec2 scale;
      in vec2 uvTop;
      in vec2 uvBottom;
      in uint texture;

      out outData {
        vec2 vOrigin;
        float vRotation;
        vec2 vScale;

        vec2 vUvTop;
        vec2 vUvBottom;
        uint vTexture;

        vec2 uvCoord;
      } outData;

      void main() {
        outData.vOrigin = origin;
        outData.vRotation = rotation;
        outData.vScale = scale;//projection * view * vec4(scale, 0.0, 1.0);
        outData.vUvTop = uvTop;
        outData.vUvBottom = uvBottom;
        outData.vTexture = texture;

        gl_Position = vec4(position, 0.0, 1.0);//projection * view * vec4(position, 0.0, 1.0);
      }
      )END";

    const char* geometry = R"END(
      #version 400
      layout (points) in;
      layout (triangle_strip, max_vertices=4) out;

      in vec2 vOrigin[];
      in float vRotation[];
      in vec2 vScale[];
      in vec2 vUvTop[];
      in vec2 vUvBottom[];
      in uint texture[];
      
      out vec2 uvCoord;

      void main() {
        //gl_Position = vec4(gl_in[0].gl_Position.xy + vec2(vScale[0].x * -0.5, vScale[0].y * -0.5), 0.0, 1.0);
        //gl_Position = gl_in[0].gl_Position;
        gl_Position = vec4(-0.1, -0.1, 0.0, 1.0);
        uvCoord = vUvTop[0];
        //outTexture = texture[0];
        EmitVertex();

        //gl_Position = vec4(gl_in[0].gl_Position.xy + vec2(vScale[0].x * 0.5, vScale[0].y * -0.5), 0.0, 1.0);
        //gl_Position = gl_in[0].gl_Position;
        gl_Position = vec4(0.1, -0.1, 0.0, 1.0);
        uvCoord = vec2(vUvBottom[0].x, vUvTop[0].y);
        //outTexture = texture[0];
        EmitVertex();

        //gl_Position = vec4(gl_in[0].gl_Position.xy + vec2(vScale[0].x * -0.5, vScale[0].y * 0.5), 0.0, 1.0);
        //gl_Position = gl_in[0].gl_Position;
        gl_Position = vec4(-0.1, 0.1, 0.0, 1.0);
        uvCoord = vec2(vUvTop[0].x, vUvBottom[0].y);
        //outTexture = texture[0];
        EmitVertex();

        //gl_Position = vec4(gl_in[0].gl_Position.xy + vec2(vScale[0].x * 0.5, vScale[0].y * 0.5), 0.0, 1.0);
        //gl_Position = gl_in[0].gl_Position;
        gl_Position = vec4(0.1, 0.1, 0.0, 1.0);
        uvCoord = vUvBottom[0];
        //outTexture = texture[0];
        EmitVertex();

        EndPrimitive();
      }      

    )END";

    // Fragment shader.
    const char* fragment = R"END(
      #version 400
      
      in vec2 uvCoord;

      out vec4 frag_color;

      uniform sampler2D tex;

      void main() {
        frag_color = texture2D(tex, uvCoord);
        //frag_color = vec4(1.0, 0.0, 0.0, 1.0);
      }
      )END";

    shader.SetShaderSource(Shader::ShaderType::Fragment, fragment);
    shader.SetShaderSource(Shader::ShaderType::Vertex, vertex);
    //shader.SetShaderSource(Shader::ShaderType::Geometry, geometry);
    shader.Compile();
    shader.SetAttribute("position", 0);
    shader.SetAttribute("origin", 1);
    shader.SetAttribute("rotation", 2);
    shader.SetAttribute("scale", 3);
    shader.SetAttribute("uvTop", 4);
    shader.SetAttribute("uvBottom", 5);
    shader.SetAttribute("texture", 6);
    shader.Link();

    shader.SetUniform("tex", 0);

    // TEMP
    shader.SetUniform("projection", glm::ortho(-1000.0f, 1000.0f, 1000.0f, -1000.0f));
    shader.SetUniform("view", glm::mat4());

    compiled = true;
  
    return &shader;
  }
}