/*
 * MsgQueue.cpp
 *
 *  Created on: 06.05.2018
 *      Author: LuisNickel
 */

#include "MsgQueue.h"

MsgQueue::MsgQueue() {
	// TODO Auto-generated constructor stub

}

MsgQueue::~MsgQueue() {
	// TODO Auto-generated destructor stub
}

SerialMsg MsgQueue::front() {
	SerialMsg msg;
	msg.header.type = -1;
	std::lock_guard<std::mutex> lock(queueMutex_);
	if (!msgQueue_.empty()) {
		msg = msgQueue_.front();
	}

	return msg;
}

void MsgQueue::pop() {
	std::lock_guard<std::mutex> lock(queueMutex_);

	if (!msgQueue_.empty()) {
		msgQueue_.pop();
	}
}

void MsgQueue::push(SerialMsg msg) {
	std::lock_guard<std::mutex> lock(queueMutex_);
	msgQueue_.push(msg);
}

int MsgQueue::size() {
	return msgQueue_.size();
}

bool MsgQueue::empty() {
	return msgQueue_.empty();
}

void MsgQueue::clear(){
	while(!msgQueue_.empty()){
		msgQueue_.pop();
	}
}
