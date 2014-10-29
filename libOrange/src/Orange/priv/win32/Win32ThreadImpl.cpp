#if defined __CYGWIN__ || defined _WIN32
#include <Orange/priv/win32/Win32ThreadImpl.hpp>
using namespace orange::priv::win;

#include <Orange/logging/Logging.hpp>

Win32ThreadImpl::Win32ThreadImpl(ThreadFunc _func, void* _param) {
	// Set the handle to null
	handle = 0;

	// The function to call
	threadFunc = _func;

	// Parameter to send to functino
	param = _param;
}

Win32ThreadImpl::~Win32ThreadImpl() {
	TerminateThread();
}

bool Win32ThreadImpl::StartThread() {
	// Create the handle
	if (handle == 0) {
		handle = CreateThread(NULL,
							  0,
							  Win32ThreadImpl::StaticStartFunction,
							  this,
							  CREATE_SUSPENDED,
							  &threadId);
		if (!handle) {
			LOG(Log::CRITICAL) << "Could not create win32 thread! Windows error number " << GetLastError();
			return false;
		}

		// Start the thread.
		ResumeThread(handle);

		// Succesfully started
		return true;
	}

	LOG(Log::WARNING) << "This win32 thread is already running!";
	return false; // There was already a thread started.
}

// Terminate the thread
void Win32ThreadImpl::TerminateThread() {
	// If a thread was created and is still running, we should terminate it.
	if (handle) {
		DWORD result = WaitForSingleObject(handle, 0);
		if (result != WAIT_OBJECT_0) {
			// Terminate
			LOG(Log::WARNING) << "Thread has not been safely closed, terminating..";
			if (!::TerminateThread(handle, 0)) {
				LOG(Log::CRITICAL) << "Could not terminate thread!";
			} else {
				LOG(Log::DEFAULT) << "Thread succesfully terminated";
			}
		}

		CloseHandle(handle);
		handle = 0;
	}
}

// Wait for this thread to finish.
void Win32ThreadImpl::JoinThread() {
	if (handle) {
		LOG(Log::DEFAULT) << "Waiting to join thread...";
		WaitForSingleObject(handle, INFINITE);
	}
}

bool Win32ThreadImpl::IsThreadRunning() const {
	return WaitForSingleObject(handle, 0) != WAIT_OBJECT_0;
}

void Win32ThreadImpl::RunThread() {
	(*threadFunc)(param);
}

DWORD WINAPI Win32ThreadImpl::StaticStartFunction(LPVOID _param) {
	// Get the thread object
	Win32ThreadImpl* thread = (Win32ThreadImpl*)(_param);
	if (thread) {
		thread->RunThread();
		return 1;
	}

	return 0;
}
#endif
