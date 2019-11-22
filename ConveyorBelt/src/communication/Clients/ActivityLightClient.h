/*
 * ActivityLightClient.h
 *
 *  Created on: 30.04.2018
 *      Author: Johannes Kruber
 */

#ifndef COMMUNICATION_CLIENTS_ACTIVITYLIGHTCLIENT_H_
#define COMMUNICATION_CLIENTS_ACTIVITYLIGHTCLIENT_H_

#include "../Client.h"

namespace communication
{

class ActivityLightClient : public Client
{
private:
	bool blink[BLINK_ARRAY_SIZE];
	bool acLightStatus[BLINK_ARRAY_SIZE];

public:
	ActivityLightClient(int channelId);
	virtual ~ActivityLightClient();
protected:
	virtual void work();
};

} /* namespace communication */

#endif /* COMMUNICATION_CLIENTS_ACTIVITYLIGHTCLIENT_H_ */
