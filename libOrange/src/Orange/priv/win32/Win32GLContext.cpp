#include <GL/glew.h>
#include <GL/wglew.h>

#include <Orange/priv/win32/Win32GLContext.hpp>
using namespace orange::priv::win;

#include <Orange/logging/Logging.hpp>
#include <Orange/priv/window/WindowImpl.hpp>

#include <thread>
#include <mutex>

Win32GLContext::Win32GLContext(Win32GLContext* _shared) {
  window = nullptr;
  dc = nullptr;
  context = nullptr;
  ownsWindow = true;

  LOG(Log::DEFAULT) << "Creating opengl context...";	

  // We need to create a dummy window because windows
  // doesn't let you create a context without a window =/
  window = CreateWindow("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, 1, 1, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
  ShowWindow(window, SW_HIDE);
  dc = GetDC(window);

  // Create context
  if (dc)
    CreateContext(_shared, Settings());
}
Win32GLContext::Win32GLContext(Win32GLContext* _shared, const GLContext::Settings& _settings, const priv::WindowImpl* _window) {
  window = nullptr;
  dc = nullptr;
  context = nullptr;
  ownsWindow = false;

  LOG(Log::DEFAULT) << "Creating opengl context...";	

  // Get the window info
  window = (HWND)(_window->GetWindowHandle());
  dc = GetDC(window);

  // Create context
  if (dc)
    CreateContext(_shared, _settings);
}
Win32GLContext::Win32GLContext(Win32GLContext* _shared, const GLContext::Settings& _settings, unsigned int _width, unsigned int _height) {
  window = nullptr;
  dc = nullptr;
  context = nullptr;
  ownsWindow = true;

  LOG(Log::DEFAULT) << "Creating opengl context...";	

  // We need to create a dummy window because windows
  // doesn't let you create a context without a window =/
  window = CreateWindow("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, _width, _height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
  ShowWindow(window, SW_HIDE);
  dc = GetDC(window);

  // Create context
  if (dc)
    CreateContext(_shared, _settings);
}

Win32GLContext::~Win32GLContext() {
  // Destroy the context and window if we own it.
  if (context) {
    if (wglGetCurrentContext() == context)
      wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(context);
  }

  if (dc) {
    ReleaseDC(window, dc);
    dc = nullptr;
  }

  if (window && ownsWindow) {
    DestroyWindow(window);
    window = nullptr;
  }
}

// Create a context.
void Win32GLContext::CreateContext(Win32GLContext* _shared, const GLContext::Settings& _settings) {
  // Save the settings.
  settings = _settings;

  // Make sure we have gotten the functions we need from glew.
  if (wglChoosePixelFormatARB && wglCreateContextAttribsARB) {
    // Find a pixel format
    float fattributes[] = {0,0};
    int pixelAttribs[] {
      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
      WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
      WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
      WGL_SAMPLE_BUFFERS_ARB, settings.AntialiasingLevel != 0,
      WGL_SAMPLES_ARB, (int)settings.AntialiasingLevel,
      WGL_COLOR_BITS_ARB, (int)settings.ColorBits,
      WGL_ALPHA_BITS_ARB, (int)settings.AlphaBits,
      WGL_DEPTH_BITS_ARB, (int)settings.DepthBits,
      WGL_STENCIL_BITS_ARB, (int)settings.StencilBits,
      0, 0
    };
    unsigned int numformats = 0;
    int pixelformat = 0;
    bool valid = wglChoosePixelFormatARB(dc, pixelAttribs, fattributes, 1, &pixelformat, &numformats);
    if (!valid) {
      LOG(Log::FATAL) << "Couldn't find a pixel format matching requirements! Failed to create OpenGl context!";
      return;
    }

    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    DescribePixelFormat(dc, pixelformat, sizeof(pfd), &pfd);
    if (!SetPixelFormat(dc, pixelformat, &pfd)) {
      LOG(Log::FATAL) << "Couldn't set pixel format. Failed to create OpenGL context!";
      return;
    }

    // Create the context
    int contextAttribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, (int)settings.MajorVersion,
      WGL_CONTEXT_MINOR_VERSION_ARB, (int)settings.MinorVersion,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0, 0
    };
    if (_shared)
      context = wglCreateContextAttribsARB(dc, _shared->context, contextAttribs);
    else
      context = wglCreateContextAttribsARB(dc, nullptr, contextAttribs);
  }

  if (!context) {
    // No context could be created..
    // Try making a 2.0 context?

    // Pick a pixel format
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1.0;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = settings.ColorBits;
    pfd.cDepthBits = settings.DepthBits;
    pfd.cStencilBits = settings.StencilBits;
    pfd.cAlphaBits = settings.AlphaBits;
    int pf = ChoosePixelFormat(dc, &pfd);
    if (!pf) {
      LOG(Log::FATAL) << "Failed to pick a pixel format!";
      return;
    }

    // Set the pixel format
    SetPixelFormat(dc, pf, &pfd);

    // Create the context.
    context = wglCreateContext(dc);
    if (!context) {
      LOG(Log::FATAL) << "Failed to create OpenGL context!";
      return;
    }

    if (_shared) {
      static std::mutex mutex;
      std::lock_guard<std::mutex> lock(mutex);

      if (!wglShareLists(_shared->context, context)) {
        LOG(Log::FATAL) << "Failed to share OpenGL context!";
        return;
      }
    }
  }
}

// Display / Swapbuffers / Present / whatever you want to call it.
void Win32GLContext::Display() {
  if (dc && context)
    SwapBuffers(dc);
}

// Enable / Disable vertical sync
void Win32GLContext::SetVSync(bool _v) {
  wglSwapIntervalEXT(_v ? 1 : 0);
}

// Make this context current
bool Win32GLContext::MakeCurrent() {
  if (dc && context) {
    return wglMakeCurrent(dc, context);
  }

  return false;
}
