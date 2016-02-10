#ifndef ORANGE_LINUXINPUTIMPL
#define ORANGE_LINUXINPUTIMPL

#include <Orange/priv/window/WindowImpl.hpp>
#include <Orange/priv/input/InputImpl.hpp>

namespace orange {
  namespace priv {
    namespace linux {

      class LinuxInputImpl : public InputImpl {
      public:
        LinuxInputImpl(WindowImpl* _impl);
        virtual ~LinuxInputImpl();

        // Keyboard
        bool IsKeyDown(InputCode::Key::Key _key);
        bool IsKeyPressed(InputCode::Key::Key _key);
        bool IsKeyReleased(InputCode::Key::Key _key);

        // Mouse
        glm::vec2 GetMousePos();
        glm::vec2 GetRelativeMouseMove();
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
        void Update();

      private:
        WindowImpl* window;
      };

    }
  }
}

#endif
