#include <Orange/timing/Timer.hpp>
using namespace orange;

Timer::Timer() {
  timer = priv::TimerImpl::Create();
}

Timer::~Timer() {
  if (timer)
    delete timer;
  timer = nullptr;
}

double Timer::Reset() {
  if (timer)
    return timer->Reset();
  return 0.0;
}
