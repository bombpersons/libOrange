#if defined __linux__

#include <Orange/priv/linux/LinuxWindowImpl.hpp>
#include <Orange/priv/linux/LinuxGLContext.hpp>
#include <Orange/logging/Logging.hpp>

namespace orange {
  namespace priv {
    namespace linux {

      LinuxGLContext::LinuxGLContext(LinuxGLContext* _shared) {
        XLockDisplay(LinuxWindowImpl::GetDisplay());
        window = DefaultRootWindow(LinuxWindowImpl::GetDisplay());
        XUnlockDisplay(LinuxWindowImpl::GetDisplay());

        CreateContext(_shared, Settings());
      }

      LinuxGLContext::LinuxGLContext(LinuxGLContext* _shared, const GLContext::Settings& _settings, const priv::WindowImpl* _window) {
        window = (::Window)(_window->GetWindowHandle());

        CreateContext(_shared, _settings);
      }

      LinuxGLContext::LinuxGLContext(LinuxGLContext* _shared, const GLContext::Settings& _settings, unsigned int _width, unsigned int _height) {
        CreateContext(_shared, _settings);
      }

      LinuxGLContext::~LinuxGLContext() {
      }

      // Display / Swapbuffers / Present / whatever you want to call it.
      void LinuxGLContext::Display() {
        if (window) {
          XLockDisplay(LinuxWindowImpl::GetDisplay());
          glXSwapBuffers(LinuxWindowImpl::GetDisplay(), window);
          XUnlockDisplay(LinuxWindowImpl::GetDisplay());
        }
      }

      void LinuxGLContext::SetVSync(bool _v) {
        LOG(Log::DEFAULT) << "LinuxGLContext::SetVSync() - Stub";
        // Enable / Disable vertical sync
      }

      // Make this context current
      bool LinuxGLContext::MakeCurrent() {
        if (window && context) {
          XLockDisplay(LinuxWindowImpl::GetDisplay());
          glXMakeCurrent(LinuxWindowImpl::GetDisplay(), window, context);
          XUnlockDisplay(LinuxWindowImpl::GetDisplay());
          return true;
        }

        return false;
      }

      // Create a context
      void LinuxGLContext::CreateContext(LinuxGLContext* _shared, const GLContext::Settings& _settings) {

        // Lock the display to this thread.
        XLockDisplay(LinuxWindowImpl::GetDisplay());

        int screenId = XDefaultScreen(LinuxWindowImpl::GetDisplay());

        // Make sure we have glx extensions loaded.
        gladLoadGLX(LinuxWindowImpl::GetDisplay(), screenId);

        // If we have already loaded extension functions, make a fancy modern context.
        int context_attribs[] = {
          GLX_CONTEXT_MAJOR_VERSION_ARB, (int)_settings.MajorVersion,
          GLX_CONTEXT_MINOR_VERSION_ARB, (int)_settings.MinorVersion,
          GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
          None
        };

        GLXFBConfig fbc = LinuxWindowImpl::GetBestFBConfig(LinuxWindowImpl::GetDisplay(), screenId, _settings);

        if (glXCreateContextAttribsARB) {
          if (_shared)
            context = glXCreateContextAttribsARB(LinuxWindowImpl::GetDisplay(), fbc, _shared->context, true, context_attribs);
          else
            context = glXCreateContextAttribsARB(LinuxWindowImpl::GetDisplay(), fbc, nullptr, true, context_attribs);

        } else {
          if (_shared)
            context = glXCreateNewContext(LinuxWindowImpl::GetDisplay(), fbc, GLX_RGBA_TYPE, _shared->context, True);
          else
            context = glXCreateNewContext(LinuxWindowImpl::GetDisplay(), fbc, GLX_RGBA_TYPE, 0, True);
        }

        XSync(LinuxWindowImpl::GetDisplay(), False);

        // Unlock the display from this thread.
        XUnlockDisplay(LinuxWindowImpl::GetDisplay());

      }

    }
  }
}

#endif
