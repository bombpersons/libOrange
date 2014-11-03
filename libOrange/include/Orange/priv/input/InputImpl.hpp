#ifndef ORANGE_INPUTIMPL
#define ORANGE_INPUTIMPL

#include <glm/fwd.hpp>
#include <Orange/input/Input.hpp>

namespace orange {
  namespace priv {
    class WindowImpl;
    class InputImpl {
    public:
      InputImpl() {}
      virtual ~InputImpl() {}

      // Create
      static InputImpl* Create(WindowImpl* _window);

      // Keyboard
      virtual bool IsKeyDown(InputCode::Key::Key _key) = 0;
      virtual bool IsKeyPressed(InputCode::Key::Key _key) = 0;
      virtual bool IsKeyReleased(InputCode::Key::Key _key) = 0;

      // Mouse
      virtual glm::vec2 GetMousePos() = 0;
      virtual bool IsMouseButtonDown(InputCode::MouseButton::MouseButton _button) = 0;
      virtual bool IsMouseButtonPressed(InputCode::MouseButton::MouseButton _button) = 0;
      virtual bool IsMouseButtonReleased(InputCode::MouseButton::MouseButton _button) = 0;
      virtual int GetScrollWheelDelta() = 0;

      // Joysticks / Gamepads
      virtual bool IsGamePadButtonDown() = 0;
      virtual bool IsGamePadButtonPressed() = 0;
      virtual bool IsGamePadButtonReleased() = 0;
      virtual glm::vec2 GetGamePadAxis() = 0;

      // Graphics tablets
      virtual glm::vec2 GetTabletPos() = 0;
      virtual bool IsTabletButtonDown() = 0;
      virtual bool IsTabletButtonPressed() = 0;
      virtual bool IsTabletButtonReleased() = 0;

      // Update
      virtual void Update() = 0;
    private:

    };
  }
}

#endif
