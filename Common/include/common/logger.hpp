#pragma once

#include <map>
#include <string>
#include "utils.hpp"

#define INFO(msg) Logger::instance()->logInfo(msg)
#define WARN(msg) Logger::instance()->logWarning(msg)
#define ERROR(msg) Logger::instance()->logError(msg)
#define CRITICAL(msg) Logger::instance()->logCritical(msg)

enum LogLevel {
	LOG_INFO,
	LOG_WARN,
	LOG_ERR,
	LOG_CRIT,
};

/**
 * @class Logger
 * @brief Singleton class used to dump logs to standard output/err.
 */
class Logger {
public:
	void log(const LogLevel &level, const std::string &msg) const;

	void logInfo(const std::string &content) const;

	void logWarning(const std::string &content) const;

	void logError(const std::string &content) const;

	void logCritical(const std::string &content) const;

	Logger(Logger &other) = delete;

	void operator=(const Logger &other) = delete;

	/**
	 * @brief Retrieves the instance of the Logger.
	 * If it's not instanced, it will be instanced automatically.
	 *
	 * @return a Shared<Logger> object
	 */
	inline static Shared<Logger> instance() {
		if (s_pointer == nullptr) {
			Shared<Logger> copy(new Logger());
			copy.swap(s_pointer);
		}
		return s_pointer;
	}

	~Logger() = default;

private:
	Logger() = default;

	/// static shared pointer for Singleton
	inline static Shared<Logger> s_pointer = nullptr;

	const std::map<LogLevel, std::string> m_strings{
		{LOG_INFO, "INFO"},
		{LOG_WARN, "WARN"},
		{LOG_ERR, "ERROR"},
		{LOG_CRIT, "CRITICAL"},
	};
};
