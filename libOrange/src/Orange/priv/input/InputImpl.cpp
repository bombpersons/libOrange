#include <Orange/priv/input/InputImpl.hpp>
using namespace orange;
using namespace orange::priv;

#include <glm/glm.hpp>

// Depending on the system we are compiling on, use different implementations of this class
#if defined __CYGWIN__ || defined _WIN32
#include <Orange/priv/win32/Win32InputImpl.hpp>
typedef win::Win32InputImpl InputImplType;
#endif

// Create
InputImpl* InputImpl::Create(WindowImpl* _window) {
  return new InputImplType(_window);
}
