#if defined __linux__

#include <Orange/priv/linux/LinuxGLContext.hpp>
#include <Orange/logging/Logging.hpp>

namespace orange {
  namespace priv {
    namespace linux {

      LinuxGLContext::LinuxGLContext(LinuxGLContext* _shared) {
        ::Display* display = nullptr;

        CreateContext(_shared, Settings());
      }

      LinuxGLContext::LinuxGLContext(LinuxGLContext* _shared, const GLContext::Settings& _settings, const priv::WindowImpl* _window) {
        CreateContext(_shared, _settings);
      }

      LinuxGLContext::LinuxGLContext(LinuxGLContext* _shared, const GLContext::Settings& _settings, unsigned int _width, unsigned int _height) {
        CreateContext(_shared, _settings);
      }

      LinuxGLContext::~LinuxGLContext() {
      }

      // Display / Swapbuffers / Present / whatever you want to call it.
      void LinuxGLContext::Display() {
        LOG(Log::DEFAULT) << "LinuxGLContext::Display() - Stub";
      }

      void LinuxGLContext::SetVSync(bool _v) {
        LOG(Log::DEFAULT) << "LinuxGLContext::SetVSync() - Stub";
        // Enable / Disable vertical sync
      }

      // Make this context current
      bool LinuxGLContext::MakeCurrent() {
        LOG(Log::DEFAULT) << "LinuxGLContext::MakeCurrent() - Stub";
        return false;
      }

      // Create a context
      void LinuxGLContext::CreateContext(LinuxGLContext* _shared, const GLContext::Settings& _settings) {
        LOG(Log::DEFAULT) << "LinuxGLContext::CreateContext() - Stub";
      }

    }
  }
}

#endif
