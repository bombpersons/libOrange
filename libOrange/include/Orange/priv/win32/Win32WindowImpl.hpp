#ifndef ORANGE_WIN32_WINDOWIMPL
#define ORANGE_WIN32_WINDOWIMPL

#if defined __CYGWIN__ || defined _WIN32
#include <Windows.h>
#include <Orange/priv/window/WindowImpl.hpp>
#include <Orange/threads/Thread.hpp>

namespace orange {
	namespace priv {
		namespace win {
			class Win32WindowImpl : public WindowImpl, public Thread {
			public:
				Win32WindowImpl();
				virtual ~Win32WindowImpl();

				// Create the window
				virtual bool Create(int _width, int _height, int _depth, bool _fullscreen);

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
				LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

				// Window handle.
				HWND hwnd;

				// Variables
				bool isOpen;

				// We need to store these variables so that when Run is called we can create the window
				int width, height, depth;
				bool fullscreen;

				// Register window class
				static bool RegisterWindowClass();

				static LRESULT CALLBACK StaticWndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
				static LPCTSTR windowClassName;
			};

		}
	}
}

#endif

#endif
