/*
 * PulseReceiver.cpp
 *
 *  Created on: 30.04.2018
 *      Author: Luisn
 */

#include "PulseReceiver.h"
#include "../PulseId.h"
#include "../ClientId.h"
#include "../../hardware/Bib/Util.h"
#include <sys/neutrino.h>
#include <iostream>


namespace communication {
PulseReceiver::PulseReceiver(int channelId) :
		Client(channelId) {
	channelId_ = ChannelCreate_r(0);
	if (channelId_ < 0) {
		DBG_ERROR("Fail Channel create" + channelId_);
		exit (-1);
	}
	counter_=0;
}

PulseReceiver::~PulseReceiver() {

}

int PulseReceiver::getChannelId() {
	return channelId_;
}

void PulseReceiver::work() {
	int ret;
	struct _pulse pulse;
	ret = MsgReceive(channelId_, &pulse, sizeof(pulse), NULL);
	if (ret < 0) {
		DBG_ERROR("Fail MsgReceivePulse: " + ret);
		exit (EXIT_FAILURE);
	}
	msg.senderId=PULSE_RECIEVER;
	switch(pulse.code){
	case ISR:
		//std::cout << "ISR" << std::endl;
		msg.receiverId=HARDWARE_CONTR;
		if(pulse.value.sival_int==ISRGPIO){
			msg.pulseType=MSG_PULS_GPIOISR;
		}else if(pulse.value.sival_int==ISRADC){
			msg.pulseType=MSG_PULS_ADCISR;
		}
		break;
	case TIMERAL:
		//std::cout << "Timer al "<< msg.blink[1] << std::endl;
		msg.receiverId=ACLIGHT_CONTR;
		msg.pulseType=MSG_PULS_ALTIMER;
		break;
	case TIMERTIK:
		//std::cout << "tick" << std::endl;
		msg.receiverId=MAIN_CNTR;
		msg.pulseType=MSG_PULS_TIKTIMER;
		break;
	case SERIAL:
		//std::cout << "Serial" << std::endl;
		msg.receiverId=MAIN_CNTR;
		msg.pulseType=MSG_PULS_SERIAL;
		break;
	default:
		break;
		//std::cout<< "PulseMsg received" << std::endl;
	}

}

}
