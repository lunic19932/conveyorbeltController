/*
 * Dispatcher.h
 *
 *  Created on: 28.04.2018
 *      Author: Johannes Kruber
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_
#include "Message.h"
#include "ClientId.h"
#include <queue>

namespace communication {

class Dispatcher {
private:
	int channelId;
	Message msg, msgsave;
	std::queue<Message> fifo;

	bool running;
	bool sensorsInitialised;
	int clients[COUNT];

	void receiveMsg();
	void log();

public:
	Dispatcher();
	virtual ~Dispatcher();

	void operator()();
	void stop();

	int getChannelId();
};

} /* namespace communication */

#endif /* DISPATCHER_H_ */
