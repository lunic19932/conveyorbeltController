/*
 * PulseReceiver.h
 *
 *  Created on: 30.04.2018
 *      Author: Luisn
 */

#ifndef COMMUNICATION_CLIENTS_PULSERECEIVER_H_
#define COMMUNICATION_CLIENTS_PULSERECEIVER_H_

#include "../Client.h"
namespace communication {

class PulseReceiver :public Client{
public:
	PulseReceiver(int channelId);
	virtual ~PulseReceiver();
	int getChannelId();
protected:
	virtual void work();
private:
	int counter_;
	int channelId_;
};
}
#endif /* COMMUNICATION_CLIENTS_PULSERECEIVER_H_ */
