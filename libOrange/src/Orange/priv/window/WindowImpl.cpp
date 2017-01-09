#include <Orange/priv/window/WindowImpl.hpp>
using namespace orange::priv;

// Depending on the system we are compiling on, use different implementations of this class
#if defined __CYGWIN__ || defined _WIN32
#include <Orange/priv/win32/Win32WindowImpl.hpp>
typedef win::Win32WindowImpl WindowImplType;
#elif defined __linux__
#include <Orange/priv/linux/LinuxWindowImpl.hpp>
typedef linux::LinuxWindowImpl WindowImplType;
#endif

WindowImpl::WindowImpl() {

}

WindowImpl::~WindowImpl() {

}

WindowImpl* WindowImpl::Create(int _width, int _height, int _depth, bool _fullscreen, const GLContext::Settings& _settings) {
	WindowImplType* window = new WindowImplType();
	if (window->Setup(_width, _height, _depth, _fullscreen, _settings))
		return window;

	delete window;
	return nullptr;
}
