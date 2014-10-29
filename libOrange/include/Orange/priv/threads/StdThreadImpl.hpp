#ifndef ORANGE_STDTHREADIMPL
#define ORANGE_STDTHREADIMPL

#include <Orange/priv/threads/ThreadImpl.hpp>
#include <thread>
#include <atomic>
#include <condition_variable>


namespace orange {
  namespace priv {
    class StdThreadImpl : public ThreadImpl {
    public:
      // Constructors
      StdThreadImpl(ThreadImpl::ThreadFunc _func, void* _param);
      virtual ~StdThreadImpl();

      // Start the thread
      virtual bool StartThread();

      // Wait for a signal (only call this from inside the thread.)
      virtual void Wait();

      // Signal the thread (resumes it if it was waiting.)
      virtual void Signal();

      // Wait for the thread wait.
      virtual void WaitForWait();

      // Terminate the thread
      virtual void TerminateThread();

      // Check whether or not the thread is running
      virtual bool IsThreadRunning() const;

      // Wait for this thread to finish.
      virtual void JoinThread();

    private:
      // A static function that the thread will call, which then
      // bootstraps into the function the user wants to call.
      static bool StaticThreadFunc(void* _param);

      // Function to call when thread starts
      ThreadImpl::ThreadFunc func;
      void* param;

      // Whether or not the thread is currently running
      std::atomic<bool> running;

      // The thread
      std::thread* thread;

      // Conditional variables for suspending thet thread.
      std::condition_variable suspend;
      std::mutex suspendMutex;

      std::condition_variable waitForSuspend;
      std::mutex waitForSuspendMutex;

      // Whether or not the thread is signaled
      bool signaled;
      bool waiting;
    };
  }
}

#endif
