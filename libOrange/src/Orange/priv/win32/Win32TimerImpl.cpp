#if defined __CYGWIN__ || _WIN32

#include <Orange/priv/win32/Win32TimerImpl.hpp>

namespace orange {
  namespace priv {
    namespace win {

      Win32TimerImpl::Win32TimerImpl() {
        // Grab the frequency
        QueryPerformanceFrequency(&frequency);
        Reset();
      }

      Win32TimerImpl::~Win32TimerImpl() {
      }

      double Win32TimerImpl::Reset() {
        LARGE_INTEGER end;
        QueryPerformanceCounter(&end);

        // Get it in seconds.
        double ret = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

        // Set that as the start.
        start = end;

        // Return how long since last reset
        return ret;
      }

    }
  }
}

#endif
