/*
 * Timer.h
 *
 *  Created on: 01.05.2018
 *      Author: LuisNickel
 */

#ifndef COMMUNICATION_CLIENTS_TIMER_H_
#define COMMUNICATION_CLIENTS_TIMER_H_
#include "../Client.h"
#include <signal.h>
#include <time.h>
#define TIMERPOOLSIZE 100
namespace communication {

class Timer: public Client {
public:
	Timer(int channelId,int pulseChannel);
	virtual ~Timer();


protected:
	virtual void work();
private:
	timer_t setTimer(short int type,int nsec,int intervalNsec,int sec, int intervalSec);
	timer_t tikId_;
	timer_t activityLightId_;
	void initTimer();
	int deleteTimer(int timerId);
	int pulseChannel_;


};

} /* namespace communication */

#endif /* COMMUNICATION_CLIENTS_TIMER_H_ */
