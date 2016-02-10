#ifndef ORANGE_LINUXGLCONTEXT
#define ORANGE_LINUXGLCONTEXT

#include <GL/glew.h>
#include <GL/glxew.h>

#include <X11/Xlib.h>

#include <Orange/gl/GLContext.hpp>

namespace orange {
  namespace priv {
    class WindowImpl;
    namespace linux {
      class LinuxGLContext : public GLContext {
      public:
        LinuxGLContext(LinuxGLContext* _shared);
        LinuxGLContext(LinuxGLContext* _shared, const GLContext::Settings& _settings, const priv::WindowImpl* _window);
        LinuxGLContext(LinuxGLContext* _shared, const GLContext::Settings& _settings, unsigned int _width, unsigned int _height);
        virtual ~LinuxGLContext();

        // Display / Swapbuffers / Present / whatever you want to call it.
        virtual void Display();

        // Enable / Disable vertical sync
        virtual void SetVSync(bool _v);

        // Make this context current
        virtual bool MakeCurrent();

      private:
        
        // Create a context.
        void CreateContext(LinuxGLContext* _shared, const GLContext::Settings& _settings);
      };

    }
  }
}

#endif
