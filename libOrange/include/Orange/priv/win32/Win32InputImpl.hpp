#ifndef ORANGE_WIN32INPUTIMPL
#define ORANGE_WIN32INPUTIMPL

#include <Windows.h>
#include <glm/glm.hpp>

#include <Orange/priv/window/WindowImpl.hpp>
#include <Orange/priv/input/InputImpl.hpp>

namespace orange {
  namespace priv {
    namespace win {
      // This class will create a thread that polls raw input.
      class RawInput {
      public:
        // Constructor
        RawInput();
        ~RawInput();

        // Update the buffers
        bool Update();

      private:
        // Actual wndproc
        LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _w, LPARAM _l);

        // Wndproc
        static LRESULT CALLBACK StaticWndProc(HWND _hwnd, UINT _msg, WPARAM _w, LPARAM _l);

      public:
        // Keyboard
        bool KeyboardKeys[InputCode::Key::Count] = {};

        // Mouse
        int MouseRelX;
        int MouseRelY;
        int MouseRelWheel;
        bool MouseButtons[InputCode::MouseButton::Count];


      private:
        // Vars
        HWND hwnd;
        char InputBuffer[1024];
      };

      class Win32InputImpl : public InputImpl {
      public:
        Win32InputImpl(WindowImpl* _impl);
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
        void Update();

      private:
        RawInput rawinput;
        WindowImpl* window;

        // Keyboard buffer
        bool keyboardState[InputCode::Key::Count] = {};

        // Mouse
        glm::vec2 mouseAbsolute;
        bool mouseButtonState[InputCode::MouseButton::Count] = {};
      };
    }
  }
}

#endif
