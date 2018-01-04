#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>

class Message {
public:
	Message(const std::string event) {
		messageEvent = event;
	}

	Message(){}

	void setEvent(const std::string event) {
		messageEvent = event;
	}

	std::string getEvent() {
		return messageEvent;
	}

private:
	std::string messageEvent;
};
#endif