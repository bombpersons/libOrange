#if defined __CYGWIN__ || defined _WIN32
#ifndef ORANGE_WIN32THREADIMPL
#define ORANGE_WIN32THREADIMPL

#include <Windows.h>
#include <Orange/priv/threads/ThreadImpl.hpp>

namespace orange {
	namespace priv {
		namespace win {
			class Win32ThreadImpl : public ThreadImpl {
			public:
				Win32ThreadImpl(ThreadFunc _func, void* param);
				virtual ~Win32ThreadImpl();

				// Start the thread
				virtual bool StartThread();

				// Terminate the thread
				virtual void TerminateThread();

				// Wait for this thread to finish.
				virtual void JoinThread();

				// Check if the thread is running
				virtual bool IsThreadRunning() const;

				// Called by thread
				void RunThread();

			private:
				HANDLE handle;
				DWORD threadId;
				ThreadFunc threadFunc; // The function to call
				void* param; // Parameter to send to function

				static DWORD WINAPI StaticStartFunction(LPVOID _param);
			};
		}
	}
}

#endif
#endif
