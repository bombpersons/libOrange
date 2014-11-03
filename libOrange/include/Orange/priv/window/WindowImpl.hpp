#ifndef ORANGE_WINDOWIMPL
#define ORANGE_WINDOWIMPL

#include <Orange/util/NoCopy.hpp>

namespace orange {
	namespace priv {
		class WindowImpl : NoCopy {
		public:
			// Constructors
			WindowImpl();
			virtual ~WindowImpl();

			// Opens the window
			virtual void Run() = 0;

			// Returns true if the window is open
			virtual bool IsOpen() const = 0;

			// Get the platform specific handle
			virtual void* GetWindowHandle() const = 0;

			// Set the window title
			virtual void SetTitle(const char* _title) = 0;

			// Change the width, height, depth or fullscreen flags
			static WindowImpl* Create(int _width, int _height, int _depth, bool _fullscreen);

		private:

		};
	}
}

#endif
