#ifndef ORANGE_LINUXINPUTIMPL
#define ORANGE_LINUXINPUTIMPL

#include <Orange/priv/window/WindowImpl.hpp>
#include <Orange/priv/input/InputImpl.hpp>

#include <map>

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>

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

        // Display and dummy window used to recieve raw input messages.
        ::Display* display;
        ::Window dummyWindow;
        int xinputOpcode;

        // Store mappings of hardware ids.
        void UpdateDevices();

        struct MouseDevice {
          MouseDevice() {
            ScrollX.number = -1;
            ScrollY.number = -1;
            MotionX.number = -1;
            MotionY.number = -1;
            AbsoluteX.number = -1;
            AbsoluteY.number = -1;
          }
          XIScrollClassInfo ScrollX, ScrollY;
          XIValuatorClassInfo MotionX, MotionY, AbsoluteX, AbsoluteY;
        };
        std::map<int, MouseDevice> mousemapping;

        // Keyboard
        bool KeyboardKeys[InputCode::Key::Count] = {};
        bool OldKeyboardKeys[InputCode::Key::Count] = {};

        // Mouse
        double MouseRelX;
        double MouseRelY;
        double MouseAbsoluteX;
        double MouseAbsoluteY;
        double MouseRelWheelX;
        double MouseRelWheelY;

        bool MouseButtons[InputCode::MouseButton::Count];
        bool OldMouseButtons[InputCode::MouseButton::Count];
      };

    }
  }
}

#endif
