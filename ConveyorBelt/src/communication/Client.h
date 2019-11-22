/*
 * Client.h
 *
 *  Created on: 28.04.2018
 *      Author: Johannes Kruber
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "Message.h"
#include "ClientId.h"

namespace communication
{

class Client
{
private:
	bool running;
	int channelId, connectionId;

protected:
	Message msg;

	virtual void work();

public:
	Client(int channelId);
	virtual ~Client();

	void operator()();
	void stop();

};

} /* namespace communication */

#endif /* CLIENT_H_ */
