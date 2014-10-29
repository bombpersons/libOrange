#include <Orange/logging/FileLogListener.hpp>
#include <cstring>
using namespace orange;

FileLogListener::FileLogListener(const OChar* _filename) {
	file = fopen(_filename, "w");
}

FileLogListener::~FileLogListener() {
	if (file)
		fclose(file);
}

void FileLogListener::Log(const OChar* _line) {
	if (file)
		fwrite(_line, strlen(_line), sizeof(OChar), file);
}
