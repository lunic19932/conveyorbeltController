/*
 * Dispatcher.cpp
 *
 *  Created on: 28.04.2018
 *      Author: Johannes Kruber
 */

#include "Dispatcher.h"
#include <sys/neutrino.h>
#include <thread>
#include <iostream>


namespace communication
{

Dispatcher::Dispatcher()
		: channelId(-1), running(true), sensorsInitialised(false)
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
	msg.sensorData[MSG_BTN_EMERGENCY] = true;

	msg.hightSensor = 0;
	msg.receiverId = -1;


	for (int i = 0; i < COUNT; i++)
	{
		clients[i] = -1;
	}
}

Dispatcher::~Dispatcher()
{

}

void Dispatcher::operator ()()
{
	channelId = ChannelCreate(0);
	if (channelId < 0)
	{
		std::cout << "Dispatcher channel could not be created" << std::endl;
		return;
	}
	while (running)
	{
		receiveMsg();
		//sleep(2);
	}

	ChannelDestroy(channelId);
}

/**
 * Receives Msg and forward it to receiver.
 * If Receiver is not available it builds up a queu via selfcall
 */
void Dispatcher::receiveMsg()
{
	int rcvid;
	//int senderId, receiverId;
	rcvid = MsgReceive(channelId, &msg, sizeof(msg), NULL);
	//std::cout << "MSG received"<<std::endl;
	//std::cout << "Sender: " << msg.senderId << std::endl;




	if (msg.senderId < COUNT)
	{
		clients[msg.senderId] = rcvid;
	} else
	{
		//Fehler
	}

	if(msg.senderId == HARDWARE_CONTR && !sensorsInitialised)
	{
		msg.receiverId = HARDWARE_CONTR;
		msg.senderId = PULSE_RECIEVER;
	}

	if(fifo.empty())
	{
		msgsave = msg;
	}
	else
	{
		msgsave = fifo.front();
	}
	fifo.push(msg);

	if(msg.senderId == PULSE_RECIEVER && sensorsInitialised)
	{
		MsgReply(clients[msg.senderId], 0, &msgsave, sizeof(msg));
		clients[msg.senderId] = -1;
	}
	else
	{
		sensorsInitialised = true;
	}

	int size = fifo.size();
	//std::cout << "size: " << size <<std::endl;
	for(int i = 0; i < size && !fifo.empty(); i++)
	{
		msg = fifo.front();
		fifo.pop();

		//std::cout << "Sender: " << msg.senderId << " fifo: "<< i << std::endl;

		if (clients[msg.receiverId] == -1)
		{
			std::cout << "receiver not ready: " << msg.receiverId << std::endl
					<< "sender: " << msg.senderId << std::endl;
			fifo.push(msg);
		}
		else
		{
			MsgReply(clients[msg.receiverId], 0, &msg, sizeof(msg));
		}
	}
	/*
	if (receiverId >= 0 && receiverId < COUNT)
	{
		if (clients[receiverId] == -1)
		{
			std::cout << "receiver not ready: "<< receiverId << std::endl
					<< "sender: "<< senderId << std::endl;
			receiveMsg();
		}
		//log();
		msg.receiverId = receiverId;
		msg.senderId = senderId;
		MsgReply(clients[receiverId], 0, &msg, sizeof(msg));
		clients[receiverId] = -1;
	}
	*/

}

void Dispatcher::log()
{
	std::cout<< "Sender ID: " << msg.senderId << std::endl
			<< "Receiver ID: "<< msg.receiverId << std::endl;

	for (int i = 0; i < ACTOR_ARRAY_SIZE; i++)
	{
		std::cout << "Aktor " << i << ": " << msg.actorStatus[i] << std::endl;
	}

	for (int i = 0; i < SENSOR_ARRAY_SIZE; i++)
	{
		std::cout << "Sensor " << i << ": " << msg.sensorData[i] << std::endl;
	}




}

void Dispatcher::stop()
{
	running = false;
}

int Dispatcher::getChannelId()
{
	return channelId;
}

} /* namespace communication */
