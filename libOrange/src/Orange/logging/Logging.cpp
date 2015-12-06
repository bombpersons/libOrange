#include <Orange/logging/Logging.hpp>
using namespace orange;

#include <Orange/logging/ConsoleLogListener.hpp>
#include <iomanip>
#include <ctime>

const OChar* Log::LogLevelStrings[] = {
	"INFO",
	"WARNING",
	"CRITICAL",
	"FATAL"
};

std::map<std::string, Log::LogListener*> Log::listeners;
Log::LogLevel Log::minLogLevel = Log::DEFAULT;

Log::Log() {
	curLogLevel = DEFAULT;
}

Log::~Log() {
	stream << std::endl;
	for (auto it = listeners.begin(); it != listeners.end(); ++it) {
		it->second->Log(stream.str().c_str());
	}
}

bool Log::AddListener(const std::string& _name, LogListener* _listener) {
	if (listeners.find(_name) == listeners.end()) {
		listeners[_name] = _listener;
		return true;
	}

	return false;
}

bool Log::RemoveListener(const std::string& _name) {
	auto it = listeners.find(_name);
	if (it == listeners.end())
		return false;

	listeners.erase(it);
	return true;
}

void Log::SetMinLogLevel(LogLevel _min) {
	minLogLevel = _min;
}

Log::LogLevel Log::GetMinLogLevel() {
	return minLogLevel;
}

std::ostream& Log::Write(LogLevel _level) {
	curLogLevel = _level;

	// Make a temporary string stream
	std::stringstream temp;

	// Get the current time
	time_t t = time(0);
	struct tm* now = localtime(&t);
	char buf[80];
	strftime(buf, sizeof(buf), "%X", now);

	// Write the start bit of the line
	temp << "[" << buf << "]";
	temp << "[" << LogLevelStrings[_level] << "]: ";
	stream << std::left << std::setw(22) << temp.str();

	// Return the stream so more text can be added.
	return stream;
}
