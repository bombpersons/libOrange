#if defined __CYGWIN__ || _WIN32

#include <Orange/priv/win32/Win32InputImpl.hpp>
using namespace orange;
using namespace orange::priv;
using namespace orange::priv::win;

#include <atomic>

#include <Orange/priv/input/InputImpl.hpp>
#include <Orange/priv/window/WindowImpl.hpp>
#include <Orange/threads/Thread.hpp>
#include <Orange/logging/Logging.hpp>
#include <glm/glm.hpp>

#include <Windows.h>

// Load a standard us keyboard layout
HKL QuertyLayout = LoadKeyboardLayout("00000409", KLF_ACTIVATE);

InputCode::Key::Key ConvertVK(unsigned int vkey, unsigned int scanCode, bool e0, bool e1) {
  // Map to virtual keys
  vkey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK, QuertyLayout);

  if (e1) {
    // Correct the virtual key for escaped sequences
    if (vkey == VK_PAUSE)
      scanCode = 0x45; // There's a bug with MapVirtualKey that means
                       // VK_PAUSE doesn't work, so do it manually.
    else
      scanCode = MapVirtualKeyEx(vkey, MAPVK_VK_TO_VSC, QuertyLayout);
  }

  // Catch any ascii keys
  if (vkey >= 0x30 && vkey <= 0x5A) {
    return (InputCode::Key::Key)vkey;
  }

  switch (vkey) {
    case VK_BACK:
      return InputCode::Key::BackSpace;
    case VK_TAB:
      return InputCode::Key::Tab;
    case VK_PAUSE:
      return InputCode::Key::Pause;
    case VK_CAPITAL:
      return InputCode::Key::CapsLock;
    case VK_ESCAPE:
      return InputCode::Key::Escape;
    case VK_SPACE:
      return InputCode::Key::Space;
    case VK_SNAPSHOT:
      return InputCode::Key::PrintScreen;
    case VK_LWIN:
      return InputCode::Key::LSuper;
    case VK_RWIN:
      return InputCode::Key::RSuper;
    case VK_MULTIPLY:
      return InputCode::Key::NumpadMultiply;
    case VK_ADD:
      return InputCode::Key::NumpadAdd;
    case VK_SUBTRACT:
      return InputCode::Key::NumpadSubtract;
    case VK_DIVIDE:
      return InputCode::Key::NumpadDivide;
    case VK_F1:
      return InputCode::Key::F1;
    case VK_F2:
      return InputCode::Key::F2;
    case VK_F3:
      return InputCode::Key::F3;
    case VK_F4:
      return InputCode::Key::F4;
    case VK_F5:
      return InputCode::Key::F5;
    case VK_F6:
      return InputCode::Key::F6;
    case VK_F7:
      return InputCode::Key::F7;
    case VK_F8:
      return InputCode::Key::F8;
    case VK_F9:
      return InputCode::Key::F9;
    case VK_F10:
      return InputCode::Key::F10;
    case VK_F11:
      return InputCode::Key::F11;
    case VK_F12:
      return InputCode::Key::F12;
    case VK_F13:
      return InputCode::Key::F13;
    case VK_F14:
      return InputCode::Key::F14;
    case VK_F15:
      return InputCode::Key::F15;
    case VK_F16:
      return InputCode::Key::F16;
    case VK_F17:
      return InputCode::Key::F17;
    case VK_F18:
      return InputCode::Key::F18;
    case VK_F19:
      return InputCode::Key::F19;
    case VK_F20:
      return InputCode::Key::F20;
    case VK_F21:
      return InputCode::Key::F21;
    case VK_F22:
      return InputCode::Key::F22;
    case VK_F23:
      return InputCode::Key::F23;
    case VK_F24:
      return InputCode::Key::F24;
    case VK_NUMLOCK:
      return InputCode::Key::NumLock;
    case VK_SCROLL:
      return InputCode::Key::ScrollLock;
    case VK_OEM_1:
      return InputCode::Key::SemiColon;
    case VK_OEM_PLUS:
      return InputCode::Key::Add;
    case VK_OEM_COMMA:
      return InputCode::Key::Comma;
    case VK_OEM_MINUS:
      return InputCode::Key::Subtract;
    case VK_OEM_PERIOD:
      return InputCode::Key::Period;
    case VK_OEM_2:
      return InputCode::Key::ForwardSlash;
    case VK_OEM_3:
      return InputCode::Key::Hash;
    case VK_OEM_4:
      return InputCode::Key::LSquareBracket;
    case VK_OEM_5:
      return InputCode::Key::Pipe;
    case VK_OEM_6:
      return InputCode::Key::RSquareBracket;
    case VK_OEM_7:
      return InputCode::Key::Quote;

    // Exceptions
    case VK_SHIFT:
      if (e0)
        return InputCode::Key::RShift;
      else
        return InputCode::Key::LShift;

    case VK_CONTROL:
      if (e0)
        return InputCode::Key::RControl;
      else
        return InputCode::Key::LControl;

    case VK_APPS:
      return InputCode::Key::Menu;

    case VK_RETURN:
      if (e0)
        return InputCode::Key::NumpadReturn;
      else
        return InputCode::Key::Return;

    case VK_INSERT:
      if (e0)
        return InputCode::Key::Insert;
      else
        return InputCode::Key::Numpad0;

    case VK_DELETE:
      if (e0)
        return InputCode::Key::Delete;
      else
        return InputCode::Key::NumpadPeriod;

    case VK_HOME:
      if (e0)
        return InputCode::Key::Home;
      else
        return InputCode::Key::Numpad7;

    case VK_END:
      if (e0)
        return InputCode::Key::End;
      else
        return InputCode::Key::Numpad1;

    case VK_PRIOR:
      if (e0)
        return InputCode::Key::PageUp;
      else
        return InputCode::Key::Numpad9;

    case VK_NEXT:
      if (e0)
        return InputCode::Key::PageDown;
      else
        return InputCode::Key::Numpad3;

    case VK_LEFT:
      if (e0)
        return InputCode::Key::Left;
      else
        return InputCode::Key::Numpad4;

    case VK_RIGHT:
      if (e0)
        return InputCode::Key::Right;
      else
        return InputCode::Key::Numpad6;

    case VK_UP:
      if (e0)
        return InputCode::Key::Up;
      else
        return InputCode::Key::Numpad8;

    case VK_DOWN:
      if (e0)
        return InputCode::Key::Down;
      else
        return InputCode::Key::Numpad2;

    case VK_CLEAR:
      if (!e0)
        return InputCode::Key::Numpad5;
      break;
  }

  return (InputCode::Key::Key)0;
}

