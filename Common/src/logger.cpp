#include "common/logger.hpp"

#include "common/term.hpp"

#include <iostream>

void Logger::log(const LogLevel &level, const std::string &msg) const {
	switch (level) {
		case LogLevel::LOG_INFO: {
			this->logInfo(msg);
			break;
		}
		case LogLevel::LOG_WARN: {
			this->logWarning(msg);
			break;
		}
		case LogLevel::LOG_ERR: {
			this->logError(msg);
			break;
		}
		case LogLevel::LOG_CRIT: {
			this->logCritical(msg);
			break;
		}
		default:
			return;
	}
}

void Logger::logInfo(const std::string &content) const {
	std::cout << FG_LGREEN << "[" << this->m_strings.at(LOG_INFO) << "]" << " " << content << RESET
			  << "\n";
}

void Logger::logWarning(const std::string &content) const {
	std::cout << BG_LYELLOW << FG_BLACK << "[" << this->m_strings.at(LOG_WARN) << "]" << RESET
			  << FG_LYELLOW << " " << content << RESET << "\n";
}

void Logger::logError(const std::string &content) const {
	std::cerr << UNDERLINE << FG_RED << "[" << this->m_strings.at(LOG_ERR) << "]" << RESET << FG_RED
			  << " " << content << RESET << "\n";
}

void Logger::logCritical(const std::string &content) const {
	std::cerr << BOLD << BG_LRED << FG_BLACK << "[" << this->m_strings.at(LOG_CRIT) << "]" << RESET
			  << FG_LRED << " " << content << RESET << "\n";
}
