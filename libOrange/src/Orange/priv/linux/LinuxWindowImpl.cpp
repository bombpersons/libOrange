#if defined __linux__

#include <Orange/priv/linux/LinuxWindowImpl.hpp>
#include <Orange/logging/Logging.hpp>

namespace orange {
  namespace priv {
    namespace linux {

      // This bodge forces XLib to be compatible with threading.
      // XInitThreads needs to be called before any other Xlib function,
      // so let's do it in a function initializing a global variable to make
      // sure that it's first.
      bool InitXLibThreadingSupport() {
        static bool initThreads = false;
        if (!initThreads) {
          if (!XInitThreads()) {
            LOG(Log::CRITICAL) << "Couldn't initialize multithreading support for xlib!";
          }
          initThreads = true;
        }

        return true;
      }
      bool forceXLibThreadingSupport = InitXLibThreadingSupport();

      // Static
		  ::GLXFBConfig LinuxWindowImpl::GetBestFBConfig(::Display* display, int screenId, const GLContext::Settings& _settings) {
        GLint glxAttribs[] = {
          GLX_X_RENDERABLE, True,
          GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
          GLX_RENDER_TYPE, GLX_RGBA_BIT,
          GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
          GLX_RED_SIZE, (GLint)(_settings.ColorBits/3),
          GLX_GREEN_SIZE, (GLint)(_settings.ColorBits/3),
          GLX_BLUE_SIZE, (GLint)(_settings.ColorBits/3),
          GLX_ALPHA_SIZE, (GLint)(_settings.AlphaBits),
          GLX_DEPTH_SIZE, (GLint)(_settings.DepthBits),
          GLX_STENCIL_SIZE, (GLint)(_settings.StencilBits),
          GLX_DOUBLEBUFFER, True,
          GLX_SAMPLE_BUFFERS, _settings.AntialiasingLevel > 0 ? : 1,
          GLX_SAMPLES, (GLint)_settings.AntialiasingLevel,
          None
        };

        int fbcount;
        GLXFBConfig* fbc = glXChooseFBConfig(display, screenId, glxAttribs, &fbcount);
        if (!fbc) {
          LOG(Log::CRITICAL) << "Failed to retrieve any framebuffer configs!";
          return nullptr;
        }

        for (int i = 0; i < fbcount; ++i) {
          int configId;
          glXGetFBConfigAttrib(display, fbc[i], GLX_FBCONFIG_ID, &configId);

          int sampBuf, samples;
          glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &sampBuf);
          glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);

          LOG(Log::DEFAULT) << "FBConfig " << configId << ", Sample Buffers: " << sampBuf << ", Samples: " << samples;
        }

        // Store the best config.
        GLXFBConfig fb = fbc[0];

        // Clean up.
        XFree(fbc);

        // Return the config.
        return fb;
      }

      XVisualInfo* LinuxWindowImpl::GetVisualInfoFromSettings(::Display* display, int screenId, const GLContext::Settings& _settings) {
        // Just use the first one for now.
        ::XVisualInfo* vi = glXGetVisualFromFBConfig(display, GetBestFBConfig(display, screenId, _settings));

        // Return the visual info we got.
        return vi;
      }

    	::Display* LinuxWindowImpl::GetDisplay() {
        // One display for each thread?
        static ::Display* display = XOpenDisplay(nullptr);
        if (!display) {
          LOG(Log::FATAL) << "Couldn't open display!";
        }
        return display;
      }

      xcb_connection_t* LinuxWindowImpl::GetXCBConnection() {
        static xcb_connection_t* conn = nullptr;
        if (!conn) {
          XLockDisplay(GetDisplay());
          conn = XGetXCBConnection(GetDisplay());
          XSetEventQueueOwner(GetDisplay(), XCBOwnsEventQueue);
          XUnlockDisplay(GetDisplay());
        }
        return conn;
      }

      // Constructors
      LinuxWindowImpl::LinuxWindowImpl() {
      }
      LinuxWindowImpl::~LinuxWindowImpl() {
        // Stop the window and wait for it to close.
        running = false;
        JoinThread();
      }

      // Create the window
      bool LinuxWindowImpl::Setup(int _width, int _height, int _depth, bool _fullscreen, const GLContext::Settings& _settings) {
        width = _width;
        height = _height;
        depth = _depth;
        fullscreen = _fullscreen;
        settings = _settings;

        // Start the thread.
      	StartThread();

      	// Wait for the hwnd to be ready.
      	WaitForWait();

        return true;
      }

