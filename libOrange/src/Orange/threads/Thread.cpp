#include <Orange/threads/Thread.hpp>
using namespace orange;

#include <Orange/logging/Logging.hpp>

Thread::Thread() {
	// Create the thread
	thread = priv::ThreadImpl::Create(StaticThreadFunc, this);
}

Thread::~Thread() {
}

// Start thread
bool Thread::StartThread() {
	if (thread) {
		LOG(Log::DEFAULT) << "Starting thread...";
		return thread->StartThread();
	}

	LOG(Log::WARNING) << "Attempte to start thread that is already running...";
	return false;
}

// Wait for a signal (only call this from inside the thread.)
void Thread::Wait() {
	if (thread)
		thread->Wait();
}

// Signal the thread (resumes it if it was waiting.)
void Thread::Signal() {
	if (thread)
		thread->Signal();
}

// Wait for the thread wait.
void Thread::WaitForWait() {
	if (thread)
		thread->WaitForWait();
}

// Terminate
void Thread::TerminateThread() {
	if (thread)
		thread->TerminateThread();
}

// Join
void Thread::JoinThread() {
	if (thread)
		thread->JoinThread();

	LOG(Log::DEFAULT) << "Joined Thread.";
}

// Check if thread is running
bool Thread::IsThreadRunning() const {
	if (thread)
		return thread->IsThreadRunning();

	return false;
}

// Called by thread
bool Thread::RunThread() {
	LOG(Log::DEFAULT) << "Thread running... ";
	return true;
}

bool Thread::StaticThreadFunc(void* _param) {
	// Call the thread objects Run() function
	if (_param) {
		bool ret =  ((Thread*)_param)->RunThread();
		LOG(Log::DEFAULT) << "Thread closing... ";
		return ret;
	}

	return false;
}
