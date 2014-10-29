#if defined __CYGWIN__ || defined _WIN32
#include <Orange/priv/win32/Win32WindowImpl.hpp>
using namespace orange::priv::win;

#include <Orange/logging/Logging.hpp>
#include <Orange/threads/Thread.hpp>

Win32WindowImpl::Win32WindowImpl() {
	hwnd = 0;
	isOpen = false;
}

Win32WindowImpl::~Win32WindowImpl() {
}

bool Win32WindowImpl::Create(int _width, int _height, int _depth, bool _fullscreen) {
	width = _width;
	height = _height;
	depth = _depth;
	fullscreen = _fullscreen;

	// Create the window class
	RegisterWindowClass();

	// Start the thread.
	StartThread();

	// Wait for the hwnd to be ready.
	WaitForWait();

	return true;
}

bool Win32WindowImpl::RunThread() {
	Thread::RunThread();

	if (fullscreen) {
		// Create the window at full screen resolution and without borders
		hwnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			windowClassName,
			TEXT("Win32 Window"),
			WS_POPUP,
			0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			NULL, NULL, GetModuleHandle(NULL), NULL);
	} else {
		// Create the window with the specified width and height.
		hwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			windowClassName,
			TEXT("Win32 Window"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			NULL, NULL, GetModuleHandle(NULL), NULL);
	}

	if (hwnd == NULL) {
		LOG(Log::FATAL) << "Creating window failed.. Win32 error number " << GetLastError();
		return false;
	}

	// Finished creating the window.
	Wait();

	// Set the window long pointer to the this pointer so that the static wndproc can call the member function
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)this);

	// Show and update the window.
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Handle window messages.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Window is no longer open.
	return true;
}

void Win32WindowImpl::Run() {
	// Signal the thread to continue.
	Signal();

	// Window is open now
	isOpen = true;
}

// Returns true if the window is open
bool Win32WindowImpl::IsOpen() const {
	return Thread::IsThreadRunning();
}

// Return the hwnd
void* Win32WindowImpl::GetWindowHandle() const {
	return (void*)hwnd;
}

bool Win32WindowImpl::RegisterWindowClass() {
	static bool classRegistered = false;
	if (!classRegistered) {
		// Register the window class.
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = Win32WindowImpl::StaticWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon(NULL, IDC_ARROW);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = windowClassName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
		{
			LOG(Log::FATAL) << "Couldn't register win32 window class.. Win32 error number " << GetLastError();
			return false;
		}

		LOG(Log::DEFAULT) << "Registered win32 window class.";
		classRegistered = true;
		return true;
	}

	LOG(Log::WARNING) << "Win32 window class has already been registered.";
	return true;
}

LRESULT CALLBACK Win32WindowImpl::WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) {
    switch(_msg)
    {
        case WM_CLOSE:
            DestroyWindow(_hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
    }
    return 0;
}

LRESULT CALLBACK Win32WindowImpl::StaticWndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) {
	// Get the instance of the window class
	Win32WindowImpl* window = (Win32WindowImpl*)GetWindowLongPtr(_hwnd, GWLP_USERDATA);
	if (window)
		return window->WndProc(_hwnd, _msg, _wParam, _lParam);

	// Couldn't find the window class, but just let windows do the default thing.
	return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}

LPCTSTR Win32WindowImpl::windowClassName = TEXT("OrangeWin32Class");

#endif
