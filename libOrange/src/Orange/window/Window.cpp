#include <Orange/window/Window.hpp>
using namespace orange;

Window::Window() {
	window = 0;
	context = 0;
	input = 0;
}

Window::Window(int _width, int _height, int _depth, bool _fullscreen, GLContext::Settings _settings) {
	window = 0;
	context = 0;
	input = 0;
	Create(_width, _height, _depth, _fullscreen, _settings);
}

Window::~Window() {
	// Delete the implementation if it exists
	if (window)
		delete window;
	window = 0;

	// Delete the context if it exists
	if (context)
		delete context;
	context = 0;

	// Delete the input if it exists
	if (input)
		delete input;
	input = 0;
}

// Get the inputimpl
priv::InputImpl* Window::Input() {
	return input;
}

void Window::Run() {
	if (window)
		window->Run();
}

bool Window::IsOpen() {
	if (window)
		return window->IsOpen();

	return false;
}

bool Window::Update() {
	// Update the input
	input->Update();

	// Return whether or not we are open.
	return IsOpen();
}

void Window::Display() {
	// Set our context to be active
	if (context) {
		context->SetActive(true);
		context->Display();
	}
}

void Window::SetTitle(const char* _title) {
	if (window)
		window->SetTitle(_title);
}

int Window::GetWidth() {
	if (window)
		return window->GetWidth();
	return 0;
}

int Window::GetHeight() {
	if (window)
		return window->GetHeight();
	return 0;
}

bool Window::Create(int _width, int _height, int _depth, bool _fullscreen, GLContext::Settings _settings) {
	// Delete the old window if it exists
	if (window)
		delete window;
	if (context)
		delete context;

	// Create new window.
	window = priv::WindowImpl::Create(_width, _height, _depth, _fullscreen, _settings);
	if (!window)
		return false;

	// Create a new context
	context = GLContext::Create(_settings, window);
	if (!context)
		return false;
	context->Init();

	// Create an input
	input = priv::InputImpl::Create(window);
	if (!input)
		return false;

	return true;
}
