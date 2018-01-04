#ifndef LOGGER_H
#define LOGGER_H
#include <string>
class Logger {
	//General singleton logger class used to output/troubleshoot if iostream is unreasonable 
	//P.S - first time I ever learnt about/introduced to singleton classes :) 
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

