#ifndef ORANGE_STDOUTLOGLISTENER
#define ORANGE_STDOUTLOGLISTENER

#include <Orange/logging/Logging.hpp>

namespace orange {
	struct ConsoleLogListener : public Log::LogListener {
		ConsoleLogListener();
		virtual ~ConsoleLogListener();

		virtual void Log(const OChar* _line);
	};
}

#endif