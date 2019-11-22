/*
 * ActivityLightClient.cpp
 *
 *  Created on: 30.04.2018
 *      Author: Johannes Kruber
 */

#include "ActivityLightClient.h"
#include "../ClientId.h"

namespace communication
{

#define TIMER_NSEC 1000

ActivityLightClient::ActivityLightClient(int channelId)
		: Client(channelId)
{
	for (int i = 0; i < BLINK_ARRAY_SIZE; i++)
	{
		blink[i] = false;
		acLightStatus[i] = false;
	}

}

ActivityLightClient::~ActivityLightClient()
{
}

//TODO umbauen: Ein timer läuft durchgängig. Controller entscheidet ob auf die Impulse reagiert wird!
void ActivityLightClient::work()
{

	//bool check = false;
	switch (msg.senderId)
	{
	case HARDWARE_CONTR:
		blink[BLINK_RED] = msg.blink[BLINK_RED];
		blink[BLINK_YELLOW] = msg.blink[BLINK_YELLOW];
		blink[BLINK_GREEN] = msg.blink[BLINK_GREEN];

		if (!blink[BLINK_RED])
		{
			acLightStatus[BLINK_RED] = msg.actorStatus[MSG_ACTLAMP_RED];
		} else
		{
			msg.actorStatus[MSG_ACTLAMP_RED] = acLightStatus[BLINK_RED];
		}
		if (!blink[BLINK_YELLOW])
		{
			acLightStatus[BLINK_YELLOW] = msg.actorStatus[MSG_ACTLAMP_YELLOW];
		} else
		{
			msg.actorStatus[MSG_ACTLAMP_YELLOW] = acLightStatus[BLINK_YELLOW];
		}
		if (!blink[BLINK_GREEN])
		{
			acLightStatus[BLINK_GREEN] = msg.actorStatus[MSG_ACTLAMP_GREEN];
		} else
		{
			msg.actorStatus[MSG_ACTLAMP_GREEN] = acLightStatus[BLINK_GREEN];
		}

		msg.receiverId = HARDWARE_CONTR;
		break;

	case PULSE_RECIEVER:

		for (int i = 0; i < BLINK_ARRAY_SIZE; i++)
		{
			if (blink[i])
			{
				//check = true;
				if (acLightStatus[i])
				{
					acLightStatus[i] = false;

				} else
				{
					acLightStatus[i] = true;
				}
				//msg.actorStatus[i] = acLightStatus[i];
			}
		}

		msg.actorStatus[MSG_ACTLAMP_RED] = acLightStatus[BLINK_RED];
		msg.actorStatus[MSG_ACTLAMP_YELLOW] = acLightStatus[BLINK_YELLOW];
		msg.actorStatus[MSG_ACTLAMP_GREEN] = acLightStatus[BLINK_GREEN];
//		if(check)
//		{
//			msg.receiverId = HARDWARE_CONTR;
//		}
//		else
//		{
//			msg.receiverId = EMPTY;
//		}
		msg.receiverId = HARDWARE_CONTR;
		break;

	default:
		msg.receiverId = EMPTY;
		break;
	}

	msg.senderId = ACLIGHT_CONTR;
}

} /* namespace communication */
