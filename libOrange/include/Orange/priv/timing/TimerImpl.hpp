#ifndef ORANGE_TIMERIMPL
#define ORANGE_TIMERIMPL

namespace orange {
  namespace priv {
    class TimerImpl {
    public:
      TimerImpl();
      virtual ~TimerImpl();

      // Create a timer.
      static TimerImpl* Create();

      // Reset
      virtual double Reset() = 0;

    private:

    };
  }
}

#endif
