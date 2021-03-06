#include <Orange/graphics/SpriteBatch.hpp>
#include <Orange/graphics/Mesh.hpp>
#include <Orange/maths/Maths.hpp>

namespace orange {
  // Constructors
  SpriteBatch::SpriteBatch(int _maxBatch) {
    // Create memory for sprite data.
    spriteData = new SpritePoint[_maxBatch];
    spriteDataCount = 0;
    spriteDataTotalCount = _maxBatch;

    texture = nullptr;

    // Create a buffer.
    mesh.SetBuffer(0, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, position));
    mesh.SetBuffer(1, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, origin));
    mesh.SetBuffer(2, 1, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, rotation));
    mesh.SetBuffer(3, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, scale));
    mesh.SetBuffer(4, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, uvTop));
    mesh.SetBuffer(5, 2, GL_FLOAT, sizeof(SpritePoint), offsetof(SpritePoint, uvBottom));
    mesh.SetDrawMode(GL_POINTS);
  }

  SpriteBatch::~SpriteBatch() {
    // Delete the data allocated for the sprite data
    delete [] spriteData;
  }

  // Set matrices
  void SpriteBatch::SetProjection(const glm::mat4& _mat) {
    GetShader()->SetUniform("projection", _mat);
  }

  void SpriteBatch::SetView(const glm::mat4& _mat) {
    GetShader()->SetUniform("view", _mat);
  }

  // Draw
  void SpriteBatch::Draw(Texture* _texture, glm::vec2 _pos, glm::vec2 _scale, float _rot, glm::vec2 _origin, glm::vec2 _uvTopLeft, glm::vec2 _uvBottomRight) {
    // If we have a full buffer, then flush it first.
    if (spriteDataCount >= spriteDataTotalCount)
      Flush();

    // Store the texture and get the texture id.
    if (texture != _texture) {
      Flush();
    }
    texture = _texture;

    // Get a reference to a new sprite data.
    SpritePoint& point = spriteData[spriteDataCount];
    spriteDataCount++;

    // Set the position.
    point.position = _pos;
    point.scale = glm::vec2(_scale.x * _texture->GetWidth(), _scale.y * _texture->GetHeight());
    point.rotation = _rot;
    point.origin = _origin;
    point.uvTop = _uvTopLeft;
    point.uvBottom = _uvBottomRight;
  }

  void SpriteBatch::Draw(const TexturePortion* _block, glm::vec2 _pos, glm::vec2 _scale, float _rot, glm::vec2 _origin) {
    if (_block && _block->texture) {
      Draw(_block->texture, _pos, _scale, _rot, _origin, _block->topLeft, _block->bottomRight);
    }
  }

  // Flush
  void SpriteBatch::Flush() {
    // Don't really have anything to do if we don't have any data ...
    if (spriteDataCount <= 0)
      return;

    // Ensure a context
    GLContext::EnsureContext();

    // Bind our shader
    GetShader()->Bind();

    // Bind textures.
    if (texture)
      texture->Bind();

    // Draw our mesh.
    mesh.SetData((void*)spriteData, sizeof(SpritePoint) * spriteDataCount);
    mesh.SetVertexCount(spriteDataCount);
    mesh.Draw();

    // Clear our buffer.
    spriteDataCount = 0;
  }

  // Compile the shaders
  Shader* SpriteBatch::GetShader() {
    static bool compiled = false;
    static Shader shader;

    if (compiled)
      return &shader;

    // Vertex shader
    const char* vertex = R"(
      #version 400
      uniform mat4 projection;
      uniform mat4 view;

      in vec2 position;
      in vec2 origin;
      in float rotation;
      in vec2 scale;
      in vec2 uvTop;
      in vec2 uvBottom;

      out vec2 vOrigin;
      out float vRotation;
      out vec2 vScale;
      out vec2 vUvTop;
      out vec2 vUvBottom;

      void main() {
        vOrigin = origin;
        vRotation = rotation;
        vScale = scale;
        vUvTop = uvTop;
        vUvBottom = uvBottom;

        gl_Position = vec4(position, 0.0, 1.0);
      }
      )";

    const char* geometry = R"(
      #version 400
      layout (points) in;
      layout (triangle_strip, max_vertices=4) out;

      uniform mat4 projection;
      uniform mat4 view;

      in vec2 vOrigin[];
      in float vRotation[];
      in vec2 vScale[];
      in vec2 vUvTop[];
      in vec2 vUvBottom[];

      out vec2 uvCoord;
      flat out uint outTexture;

      vec4 rotatePoint(float cosValue, float sinValue, vec2 origin, vec2 point) {
        // Get the difference between each point and the origin and rotate it.
        vec2 diff = point - origin;
        vec2 rotated;
        rotated.x = diff.x * cosValue - diff.y * sinValue;
        rotated.y = diff.y * cosValue + diff.x * sinValue;
        vec2 vertex = origin + rotated;
        return vec4(vertex, 0.0, 1.0);
      }

      void main() {
        float cosTheta = cos(vRotation[0]);
        float sinTheta = sin(vRotation[0]);

        vec2 globalOrigin = gl_in[0].gl_Position.xy;
        vec2 pos = gl_in[0].gl_Position.xy - vOrigin[0];

        gl_Position = projection * view * rotatePoint(cosTheta, sinTheta, globalOrigin, pos);
        uvCoord = vUvTop[0];
        EmitVertex();

        gl_Position = projection * view * rotatePoint(cosTheta, sinTheta, globalOrigin, pos + vec2(vScale[0].x, 0.0));
        uvCoord = vec2(vUvBottom[0].x, vUvTop[0].y);
        EmitVertex();

        gl_Position = projection * view * rotatePoint(cosTheta, sinTheta, globalOrigin, pos + vec2(0, vScale[0].y));
        uvCoord = vec2(vUvTop[0].x, vUvBottom[0].y);
        EmitVertex();

        gl_Position = projection * view * rotatePoint(cosTheta, sinTheta, globalOrigin, pos + vec2(vScale[0].x, vScale[0].y));
        uvCoord = vUvBottom[0];
        EmitVertex();

        EndPrimitive();
      }

    )";

    // Fragment shader.
    const char* fragment = R"(
      #version 400

      in vec2 uvCoord;
      flat in uint outTexture;

      out vec4 frag_color;

      uniform sampler2D texture;

      void main() {
        frag_color = texture2D(texture, uvCoord);
      }
      )";

    shader.SetShaderSource(Shader::ShaderType::Fragment, fragment);
    shader.SetShaderSource(Shader::ShaderType::Vertex, vertex);
    shader.SetShaderSource(Shader::ShaderType::Geometry, geometry);
    shader.Compile();
    shader.SetAttribute("position", 0);
    shader.SetAttribute("origin", 1);
    shader.SetAttribute("rotation", 2);
    shader.SetAttribute("scale", 3);
    shader.SetAttribute("uvTop", 4);
    shader.SetAttribute("uvBottom", 5);
    shader.SetAttribute("texture", 6);
    shader.Link();

    compiled = true;

    return &shader;
  }
}
