#ifndef ORANGE_LINUXTIMERIMPL
#define ORANGE_LINUXTIMERIMPL

#include <Orange/priv/timing/TimerImpl.hpp>

#include <time.h>

namespace orange {
  namespace priv {
    namespace linux {
      class LinuxTimerImpl : public TimerImpl {
      public:
        LinuxTimerImpl();
        virtual ~LinuxTimerImpl();

        virtual double Reset();

      private:
        struct timespec oldTime = {};

      };
    }
  }
}

#endif