// This class will create a thread that polls raw input.

// Constructor
RawInput::RawInput() {
  // Set default values
  hwnd = nullptr;

  // Register a class
  WNDCLASSEX wc;
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = 0;
  wc.lpfnWndProc = RawInput::StaticWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetModuleHandle(NULL);
  wc.hIcon = LoadIcon(NULL, IDC_ARROW);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = TEXT("RawInput");
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  if (RegisterClassEx(&wc))
  {
    // Create a dummy window so that we can use the rawinput api.
    hwnd = CreateWindow(TEXT("RawInput"), TEXT(""), WS_POPUP | WS_DISABLED, 0, 0, 1, 1, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    ShowWindow(hwnd, SW_HIDE);

    // Register the window for raw input
    RAWINPUTDEVICE rid[2];
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x02; // Mouse
    rid[0].dwFlags = RIDEV_INPUTSINK;
    rid[0].hwndTarget = hwnd;

    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x06; // Keyboard
    rid[1].dwFlags = RIDEV_INPUTSINK;
    rid[1].hwndTarget = hwnd;

    // Register the devices
    if (RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE)))
      LOG(Log::DEFAULT) << "Registered RawInput Device.";
    else
      LOG(Log::CRITICAL) << "Error registering RawInput Device! Error: " << GetLastError();

    // Set the userdata on the window
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    // Start the message pump.
    UpdateWindow(hwnd);

    // Update input
    Update();
  } else {
    LOG(Log::FATAL) << "Couldn't register rawinput window class... Error: " << GetLastError();
  }
}

