#ifndef LOGGER_H
#define LOGGER_H
#include <string>
class Logger {
	//general singleton logger class used to output/troubleshoot if iostream is unreasonable
public:
	static Logger* Instance();
	void write(std::string message);
private:
	Logger();
	Logger(Logger const&) {};
	Logger& operator=(Logger const&) {};
	static Logger* m_pInstance;
	std::string fileName = "log.txt";
};

#endif

