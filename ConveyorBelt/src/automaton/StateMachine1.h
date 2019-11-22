/*
 * StateMachine4.h
 *
 *  Created on: 14.06.2018
 *      Author: LuisNickel
 */

#ifndef AUTOMATON_STATEMACHINE1_H_
#define AUTOMATON_STATEMACHINE1_H_
#include <deque>
#include "PukStateMachine.h"
#include "StateMachine.h"

namespace automaton {

class StateMachine1: public StateMachine {
protected:
	virtual void updateStates(void);
	virtual void runStates(void);
public:
	StateMachine1(struct CalibrationData* calData);
	virtual ~StateMachine1();

private:
	std::deque<PukStateMachine> queueLbStart;
	std::deque<PukStateMachine> queueLbHight;
	std::deque<PukStateMachine> queueLbSwitch;
	std::deque<PukStateMachine> queueLbRamp;
	std::deque<PukStateMachine> queueLbEnd;
	PukStateMachine* outPuk;
	bool old_sn_lb_start;
	bool error;
	int pukId;
	int systemTime;
	virtual void makeTikStep(void);
	virtual char getType(void);
	virtual void reset(void);
	virtual bool checkError(void);
	void updateQueueLsStart(void);
	void updateQueueLsHight(void);
	void updateQueueLsSwitch(void);
	void updateQueueLsRamp(void);
	void updateQueueLsEnd(void);
	bool isConveyerBeltEmpty(void);
	bool isBeltEmpty(void);
	void setActors(void);
	void setActorsForStartQueue(void);
	void setActorsForHightQueue(void);
	void setActorsForSwitchQueue(void);
	void setActorsForRampQueue(void);
	void setActorsForEndQueue(void);
	void updateProgressQueueStart(void);
	void updateProgressQueueHight(void);
	void updateProgressQueueSwitch(void);
	void updateProgressQueueRamp(void);
	void updateProgressQueueEnd(void);
	void updatePukProgress(void);

	double getSpeed(void);
};

} /* namespace automaton */

#endif /* AUTOMATON_STATEMACHINE1_H_ */