RawInput::~RawInput() {
  // Close the window and whatnot
  if (hwnd) {
    // Close the window
    DestroyWindow(hwnd);
    hwnd = nullptr;
  }
}

// Update the buffers
bool RawInput::Update() {
  // Reset relative mouse movement.
  MouseRelX = 0;
  MouseRelY = 0;
  MouseRelWheel = 0;

  // Handle window messages.
  MSG msg;
  while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return true;
}

// Actual wndproc
LRESULT CALLBACK RawInput::WndProc(HWND _hwnd, UINT _msg, WPARAM _w, LPARAM _l) {
  // Listen for WM_INPUT
  switch (_msg) {
    // We can read raw input here.
    case WM_INPUT:
    {
      // Read data
      UINT sizeOfBuffer = sizeof(InputBuffer);
      if (!GetRawInputData((HRAWINPUT)_l, RID_INPUT, InputBuffer, &sizeOfBuffer, sizeof(RAWINPUTHEADER))) {
        LOG(Log::CRITICAL) << "GetRawInputData failed! Error: " << GetLastError();
        break;
      }

      // Parse this block
      RAWINPUT* raw = (RAWINPUT*)InputBuffer;
      switch (raw->header.dwType) {
        case RIM_TYPEHID:
        {
          // Not reading from any HID devices..
          // We should use this to read data from controllers and stuff

          break;
        }
        case RIM_TYPEKEYBOARD:
        {
          // Sorting out keyboard input
          // Apparently it's more complicated than it seems.
          // More info here: http://molecularmusings.wordpress.com/2011/09/05/properly-handling-keyboard-input/
          unsigned int vkey = raw->data.keyboard.VKey;
          unsigned int scanCode = raw->data.keyboard.MakeCode;
          unsigned int flags = raw->data.keyboard.Flags;

          // Get flags
          const bool e0 = ((flags & RI_KEY_E0) != 0);
          const bool e1 = ((flags & RI_KEY_E1) != 0);

          // Discard "fake" keys
          if (vkey == 255)
            break;

          // Convert to our own format.
          InputCode::Key::Key key = ConvertVK(vkey, scanCode, e0, e1);

          // Set the state.
          if (raw->data.keyboard.Message == WM_KEYUP || raw->data.keyboard.Message == WM_SYSKEYUP) {
            // Key is up
            KeyboardKeys[key] = false;
            LOG(Log::DEFAULT) << "Key Up: " << key << ": " << KeyboardKeys[key];
          } else if (raw->data.keyboard.Message == WM_KEYDOWN || raw->data.keyboard.Message == WM_SYSKEYDOWN) {
            // Key is down
            KeyboardKeys[key] = true;
            LOG(Log::DEFAULT) << "Key Down: " << key << ": " << KeyboardKeys[key];
          }
          break;
        }
        case RIM_TYPEMOUSE:
        {
          // Relative mouse data
          MouseRelX += raw->data.mouse.lLastX;
          MouseRelY += raw->data.mouse.lLastY;

          // Buttons
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
            MouseButtons[InputCode::MouseButton::Left] = true;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
            MouseButtons[InputCode::MouseButton::Left] = false;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
            MouseButtons[InputCode::MouseButton::Middle] = true;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
            MouseButtons[InputCode::MouseButton::Middle] = false;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
            MouseButtons[InputCode::MouseButton::Right] = true;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
            MouseButtons[InputCode::MouseButton::Right] = false;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra1] = true;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra1] = false;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra2] = true;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra2] = false;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra3] = true;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra3] = false;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra4] = true;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra4] = false;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra5] = true;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
            MouseButtons[InputCode::MouseButton::Extra5] = false;
          }
          if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
            MouseRelWheel += raw->data.mouse.usButtonData;
          }
          break;
        }
      }

      break;
    }

    // Do the default for other stuff.
    default:
      return DefWindowProc(_hwnd, _msg, _w, _l);
  }

  return 0;
}

