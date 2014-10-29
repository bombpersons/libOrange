#ifndef ORANGE_THREADIMPL
#define ORANGE_THREADIMPL

#include <Orange/util/NoCopy.hpp>

namespace orange {
	namespace priv {
		class ThreadImpl : NoCopy {
		public:
			// Typedef for function ponter
			typedef bool (*ThreadFunc)(void*);

			// Constructors
			ThreadImpl();
			virtual ~ThreadImpl();

			// Start the thread
			virtual bool StartThread() = 0;

			// Wait for a signal (only call this from inside the thread.)
			virtual void Wait() = 0;

			// Signal the thread (resumes it if it was waiting.)
			virtual void Signal() = 0;

			// Wait for the thread wait.
			virtual void WaitForWait() = 0;

			// Terminate the thread
			virtual void TerminateThread() = 0;

			// Check whether or not the thread is running
			virtual bool IsThreadRunning() const = 0;

			// Wait for this thread to finish.
			virtual void JoinThread() = 0;

			// Create the thread
			static ThreadImpl* Create(ThreadFunc _func, void* _param);
		private:
		};
	}
}

#endif
