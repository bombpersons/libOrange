#include <Orange/priv/threads/ThreadImpl.hpp>
using namespace orange::priv;

//#if defined __CYGWIN__ || defined _WIN32
//#include <Orange/priv/win32/Win32ThreadImpl.hpp>
//typedef Win32ThreadImpl ThreadImplType;
//#endif

#include <Orange/priv/threads/StdThreadImpl.hpp>
typedef StdThreadImpl ThreadImplType;

ThreadImpl::ThreadImpl() {
}

ThreadImpl::~ThreadImpl() {
}

ThreadImpl* ThreadImpl::Create(ThreadFunc _func, void* _param) {
	// Try and create a new thread.
	ThreadImpl* impl = new ThreadImplType(_func, _param);
	return impl;
}