// Wndproc
LRESULT CALLBACK RawInput::StaticWndProc(HWND _hwnd, UINT _msg, WPARAM _w, LPARAM _l) {
  // Send the message to the class.
  RawInput* input = (RawInput*)GetWindowLongPtr(_hwnd, GWLP_USERDATA);
  if (input)
    return input->WndProc(_hwnd, _msg, _w, _l);

  // Couldn't find the window class, but just let windows do the default thing.
  return DefWindowProc(_hwnd, _msg, _w, _l);
}

//
Win32InputImpl::Win32InputImpl(WindowImpl* _impl) {
  window = _impl;
}
Win32InputImpl::~Win32InputImpl() {
}

// Keyboard
bool Win32InputImpl::IsKeyDown(InputCode::Key::Key _key) {
  return keyboardState[_key];
}
bool Win32InputImpl::IsKeyPressed(InputCode::Key::Key _key) {
  return keyboardState[_key] && !oldKeyboardState[_key];
}
bool Win32InputImpl::IsKeyReleased(InputCode::Key::Key _key) {
  return !keyboardState[_key] && oldKeyboardState[_key];
}

// Mouse
glm::vec2 Win32InputImpl::GetMousePos() {
  return mouseAbsolute;
}
glm::vec2 Win32InputImpl::GetRelativeMouseMove() {
  return mouseRelative;
}
bool Win32InputImpl::IsMouseButtonDown(InputCode::MouseButton::MouseButton _button) {
  return mouseButtonState[_button];
}
bool Win32InputImpl::IsMouseButtonPressed(InputCode::MouseButton::MouseButton _button) {
  return mouseButtonState[_button] && !oldMouseButtonState[_button];
}
bool Win32InputImpl::IsMouseButtonReleased(InputCode::MouseButton::MouseButton _button) {
  return !mouseButtonState[_button] && oldMouseButtonState[_button];
}
int Win32InputImpl::GetScrollWheelDelta() {
  return mouseScroll;
}

// Joysticks / Gamepads
bool Win32InputImpl::IsGamePadButtonDown() {return false;}
bool Win32InputImpl::IsGamePadButtonPressed() {return false;}
bool Win32InputImpl::IsGamePadButtonReleased() {return false;}
glm::vec2 Win32InputImpl::GetGamePadAxis() {
  return glm::vec2(0.0f, 0.0f);
}

// Graphics tablets
glm::vec2 Win32InputImpl::GetTabletPos() {
  return glm::vec2(0.0f, 0.0f);
}
bool Win32InputImpl::IsTabletButtonDown() {return false;};
bool Win32InputImpl::IsTabletButtonPressed() {return false;}
bool Win32InputImpl::IsTabletButtonReleased() {return false;}

// Update
void Win32InputImpl::Update() {
  // Get these values before updating since Update() will clear them.
  // Get the mouse scroll
  mouseScroll = rawinput.MouseRelWheel;

  // Get relative mouse movement
  mouseRelative = glm::vec2(rawinput.MouseRelX, rawinput.MouseRelY);

  // Update raw input.
  rawinput.Update();

  // Check if we have focus.
  bool hasFocus = GetForegroundWindow() == window->GetWindowHandle();

  // Record the keys if we have focus
  if (hasFocus) {
    // Copy the keyboard state
    memcpy(oldKeyboardState, keyboardState, sizeof(keyboardState));
    memcpy(keyboardState, rawinput.KeyboardKeys, sizeof(keyboardState));

    // Copy the mouse button state
    memcpy(oldMouseButtonState, mouseButtonState, sizeof(mouseButtonState));
    memcpy(mouseButtonState, rawinput.MouseButtons, sizeof(mouseButtonState));

    // Get the absolute mouse position
    POINT point;
    GetCursorPos(&point);
    ScreenToClient((HWND)window->GetWindowHandle(), &point);

    // If the point is outside of the screen don't record it.
    RECT rect;
    GetClientRect((HWND)window->GetWindowHandle(), &rect);
    if (point.x >= 0 && point.y >= 0 &&
        point.x < rect.right && point.y < rect.bottom) {
      mouseAbsolute.x = (float)point.x;
      mouseAbsolute.y = (float)point.y;
    }
  }
}

#endif
