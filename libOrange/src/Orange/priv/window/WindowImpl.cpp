#include <Orange/priv/window/WindowImpl.hpp>
using namespace orange::priv;

// Depending on the system we are compiling on, use different implementations of this class
#if defined __CYGWIN__ || defined _WIN32
#include <Orange/priv/win32/Win32WindowImpl.hpp>
typedef win::Win32WindowImpl WindowImplType;
#endif

WindowImpl::WindowImpl() {

}

WindowImpl::~WindowImpl() {

}

WindowImpl* WindowImpl::Create(int _width, int _height, int _depth, bool _fullscreen) {
	WindowImplType* window = new WindowImplType();
	if (window->Create(_width, _height, _depth, _fullscreen))
		return window;

	delete window;
	return NULL;
}
