#include <Orange/util/FPSDebugCamera.hpp>
#include <Orange/window/Window.hpp>
#include <Orange/logging/Logging.hpp>

#include <Orange/maths/Maths.hpp>

namespace orange {

  FPSDebugCamera::FPSDebugCamera() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec2(0.0f, 0.0f);
    speed = 1.0f;
    sensitivity = 0.001f;
  }
  FPSDebugCamera::~FPSDebugCamera() {
  }

  // Set our location
  void FPSDebugCamera::SetPosition(const glm::vec3& _pos) {
    position = _pos;
  }

  // Set rotation (only yaw and pitch)
  void FPSDebugCamera::SetRotation(const glm::vec2& _rot) {
    rotation = _rot;
  }

  // Set speed
  void FPSDebugCamera::SetSpeed(float _speed) {
    speed = _speed;
  }

  // Set sensitivity
  void FPSDebugCamera::SetSensitivity(float _sensitivity) {
    sensitivity = _sensitivity;
  }

  // Movement
  void FPSDebugCamera::Move(float _delta, const glm::vec3& _move) {
    // Check if we are not moving at all
    if (_move.x == 0 && _move.y == 0 && _move.z == 0)
      return;

    // Get the current matrix
    GetCameraMatrix();

    // Get the facing vectors from the matrix
    glm::vec3 x(transform[0].x, transform[1].x, transform[2].x);
    glm::vec3 y(transform[0].y, transform[1].y, transform[2].y);
    glm::vec3 z(transform[0].z, transform[1].z, transform[2].z);

    // Get the translation to apply
    glm::vec3 translation;
    translation += x * _move.x;
    translation.y += _move.y; // Always go up or down globally
    translation += z * _move.z;

    // Apply it
    position += glm::normalize(translation) * speed * _delta;
  }

  // Turn
  void FPSDebugCamera::Turn(const glm::vec2& _turn) {
    rotation += _turn * sensitivity;
  }

  // Update
  void FPSDebugCamera::Update(float _delta, Window* _window) {
    // Get input and apply it
    glm::vec3 move(0.0f, 0.0f, 0.0f);
    move.z += _window->Input()->IsKeyDown(InputCode::Key::W) ? -1 : 0;
    move.z += _window->Input()->IsKeyDown(InputCode::Key::S) ? 1 : 0;
    move.x += _window->Input()->IsKeyDown(InputCode::Key::A) ? -1 : 0;
    move.x += _window->Input()->IsKeyDown(InputCode::Key::D) ? 1 : 0;
    move.y += _window->Input()->IsKeyDown(InputCode::Key::Space) ? 1 : 0;
    move.y += _window->Input()->IsKeyDown(InputCode::Key::LShift) ? -1 : 0;

    // Normalize the movement
    if (move.x != 0 || move.y != 0 || move.z != 0)
      move = glm::normalize(move);

    // Move
    Move(_delta, move);

    // Get the rotation
    glm::vec2 turn = _window->Input()->GetRelativeMouseMove();
    Turn(turn);

    LOG(Log::DEFAULT) << "Pos: " << position.x << ", " << position.y << ", " << position.z
                      << " Rot: " << rotation.x << ", " << rotation.y;
  }

  // Get the camera matrix
  const glm::mat4& FPSDebugCamera::GetCameraMatrix() {
    // Create an identity matrix.
    transform = glm::mat4();

    // Rotate
    transform = glm::rotate(transform, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));

    // Now set translation
    transform = glm::translate(transform, -position);

    // Return
    return transform;
  }

}
