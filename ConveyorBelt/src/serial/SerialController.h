/*
 * SerialController.h
 *
 *  Created on: 29.04.2018
 *      Author: LuisNickel
 */

#ifndef SRC_SERIALCONTROLLER_H_
#define SRC_SERIALCONTROLLER_H_
#include <string>
#include "SerialMsg.h"
#include "MsgQueue.h"
#include <mutex>

class SerialController {
public:
	SerialController(int channelId);
	virtual ~SerialController();
	void sendMsg(SerialMsg msg);
	void operator()();
	void stop();
	SerialMsg getNextMsg();
	bool receiveEmpty();
	bool isConnected();
	void reset();
private:
	long buildCheckSum(SerialMsg msg);
	void receive();
	void send();
	void init();
	int fdesc_;
	int ackNmb_;
	int msgNmb_;
	bool running_;
	MsgQueue sendQueue_;
	MsgQueue receivedQueue_;
	int connectionId_;
	bool connected_;
	bool oldConnected_;
	int version;
	std::mutex serialMutex_;


};

#endif /* SRC_SERIALCONTROLLER_H_ */
