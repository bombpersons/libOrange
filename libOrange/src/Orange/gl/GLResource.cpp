#include <Orange/gl/GLResource.hpp>
#include <Orange/gl/GLContext.hpp>
using namespace orange;

#include <atomic>

// A static count of all GLResources
std::atomic<unsigned int> count(0);

GLResource::GLResource() {
  // If count was zero, then this is the first opengl resource.
  if (count == 0) {
    // Initialize opengl.
    GLContext::StaticInit();
  }

  // Increment the count
  count++;
}

GLResource::~GLResource() {
  // Decrement the count
  count--;

  // If the count is zero, then all opengl resources have been destroyed.
  if (count == 0) {
    // We can de-initialize opengl
    GLContext::StaticShutdown();
  }
}
