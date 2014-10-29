#ifndef ORANGE_THREADS
#define ORANGE_THREADS

#include <Orange/util/NoCopy.hpp>
#include <Orange/priv/threads/ThreadImpl.hpp>

namespace orange {

	class Thread : NoCopy {
	public:
		// Constructors
		Thread();
		virtual ~Thread();

		// Start the thread
		bool StartThread();

		// Wait for a signal (only call this from inside the thread.)
		void Wait();

		// Signal the thread (resumes it if it was waiting.)
		void Signal();

		// Wait for the thread wait.
		void WaitForWait();

		// Terminate the thread
		void TerminateThread();

		// Wait for this thread to finish.
		void JoinThread();

		// Check if the thread is running
		bool IsThreadRunning() const;

		// Inherit from this and put code to run in the thread.
		virtual bool RunThread();

	private:
		priv::ThreadImpl* thread;

		// Thread will call this function.
		static bool StaticThreadFunc(void* _param);
	};
}

#endif
