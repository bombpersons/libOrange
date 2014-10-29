#include <Orange/priv/threads/StdThreadImpl.hpp>
using namespace orange::priv;

StdThreadImpl::StdThreadImpl(ThreadImpl::ThreadFunc _func, void* _param) {
  func = _func;
  param = _param;

  running = false;

  thread = nullptr;

  signaled = false;
  waiting = false;
}

StdThreadImpl::~StdThreadImpl() {
  TerminateThread();
}

bool StdThreadImpl::StartThread() {
  // Stop the thread if we are running
  TerminateThread();

  // Not signaled
  signaled = false;
  waiting = false;

  // Start a new thread.
  thread = new std::thread(StdThreadImpl::StaticThreadFunc, this);

  // Set running to true now. If we do it later, then
  // for a short period of time, IsThreadRunning will return
  // false, which can screw up some things, such as checking whether
  // or not a window is open.
  running = true;
  return IsThreadRunning();
}

// Wait for a signal (only call this from inside the thread.)
void StdThreadImpl::Wait() {
  // Reset the signal
  signaled = false;
  waiting = true;

  // Signal anyone waiting for us to wait.
  waitForSuspend.notify_all();

  // Wait to be signalled.
  std::unique_lock<std::mutex> lock(suspendMutex);
  suspend.wait(lock, [this]() {return this->signaled;});

  // Not waiting anymore
  waiting = false;
}

// Signal the thread (resumes it if it was waiting.)
void StdThreadImpl::Signal() {
  // Set the signal
  signaled = true;
  suspend.notify_all();
}

// Wait for the thread wait.
void StdThreadImpl::WaitForWait() {
  std::unique_lock<std::mutex> lock(waitForSuspendMutex);
  waitForSuspend.wait(lock, [this]() {return this->waiting;});

  // Thread is now waiting
  return;
}

void StdThreadImpl::TerminateThread() {
  if (thread)
    delete thread;
  thread = nullptr;
}

bool StdThreadImpl::IsThreadRunning() const {
  return running;
}

void StdThreadImpl::JoinThread() {
  if (thread)
    thread->join();
}

bool StdThreadImpl::StaticThreadFunc(void* _param) {
  StdThreadImpl* self = (StdThreadImpl*)_param;
  bool ret = false;
  if (self) {
    ret = (*(self->func))(self->param);
  }

  self->running = false;
  return ret;
}
