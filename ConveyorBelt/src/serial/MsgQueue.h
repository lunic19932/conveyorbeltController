/*
 * MsgQueue.h
 *
 *  Created on: 06.05.2018
 *      Author: LuisNickel
 */

#ifndef SRC_MSGQUEUE_H_
#define SRC_MSGQUEUE_H_
#include <queue>
#include <mutex>
#include "SerialMsg.h"


class MsgQueue {
public:
	MsgQueue();
	virtual ~MsgQueue();
	SerialMsg front();
	void pop();
	void push(SerialMsg msg);
	int size();
	bool empty();
	void clear();
private:
	std::queue<SerialMsg> msgQueue_;
	std::mutex queueMutex_;

};

#endif /* SRC_MSGQUEUE_H_ */
