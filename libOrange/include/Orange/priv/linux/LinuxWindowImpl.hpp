#ifndef ORANGE_LINUXWINDOWIMPL
#define ORANGE_LINUXWINDOWIMPL

#include <Orange/priv/window/WindowImpl.hpp>
#include <Orange/priv/linux/LinuxGLContext.hpp>
#include <Orange/threads/Thread.hpp>

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>

namespace orange {
	namespace priv {
		namespace linux {
			class LinuxWindowImpl : public WindowImpl, public Thread {
				friend class LinuxGLContext;
			public:
				static ::GLXFBConfig GetBestFBConfig(::Display* display, int screenId, const GLContext::Settings& _settings);
				static ::XVisualInfo* GetVisualInfoFromSettings(::Display* display, int screenId, const GLContext::Settings& _settings);
				static ::Display* GetDisplay();

      	static xcb_connection_t* GetXCBConnection();

			public:
				LinuxWindowImpl();
				virtual ~LinuxWindowImpl();

				// Create the window
				virtual bool Setup(int _width, int _height, int _depth, bool _fullscreen, const GLContext::Settings& _settings);

				// Called by thread
				virtual bool RunThread();

				// Starts the window
				virtual void Run();

				// Returns true if the window is open
				virtual bool IsOpen() const;

				// Get the window handle (in this case the HWND)
				virtual void* GetWindowHandle() const;

				// Set the window title
				virtual void SetTitle(const char* _title);

				// Get the width and height
				virtual int GetWidth();
				virtual int GetHeight();

			private:
				int width, height, depth;
				bool fullscreen;
				GLContext::Settings settings;

				bool running;

				xcb_window_t window;
			};

		}
	}
}

#endif
