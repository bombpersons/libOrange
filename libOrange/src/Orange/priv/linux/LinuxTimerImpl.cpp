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
        long nanoDiff = newTime.tv_nsec - oldTime.tv_nsec;
        oldTime = newTime;

        // Convert our nanosecond time to seconds...
        return (double)nanoDiff * 1000000000.0;

      }

    }
  }
}

#endif
