#ifndef ORANGE_WIN32INPUTIMPL
#define ORANGE_WIN32INPUTIMPL

#include <Windows.h>
#include <glm/fwd.hpp>

#include <Orange/priv/input/InputImpl.hpp>

namespace orange {
  namespace priv {
    namespace win {
      class Win32InputImpl : public InputImpl {
      public:
        Win32InputImpl();
        virtual ~Win32InputImpl();

        // Keyboard
        bool IsKeyDown(InputCode::Key::Key _key);
        bool IsKeyPressed(InputCode::Key::Key _key);
        bool IsKeyReleased(InputCode::Key::Key _key);

        // Mouse
        glm::vec2 GetMousePos();
        bool IsMouseButtonDown(InputCode::MouseButton::MouseButton _button);
        bool IsMouseButtonPressed(InputCode::MouseButton::MouseButton _button);
        bool IsMouseButtonReleased(InputCode::MouseButton::MouseButton _button);
        int GetScrollWheelDelta();

        // Joysticks / Gamepads
        bool IsGamePadButtonDown();
        bool IsGamePadButtonPressed();
        bool IsGamePadButtonReleased();
        glm::vec2 GetGamePadAxis();

        // Graphics tablets
        glm::vec2 GetTabletPos();
        bool IsTabletButtonDown();
        bool IsTabletButtonPressed();
        bool IsTabletButtonReleased();

        // Update
        void Update(float _delta);

      private:
      };
    }
  }
}

#endif