      bool LinuxWindowImpl::RunThread() {
        Thread::RunThread();

        // Lock our use of the display connection..
        XLockDisplay(GetDisplay());

        int screenId = DefaultScreen(GetDisplay());

        // Make sure we have glx extensions loaded.
        gladLoadGLX(GetDisplay(), screenId);

        // Make sure we've got a good version of GLX
        int majorGLX, minorGLX = 0;
        glXQueryVersion(GetDisplay(), &majorGLX, &minorGLX);
        if (majorGLX <= 1 && minorGLX < 3) {
          LOG(Log::CRITICAL) << "GLX 1.3 or greater is required.\n";
          return false;
        }
        else {
          LOG(Log::DEFAULT) << "GLX version: " << majorGLX << "." << minorGLX << '\n';
        }

        // Pick a visual info based on the context display settings.
        ::XVisualInfo* vi = GetVisualInfoFromSettings(GetDisplay(), screenId, settings);
        if (!vi) {
          LOG(Log::CRITICAL) << "Couldn't find an XVisualInfo that matches Context Settings!";
          return false;
        }

        // Open the actual window.
        XSetWindowAttributes windowAttribs;
        windowAttribs.border_pixel = BlackPixel(GetDisplay(), screenId);
        windowAttribs.background_pixel = WhitePixel(GetDisplay(), screenId);
        windowAttribs.override_redirect = True;
        windowAttribs.colormap = XCreateColormap(GetDisplay(), RootWindow(GetDisplay(), screenId), vi->visual, AllocNone);
        windowAttribs.event_mask = ExposureMask | FocusChangeMask;
        window = XCreateWindow(GetDisplay(),
                               RootWindow(GetDisplay(), screenId),
                               0, 0,
                               width, height,
                               0,
                               vi->depth,
                               InputOutput,
                               vi->visual,
                               CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                               &windowAttribs);

        XClearWindow(GetDisplay(), window);
        XMapRaised(GetDisplay(), window);

        // Finished with the display
        XUnlockDisplay(GetDisplay());

        Wait();

        XLockDisplay(GetDisplay());

        // Have to do this ridiculousness just to capture when the window is closed...
        Atom wmDeleteMessage = XInternAtom(GetDisplay(), "WM_DELETE_WINDOW", False);
        XSetWMProtocols(GetDisplay(), window, &wmDeleteMessage, 1);

        XUnlockDisplay(GetDisplay());

        // Loop through the events dealing with them.
        running = true;
        while (running) {
          xcb_generic_event_t* event = xcb_wait_for_event(GetXCBConnection());
          switch(event->response_type & ~0x80) {
            case XCB_EXPOSE:
              break;

            case XCB_CLIENT_MESSAGE: {
              xcb_client_message_event_t* clientMessage = (xcb_client_message_event_t*)event;
              if (clientMessage->data.data32[0] == wmDeleteMessage)
                running = false;
              break;
            }

            default:
              break;
          }

          free(event);
        }

        XFree(vi);

        XLockDisplay(GetDisplay());
        XFreeColormap(LinuxWindowImpl::GetDisplay(), windowAttribs.colormap);
        XDestroyWindow(LinuxWindowImpl::GetDisplay(), window);
        XUnlockDisplay(GetDisplay());

        return true;
      }

      // Opens the window
      void LinuxWindowImpl::Run() {
        // Signal the thread to continue.
      	Signal();
      }

      // Returns true if the window is open
      bool LinuxWindowImpl::IsOpen() const {
	      return Thread::IsThreadRunning();
      }

      // Get the platform specific handle
      void* LinuxWindowImpl::GetWindowHandle() const {
        return (void*)window;
      }

      // Set the window title
      void LinuxWindowImpl::SetTitle(const char* _title) {
        XLockDisplay(GetDisplay());
        XStoreName(GetDisplay(), window, _title);
        XUnlockDisplay(GetDisplay());
      }

      // Get width and height
      int LinuxWindowImpl::GetWidth() {
        ::Window root;
        int x, y;
        unsigned int width, height, borderWidth, depth;
        XLockDisplay(GetDisplay());
        XGetGeometry(LinuxWindowImpl::GetDisplay(), window,
                     &root, &x, &y, &width, &height, &borderWidth, &depth);
        XUnlockDisplay(GetDisplay());
        return width;
      }
      int LinuxWindowImpl::GetHeight() {
        ::Window root;
        int x, y;
        unsigned int width, height, borderWidth, depth;
        XLockDisplay(GetDisplay());
        XGetGeometry(LinuxWindowImpl::GetDisplay(), window,
                     &root, &x, &y, &width, &height, &borderWidth, &depth);
        XUnlockDisplay(GetDisplay());
        return height;
      }

    }
  }
}

#endif
