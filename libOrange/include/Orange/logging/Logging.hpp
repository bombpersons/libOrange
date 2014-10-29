#ifndef ORANGE_LOGGING
#define ORANGE_LOGGING

#include <Orange/Types.hpp>

#include <sstream>
#include <unordered_map>

namespace orange {
	class Log {
	public:
		// Enums
		enum LogLevel {
			DEFAULT = 0,
			WARNING,
			CRITICAL,
			FATAL,

			LOGLEVEL_COUNT
		};
		static const OChar* LogLevelStrings[LOGLEVEL_COUNT];

		// Inherit from this to listen to log messages as they get written.
		struct LogListener {
			LogListener() {}
			virtual ~LogListener() {}

			virtual void Log(const OChar* _line) = 0;
		};

		// Constructors
		Log();
		virtual ~Log();

		// Add / Remove listeners
		static bool AddListener(const std::string& _name, LogListener* _listener);
		static bool RemoveListener(const std::string& _name);

		// Set the miminum logging level
		static void SetMinLogLevel(LogLevel _min);
		static LogLevel GetMinLogLevel();

		// Log
		std::ostream& Write(LogLevel _level=DEFAULT);

	private:
		std::ostringstream stream;
		LogLevel curLogLevel;

		static std::unordered_map<std::string, LogListener*> listeners;
		static LogLevel minLogLevel;
	};

	// Use this function to log messages
	#define LOG(level) \
	if (level < Log::GetMinLogLevel()) ; \
	else Log().Write(level)
}

#endif
