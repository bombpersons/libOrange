#include <Orange/priv/timing/TimerImpl.hpp>
using namespace orange::priv;

#if defined __CYGWIN__ || defined _WIN32
#include <Orange/priv/win32/Win32TimerImpl.hpp>
typedef win::Win32TimerImpl TimerImplType;
#elif defined __linux__
#include <Orange/priv/linux/LinuxTimerImpl.hpp>
typedef linux::LinuxTimerImpl TimerImplType;
#endif

TimerImpl::TimerImpl() {
}

TimerImpl::~TimerImpl() {
}

TimerImpl* TimerImpl::Create() {
  return new TimerImplType();
}
