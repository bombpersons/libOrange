#ifndef ORANGE_WIN32GLCONTEXT
#define ORANGE_WIN32GLCONTEXT

#include <Orange/gl/GLContext.hpp>
#include <Windows.h>

namespace orange {
  namespace priv {
    class WindowImpl;
    namespace win {
      class Win32GLContext : public GLContext {
      public:
        Win32GLContext(Win32GLContext* _shared);
        Win32GLContext(Win32GLContext* _shared, const GLContext::Settings& _settings, const priv::WindowImpl* _window);
        Win32GLContext(Win32GLContext* _shared, const GLContext::Settings& _settings, unsigned int _width, unsigned int _height);
        virtual ~Win32GLContext();

        // Display / Swapbuffers / Present / whatever you want to call it.
        virtual void Display();

        // Enable / Disable vertical sync
        virtual void SetVSync(bool _v);

        // Make this context current
        virtual bool MakeCurrent();

      private:
        // Create a context.
        void CreateContext(Win32GLContext* _shared, const GLContext::Settings& _settings);

        // If we have created a dummy window, store it here.
        HWND window;
        bool ownsWindow;

        // The actual context!
        HGLRC context;
        HDC dc;
      };
    }
  }
}

#endif
