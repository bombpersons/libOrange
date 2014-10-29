#ifndef ORANGE_FILELOGLISTENER
#define ORANGE_FILELOGLISTENER

#include <Orange/logging/Logging.hpp>
#include <cstdio>

namespace orange {
	struct FileLogListener : public Log::LogListener {
		FileLogListener(const OChar* _filename);
		virtual ~FileLogListener();

		virtual void Log(const OChar* _line);

	private:
		FILE* file;
	};
}

#endif