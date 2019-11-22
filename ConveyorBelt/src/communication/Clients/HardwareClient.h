/*
 * HardwareClient.h
 *
 *  Created on: 30.04.2018
 *      Author: Johannes Kruber
 */

#ifndef COMMUNICATION_CLIENTS_HARDWARECLIENT_H_
#define COMMUNICATION_CLIENTS_HARDWARECLIENT_H_

#include "../Client.h"
#include "../../hardware/Controller.h"

namespace communication {

class HardwareClient : public Client
{
private:
	bool adcEnable;

public:
	hardware::Controller controller;
	HardwareClient(int channelId);
	virtual ~HardwareClient();

	hardware::Controller* getController(void);

protected:
	virtual void work();
};

} /* namespace communication */

#endif /* COMMUNICATION_CLIENTS_HARDWARECLIENT_H_ */
