#ifndef ORANGE_FPSDEGUGCAMERA
#define ORANGE_FPSDEBUGCAMERA

#include <glm/glm.hpp>

namespace orange {
  class Window;
  class FPSDebugCamera {
  public:
    FPSDebugCamera();
    virtual ~FPSDebugCamera();

    // Set our location
    void SetPosition(const glm::vec3& _pos);

    // Set rotation (only yaw and pitch)
    void SetRotation(const glm::vec2& _rot);

    // Set speed
    void SetSpeed(float _speed);

    // Set sensitivity
    void SetSensitivity(float _sensitivity);

    // Movement
    void Move(float _delta, const glm::vec3& _move);

    // Turn
    void Turn(const glm::vec2& _turn);

    // Update
    void Update(float _delta, Window* _window);

    // Get the camera matrix
    const glm::mat4& GetCameraMatrix();

  private:
    // Speed and sensitivity
    float speed; // move speed
    float sensitivity; // turn speed

    // Location
    glm::vec3 position;
    glm::vec2 rotation;

    // The current matrix.
    glm::mat4 transform;
  };
}

#endif
