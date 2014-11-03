#ifndef ORANGE_TIMER
#define ORANGE_TIMER

#include <Orange/priv/timing/TimerImpl.hpp>

namespace orange {
  class Timer {
  public:
    Timer();
    virtual ~Timer();

    // Reset the timer
    double Reset();

  private:
    priv::TimerImpl* timer;
  };
}

#endif
