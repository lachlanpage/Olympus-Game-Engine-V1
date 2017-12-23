#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H
#include <functional> 
#include <queue>
#include <vector> 

#include "Message.h"

class MessageBus {
public: 
	
	static MessageBus* Instance();

	void addReceiver(std::function<void(Message)> messageReceiver) {
		recievers.push_back(messageReceiver);
	}

	void sendMessage(Message message) {
		messages.push(message);
	}

	void notify() {
		while (!messages.empty()) {
			for (auto iter = recievers.begin(); iter != recievers.end();iter++) {
				(*iter)(messages.front());
			}

			messages.pop();
		}
	}

private:
	MessageBus() {};
	std::vector<std::function<void(Message)>> recievers;
	std::queue<Message> messages;
	static MessageBus* m_pInstance;
};
#endif