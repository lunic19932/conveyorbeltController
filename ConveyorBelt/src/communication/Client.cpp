/*
 * Client.cpp
 *
 *  Created on: 28.04.2018
 *      Author: Johannes Kruber
 */

#include "Client.h"
#include <iostream>
#include "ClientId.h"
#include <sys/neutrino.h>

namespace communication
{

Client::Client(int channelId)
		: running(true), channelId(channelId), connectionId(-1)
{
	msg.senderId = -1;
	msg.receiverId = -1;
	for (int i = 0; i < ACTOR_ARRAY_SIZE; i++)
	{
		msg.actorStatus[i] = false;
	}

	for (int i = 0; i < SENSOR_ARRAY_SIZE; i++)
	{
		msg.sensorData[i] = false;
	}

	for (int i = 0; i < BLINK_ARRAY_SIZE; i++)
	{
		msg.blink[i] = false;
	}
	msg.sensorData[MSG_BTN_EMERGENCY] = true;
	msg.hightSensor = 0;
	msg.pulseType = -1;



}

Client::~Client()
{

}

void Client::operator ()()
{
	int status;
	connectionId = ConnectAttach(0, 0, channelId, 0, 0);


	while (running)
	{
		work();
		status = MsgSend(connectionId, &msg, sizeof(msg), &msg, sizeof(msg));
	}

	ConnectDetach(connectionId);
}

void Client::stop()
{
	running = false;
}

void Client::work()
{
	std::cout
			<< "I am the standard Client work function. i will set everything to 1/true"
			<< std::endl;

	msg.senderId = 1;
	msg.receiverId = 0;
	for (int i = 0; i < ACTOR_ARRAY_SIZE; i++)
	{
		msg.actorStatus[i] = true;
	}

	for (int i = 0; i < SENSOR_ARRAY_SIZE; i++)
	{
		msg.sensorData[i] = true;
	}

	msg.hightSensor = 1;
	msg.receiverId = 7;


}

} /* namespace communication */
