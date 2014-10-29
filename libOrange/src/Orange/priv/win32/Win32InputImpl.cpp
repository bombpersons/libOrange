#include <Orange/priv/win32/Win32InputImpl.hpp>
using namespace orange;
using namespace orange::priv;
using namespace orange::priv::win;

#include <Orange/priv/input/InputImpl.hpp>
#include <Orange/priv/window/WindowImpl.hpp>
#include <Orange/threads/Thread.hpp>
#include <Orange/logging/Logging.hpp>
#include <glm/glm.hpp>

#include <Windows.h>

// This class will create a thread that polls raw input.
class StaticRawInput : public Thread {
public:
  // Singleton accessor
  static StaticRawInput& Get() {
    return input;
  }

  // Constructor
  StaticRawInput() {
    // Start the input thread.
    StartThread();
  }

  bool RunThread() {
    Thread::RunThread();

    // Register a class
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = StaticRawInput::StaticWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDC_ARROW);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = TEXT("RawInput");
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
      LOG(Log::FATAL) << "Couldn't register rawinput window class... Error: " << GetLastError();
      return false;
    }

    // Create a dummy window so that we can use the rawinput api.
    hwnd = CreateWindow(TEXT("RawInput"), TEXT(""), WS_POPUP | WS_DISABLED, 0, 0, 1, 1, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    ShowWindow(hwnd, SW_HIDE);

    // Register the window for raw input
    RAWINPUTDEVICE rid[2];
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x02; // Mouse
    rid[0].dwFlags = 0;
    rid[0].hwndTarget = hwnd;

    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x06; // Keyboard
    rid[1].dwFlags = 0;
    rid[1].hwndTarget = hwnd;

    // Register the devices
    if (RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE)))
      LOG(Log::DEFAULT) << "Registered RawInput Device.";
    else
      LOG(Log::CRITICAL) << "Error registering RawInput Device! Error: " << GetLastError();

    // Set the userdata on the window
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)this);

    // Start the message pump.
    UpdateWindow(hwnd);

    // Handle window messages.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    return true;
  }

private:
  // Actual wndproc
  LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _w, LPARAM _l) {
    // Listen for WM_INPUT
    switch (_msg) {
      // We can read raw input here.
      case WM_INPUT:
        // Okay, we need to read into some sort of buffer so that other
        // threads can access the input without having to do locking or anything.
        

        break;

      // Do the default for other stuff.
      default:
        return DefWindowProc(_hwnd, _msg, _w, _l);
    }
  }

  // Wndproc
  static LRESULT CALLBACK StaticWndProc(HWND _hwnd, UINT _msg, WPARAM _w, LPARAM _l) {
    // Send the message to the class.
    StaticRawInput* input = (StaticRawInput*)GetWindowLongPtr(_hwnd, GWLP_USERDATA);
    if (input)
      return input->WndProc(_hwnd, _msg, _w, _l);

    // Couldn't find the window class, but just let windows do the default thing.
    return DefWindowProc(_hwnd, _msg, _w, _l);
  }

private:
  // Vars
  HWND hwnd;

  // Singleton
  static StaticRawInput input;
};
StaticRawInput StaticRawInput::input;

//
Win32InputImpl::Win32InputImpl() {
}
Win32InputImpl::~Win32InputImpl() {
}

// Keyboard
bool Win32InputImpl::IsKeyDown(InputCode::Key::Key _key) {

}
bool Win32InputImpl::IsKeyPressed(InputCode::Key::Key _key) {

}
bool Win32InputImpl::IsKeyReleased(InputCode::Key::Key _key) {

}

// Mouse
glm::vec2 Win32InputImpl::GetMousePos() {

}
bool Win32InputImpl::IsMouseButtonDown(InputCode::MouseButton::MouseButton _button) {

}
bool Win32InputImpl::IsMouseButtonPressed(InputCode::MouseButton::MouseButton _button) {

}
bool Win32InputImpl::IsMouseButtonReleased(InputCode::MouseButton::MouseButton _button) {

}
int Win32InputImpl::GetScrollWheelDelta() {

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
void Win32InputImpl::Update(float _delta) {
}
