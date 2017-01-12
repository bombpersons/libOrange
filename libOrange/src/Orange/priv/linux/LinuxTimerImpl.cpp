#if defined __linux__

#include <Orange/priv/linux/LinuxTimerImpl.hpp>

namespace orange {
  namespace priv {
    namespace linux {

      LinuxTimerImpl::LinuxTimerImpl() {
        Reset();
      }
      LinuxTimerImpl::~LinuxTimerImpl() {
      }

      double LinuxTimerImpl::Reset() {
        struct timespec newTime;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &newTime);

        // Could we get precision problems here??
        double newSeconds = newTime.tv_sec + ((double)newTime.tv_nsec / 1000000000.0);
        double oldSeconds = oldTime.tv_sec + ((double)oldTime.tv_nsec / 1000000000.0);
        oldTime = newTime;

        // Convert our nanosecond time to seconds...
        return newSeconds - oldSeconds;
      }

    }
  }
}

#endif
