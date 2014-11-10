#include <GL/glew.h>

#include <Orange/gl/GLContext.hpp>
using namespace orange;

#include <Orange/logging/Logging.hpp>

#if defined __CYGWIN__ || defined _WIN32
#include <Orange/priv/win32/Win32GLContext.hpp>
typedef priv::win::Win32GLContext GLContextType;
#endif

#include <Orange/priv/window/WindowImpl.hpp>

#include <set>
#include <mutex>
#include <thread>

// Internal contexts
// A context to create all other contexts with,
// so that all the contexts can share resources.
GLContextType* shared;

// The current context for each thread
thread_local GLContext* currentContext;

// Keep track of contexts we allocate internally so we
// can delete them later.
thread_local GLContext* internalContext;
std::set<GLContext*> internalContexts;
std::mutex internalContextsMutex;

// Check if a thread has an internal context or not.
static bool HasInternalContext() {
  if (!internalContext)
    return false;

  // Might not have been deleted from the internal context list...
  std::lock_guard<std::mutex> lock(internalContextsMutex);
  return internalContexts.find(internalContext) != internalContexts.end();
}

// Get an internal context
GLContext* GetInternalContext() {
  if (!HasInternalContext()) {
    // Create one
    internalContext = GLContext::Create();
    std::lock_guard<std::mutex> lock(internalContextsMutex);
    internalContexts.insert(internalContext);
  }

  return internalContext;
}

GLContext::GLContext() {
}

GLContext::~GLContext() {
}

void GLContext::Init() {
  // Activate the context
  SetActive(true);

  // Get the opengl version
  const unsigned char* version = glGetString(GL_VERSION);
  if (version) {
    settings.MajorVersion = version[0] - '0';
    settings.MinorVersion = version[2] - '0';
  } else {
    settings.MajorVersion = 2;
    settings.MinorVersion = 0;
  }

  LOG(Log::DEFAULT) << "Created an Opengl " << settings.MajorVersion
					<< "." << settings.MinorVersion << " context.";
}

bool GLContext::SetActive(bool _v) {
  if (_v) {
    if (this != currentContext) {
      if (MakeCurrent()) {
        currentContext = this;
        return true;
      } else {
        return false;
      }
    } else {
      // Context is already active on this thread.
      return true;
    }
  } else {
    if (this == currentContext) {
      // Set the internal context to active, so that there is always
      // a context active.
      return GetInternalContext()->SetActive(true);
    } else {
      // Context isn't active on the thread anyways.
      return true;
    }
  }
}

// Get settings
const GLContext::Settings& GLContext::GetSettings() {
  return settings;
}

// Static functions
void GLContext::StaticInit() {
  if (shared) {
    LOG(Log::WARNING) << "GLContext::Init() called more than once!";
    return; // Don't init more than once!
  }

  // Create the context and make it active so we can init glew
  shared = new GLContextType(nullptr);

  // Initialize the shared context and what not.
  shared->Init();
  shared->SetActive(false);

  // Initialize glew
  if (glewInit() != GLEW_OK) {
      LOG(Log::FATAL) << "Error initializing glew! Opengl Error: " << glGetError();
  }
}
void GLContext::StaticShutdown() {
  if (!shared) {
    LOG(Log::WARNING) << "GLContext::Shutdown() called before Init()!";
    return;
  }
  LOG(Log::DEFAULT) << "Shutting down OpenGL...";

  // Delete the context
  delete shared;
  shared = nullptr;

  // Delete all of the internal contexts.
  std::lock_guard<std::mutex> lock(internalContextsMutex);
  for (auto it = internalContexts.begin(); it != internalContexts.end(); ++it) {
    delete *it;
  }
  internalContexts.clear();
}

// Ensures that the current thread has
// an active opengl context. It will create one
// if one doesn't exist.
void GLContext::EnsureContext() {
  if (currentContext)
    GetInternalContext()->SetActive(true);
}

// Create a new context
GLContext* GLContext::Create() {
  // Create a context
  GLContext* context = new GLContextType(shared);
  context->Init();

  return context;
}
GLContext* GLContext::Create(const Settings& _settings, const priv::WindowImpl* _window) {
  // Make sure we have a context first..
  EnsureContext();

  // Create the context
  GLContext* context = new GLContextType(shared, _settings, _window);
  context->Init();
  return context;
}
GLContext* GLContext::Create(const Settings& _settings, unsigned int _width, unsigned int _height) {
  // make sure we have a context first..
  EnsureContext();

  // Create the context
  GLContext* context = new GLContextType(shared, _settings, _width, _height);
  context->Init();
  return context;
}
