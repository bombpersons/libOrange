#ifndef ORANGE_WINDOW
#define ORANGE_WINDOW

#include <Orange/priv/input/InputImpl.hpp>
#include <Orange/priv/window/WindowImpl.hpp>
#include <Orange/gl/GLContext.hpp>
#include <Orange/gl/GLResource.hpp>

namespace orange {
	class Window : public GLResource {
	public:
		Window();
		Window(int _width, int _height, int _depth, bool _fullscreen);
		virtual ~Window();

		// Get the inputimpl
		priv::InputImpl* Input();

		// Starts the windows message loop
		void Run();

		// Returns true if the window is open
		bool IsOpen();

		// Update
		bool Update();

		// Display the window
		void Display();

		// Set the window title
		void SetTitle(const char* _title);

		// Create or Recreate the window.
		bool Create(int _width, int _height, int _depth, bool _fullscreen, GLContext::Settings _settings=GLContext::Settings());

	private:
		priv::WindowImpl* window;
		GLContext* context;
		priv::InputImpl* input;
	};
}
#endif
