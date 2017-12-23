#ifndef BUSNODE_H
#define BUSNODE_H
#include <iostream>
#include <functional> 
#include "MessageBus.h"
#include "Message.h"
class BusNode {
public:
	BusNode(MessageBus *messageBus) {
		this->messageBus = messageBus;
		this->messageBus->addReceiver(this->getNotifyFunc());
	}

	virtual void update() {};
protected:
	MessageBus *messageBus;
	std::function<void(Message)> getNotifyFunc() {
		auto messageListener = [=](Message message) -> void {
			this->onNotify(message);
		};
		return messageListener;
	}

	void send(Message message) {
		messageBus->sendMessage(message);
	}

	virtual void onNotify(Message message) {
	}
};

#endif