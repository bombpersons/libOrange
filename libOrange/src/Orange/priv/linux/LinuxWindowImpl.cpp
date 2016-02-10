#if defined __linux__

#include <Orange/priv/linux/LinuxWindowImpl.hpp>
#include <Orange/logging/Logging.hpp>

namespace orange {
  namespace priv {
    namespace linux {

      // Constructors
      LinuxWindowImpl::LinuxWindowImpl() {

      }
      LinuxWindowImpl::~LinuxWindowImpl() {
        if (display) {
          XCloseDisplay(display);
        }
      }

      // Create the window
      bool LinuxWindowImpl::Setup(int _width, int _height, int _depth, bool _fullscreen) {
        width = _width;
        height = _height;
        depth = _depth;
        fullscreen = _fullscreen;

        // Start the thread.
      	StartThread();

      	// Wait for the hwnd to be ready.
      	WaitForWait();

        return true;
      }

      bool LinuxWindowImpl::RunThread() {
        Thread::RunThread();

        display = XOpenDisplay(nullptr);
        if (!display) {
          LOG(Log::FATAL) << "Couldn't open display!";
          return false;
        }

        int screen = DefaultScreen(display);
        window = XCreateSimpleWindow(display, // The display the window is on.
                                     RootWindow(display, screen), // The parent window.
                                     0, 0, // The x and y position.
                                     width, height, // The width and height of the window.
                                     0, // Border width.
                                     BlackPixel(display, screen), // The border color.
                                     WhitePixel(display, screen)); // The background color.

        XSelectInput(display, window, ExposureMask | KeyPressMask);
        XMapWindow(display, window);

        Wait();

        while (true) {
          ::XEvent event;
          XNextEvent(display, &event);
          if (event.type == KeyPress) {
            break;
          }
        }
      }

      // Opens the window
      void LinuxWindowImpl::Run() {
        // Signal the thread to continue.
      	Signal();
      }

      // Returns true if the window is open
      bool LinuxWindowImpl::IsOpen() const {
	      return Thread::IsThreadRunning();
      }

      // Get the platform specific handle
      void* LinuxWindowImpl::GetWindowHandle() const {
        return (void*)window;
      }

      // Set the window title
      void LinuxWindowImpl::SetTitle(const char* _title) {

      }

      // Get width and height
      int LinuxWindowImpl::GetWidth() {
        return 0;
      }
      int LinuxWindowImpl::GetHeight() {
        return 0;
      }

    }
  }
}

#endif
