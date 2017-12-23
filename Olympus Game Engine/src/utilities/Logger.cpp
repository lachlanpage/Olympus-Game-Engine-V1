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
	//clear file upon initiallisation
	std::ofstream logFile;
	logFile.open(fileName, std::ios::trunc);
}
//create file and delete contents
//std::ofstream logFile;
//logFile.open(fileName, std::ios::trunc);
//logFile.close();
void Logger::write(std::string message) {
	std::ofstream logFile;
	logFile.open(fileName, std::ios::app);
	if (logFile.is_open()) {
		logFile << message;
		logFile << "\n";
		logFile.close();
	}
}

