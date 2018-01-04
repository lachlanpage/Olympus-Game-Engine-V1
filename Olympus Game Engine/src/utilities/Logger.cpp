#include "Logger.h"
#include <iostream> 
#include <fstream>
Logger* Logger::m_pInstance = NULL;

Logger* Logger::Instance() {
	if (!m_pInstance)
		m_pInstance = new Logger();
	return m_pInstance;
}

Logger::Logger() {
	//trunc file upon start to clear log from previous run
	std::ofstream logFile;
	logFile.open(fileName, std::ios::trunc);
}

void Logger::write(std::string message) {
	std::ofstream logFile;
	logFile.open(fileName, std::ios::app);
	if (logFile.is_open()) {
		logFile << message;
		logFile << "\n";
		logFile.close();
	}
}

