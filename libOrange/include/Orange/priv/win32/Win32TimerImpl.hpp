#ifndef ORANGE_WIN32TIMERIMPL
#define ORANGE_WIN32TIMERIMPL

#include <Orange/priv/timing/TimerImpl.hpp>
#include <Windows.h>

namespace orange {
  namespace priv {
    namespace win {
      class Win32TimerImpl : public TimerImpl {
      public:
        Win32TimerImpl();
        virtual ~Win32TimerImpl();

        virtual double Reset();

      private:
        LARGE_INTEGER frequency;
        LARGE_INTEGER start;
      };
    }
  }
}

#endif
