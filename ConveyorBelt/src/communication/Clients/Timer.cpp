/*
 * Timer.cpp
 *
 *  Created on: 01.05.2018
 *      Author: LuisNickel
 */

#include "Timer.h"
#include <sys/neutrino.h>
#include "../../hardware/Bib/Util.h"
#include "../PulseId.h"
#include <iostream>
#include <sys/netmgr.h>

#define TIKTIME 10000000
#define ACTIVITYLIGHTTIME 1

namespace communication {

Timer::Timer(int channelId,int pulseChannel):
		Client(channelId),
		pulseChannel_(pulseChannel)
{


	activityLightId_=setTimer(TIMERAL,0,0,ACTIVITYLIGHTTIME,ACTIVITYLIGHTTIME);

	tikId_=setTimer(TIMERTIK,TIKTIME,TIKTIME,0,0);
//
//	setTimer(TIMERPUK, 0, 0, 5, 1);
}

Timer::~Timer() {
	timer_delete(activityLightId_);
	timer_delete(tikId_);
}

void Timer::work(){


	msg.senderId = TIMER;
	msg.receiverId = EMPTY;
}



timer_t Timer::setTimer(short int type,int nsec,int intervalNsec,int sec,int intervalSec){
  struct sigevent event;
  struct itimerspec timer;
  timer_t timerId;

  int connectionId = ConnectAttach_r(0, 0, pulseChannel_, 0, 0);
  	if (connectionId < 0) {
  		DBG_ERROR("Fail Channel create:Timer" + connectionId);
  		exit (-1);
  	}
  	SIGEV_PULSE_INIT(&event,connectionId,0,type ,0);
  	if(timer_create(CLOCK_REALTIME,&event,&timerId)==-1){
  		  std::cout<<"Failed to create timer"<<std::endl;
  	  }

  timer.it_value.tv_sec=sec;
  timer.it_value.tv_nsec=nsec;
  timer.it_interval.tv_sec=intervalSec;//intervalSec;
  timer.it_interval.tv_nsec=intervalNsec;

  if(timer_settime(timerId,0,&timer,NULL)==-1){
  	  std::cout<<"Failed to set timer id:"<<timerId<<std::endl;
   }

  return timerId;
}




} /* namespace communication */
