/*
 * StateMachine1.cpp
 *
 *  Created on: 14.06.2018
 *      Author: LuisNickel
 */

#include "StateMachine1.h"
#define MAX_PUK_SIZE 11
#define IDLE 0
namespace automaton {

StateMachine1::StateMachine1(struct CalibrationData* calData) :
		StateMachine(calData), outPuk(NULL), old_sn_lb_start(true), error(
				false), pukId(0),systemTime(0) {
	puk.id=-1;
}

StateMachine1::~StateMachine1() {
	// TODO Auto-generated destructor stub
}

void StateMachine1::updateStates() {
	//check new Puk
	if (!sensorData.sn_lb_start
			&& (sensorData.sn_lb_start != old_sn_lb_start)) {
		queueLbStart.push_back(
				PukStateMachine(pukId++, calData, &otherStateMachine,&rampFull,systemTime));
	}
	if(sensorData.sn_lb_ramp){
		rampFull=false;
	}

	if (!queueLbStart.empty()) {
		queueLbStart.front().updateStates(sensorData);
	}
	if (!queueLbHight.empty()) {
		queueLbHight.front().updateStates(sensorData);
	}
	if (!queueLbSwitch.empty()) {
		queueLbSwitch.front().updateStates(sensorData);
	}
	if (!queueLbRamp.empty()) {
		queueLbRamp.front().updateStates(sensorData);
	}
	if (!queueLbEnd.empty()) {
		queueLbEnd.front().updateStates(sensorData);
	}
	if (outPuk != NULL) {
		outPuk->updateStates(sensorData);
	}

	updateQueueLsStart();
	updateQueueLsHight();
	updateQueueLsSwitch();
	updateQueueLsRamp();
	updateQueueLsEnd();

	old_sn_lb_start = sensorData.sn_lb_start;

}

void StateMachine1::updateQueueLsStart() {

	if (!queueLbStart.empty()) {
		if (queueLbStart.front().getState() == transToHs) {
			queueLbHight.push_back(queueLbStart.front());
			queueLbStart.pop_front();
		}
	}
}
void StateMachine1::updateQueueLsHight() {
	if (!queueLbHight.empty()) {

		if (queueLbHight.front().getState() == transToMs
				|| queueLbHight.front().getState() == toRamp1) {
			queueLbHight.front().setPukCode(
					hightMeasureStatmachine.getResult(),
					hightMeasureStatmachine.getHightValuesOutput(0),
					hightMeasureStatmachine.getHightValuesOutput(1),
					hightMeasureStatmachine.getHightValuesOutput(2)
					);
			queueLbSwitch.push_back(queueLbHight.front());
			queueLbHight.pop_front();
			adcEnable = false;
			hightMeasureStatmachine.reset();
		}

	}
}
void StateMachine1::updateQueueLsSwitch() {
	if (!queueLbSwitch.empty()) {

		if (queueLbSwitch.front().getState() == ramp) {
			queueLbRamp.push_back(queueLbSwitch.front());
			//std::cout << "Pushed from Switch to Ramp" << std::endl;
			queueLbSwitch.pop_front();

		} else if (queueLbSwitch.front().getState() == transToLs2) {
			queueLbEnd.push_back(queueLbSwitch.front());
			queueLbSwitch.pop_front();
		}

	}
}
void StateMachine1::updateQueueLsRamp() {
	if (!queueLbRamp.empty()) {
		//std::cout << "ramp update " << queueLbRamp.front().getState() << std::endl;
		if (queueLbRamp.front().getState() == end ) {
			queueLbRamp.pop_front();
		}
		else if(queueLbRamp.front().getState() == PukState::rampFull){
			rampFull=true;
//			std::cout << "pop ramp" << std::endl;
			queueLbRamp.pop_front();
		}

	}
}
void StateMachine1::updateQueueLsEnd() {
	if (outPuk != NULL && outPuk->getState() == end) {
//		std::cout << "set  OutPuk NULL: " << outPuk->getPukID() << std::endl;
		outPuk = NULL;
	} else {
		//std::cout<<"2 OutPuk: NULL"<<std::endl;
	}
	if (!queueLbEnd.empty()) {
		if (outPuk == NULL && queueLbEnd.front().getState() == transToFesto2) {
			outPuk = &queueLbEnd.front();
			puk=outPuk->getPuk();
			queueLbEnd.pop_front();

		}
	}

}

void StateMachine1::runStates() {
	updateQueueLsStart();
	updateQueueLsHight();
	updateQueueLsSwitch();
	updateQueueLsRamp();
	updateQueueLsEnd();
	if (adcEnable) {
		hightMeasureStatmachine.updateStates(hightvalue);
		hightMeasureStatmachine.runStates();
	}
	actorState[MSG_MOTOR_RIGHT] = false;
	actorState[MSG_MOTOR_LEFT] = false;
	actorState[MSG_MOTOR_SLOW] = false;
	actorState[MSG_MOTOR_STOP] = false;
	actorState[MSG_SWITCH] = false;
	if (isBeltEmpty()) {
		actorState[MSG_MOTOR_STOP] = true;
	}

	setActors();

}
void StateMachine1::updatePukProgress() {
	setActors();
	updateProgressQueueStart();
	updateProgressQueueHight();
	updateProgressQueueSwitch();
	updateProgressQueueRamp();
	updateProgressQueueEnd();

}
void StateMachine1::setActors() {
	setActorsForStartQueue();
	setActorsForHightQueue();
	setActorsForSwitchQueue();
	setActorsForRampQueue();
	setActorsForEndQueue();
}
bool StateMachine1::isBeltEmpty() {
	return queueLbStart.empty() && queueLbHight.empty() && queueLbSwitch.empty()
			&& queueLbRamp.empty() && queueLbEnd.empty() && outPuk == NULL;
}
void StateMachine1::setActorsForStartQueue(void) {
	for (std::deque<PukStateMachine>::iterator it = queueLbStart.begin();
			it != queueLbStart.end(); ++it) {
		queueLbStart.front().runStates();
		actorState[MSG_MOTOR_RIGHT] = actorState[MSG_MOTOR_RIGHT]
				|| queueLbStart.front().getPukActor(MSG_MOTOR_RIGHT);
		actorState[MSG_MOTOR_LEFT] = actorState[MSG_MOTOR_LEFT]
				|| queueLbStart.front().getPukActor(MSG_MOTOR_LEFT);
		actorState[MSG_MOTOR_SLOW] = actorState[MSG_MOTOR_SLOW]
				|| queueLbStart.front().getPukActor(MSG_MOTOR_SLOW);
		actorState[MSG_MOTOR_STOP] = actorState[MSG_MOTOR_STOP]
				|| queueLbStart.front().getPukActor(MSG_MOTOR_STOP);
	}

	if (!queueLbSwitch.empty()
			&& queueLbSwitch.front().getPukActor(MSG_SWITCH)) {
		actorState[MSG_SWITCH] = true;
	} else if (!queueLbEnd.empty()
			&& queueLbEnd.back().getPukActor(MSG_SWITCH)) {
		actorState[MSG_SWITCH] = true;
	} else {
		actorState[MSG_SWITCH] = false;
	}

}
void StateMachine1::setActorsForHightQueue(void) {
	for (std::deque<PukStateMachine>::iterator it = queueLbHight.begin();
			it != queueLbHight.end(); ++it) {
		queueLbHight.front().runStates();
		actorState[MSG_MOTOR_RIGHT] = actorState[MSG_MOTOR_RIGHT]
				|| queueLbHight.front().getPukActor(MSG_MOTOR_RIGHT);
		actorState[MSG_MOTOR_LEFT] = actorState[MSG_MOTOR_LEFT]
				|| queueLbHight.front().getPukActor(MSG_MOTOR_LEFT);
		actorState[MSG_MOTOR_SLOW] = actorState[MSG_MOTOR_SLOW]
				|| queueLbHight.front().getPukActor(MSG_MOTOR_SLOW);
		actorState[MSG_MOTOR_STOP] = actorState[MSG_MOTOR_STOP]
				|| queueLbHight.front().getPukActor(MSG_MOTOR_STOP);
		actorState[MSG_SWITCH] = actorState[MSG_SWITCH]
				|| queueLbHight.front().getPukActor(MSG_SWITCH);
	}
	if (!queueLbHight.empty() && queueLbHight.front().getState() == hs) {
		adcEnable = true;
	}

}
void StateMachine1::setActorsForSwitchQueue(void) {
	for (std::deque<PukStateMachine>::iterator it = queueLbSwitch.begin();
			it != queueLbSwitch.end(); ++it) {
		queueLbSwitch.front().runStates();
		actorState[MSG_MOTOR_RIGHT] = actorState[MSG_MOTOR_RIGHT]
				|| queueLbSwitch.front().getPukActor(MSG_MOTOR_RIGHT);
		actorState[MSG_MOTOR_LEFT] = actorState[MSG_MOTOR_LEFT]
				|| queueLbSwitch.front().getPukActor(MSG_MOTOR_LEFT);
		actorState[MSG_MOTOR_SLOW] = actorState[MSG_MOTOR_SLOW]
				|| queueLbSwitch.front().getPukActor(MSG_MOTOR_SLOW);
		actorState[MSG_MOTOR_STOP] = actorState[MSG_MOTOR_STOP]
				|| queueLbSwitch.front().getPukActor(MSG_MOTOR_STOP);
		actorState[MSG_SWITCH] = actorState[MSG_SWITCH]
				|| queueLbSwitch.front().getPukActor(MSG_SWITCH);
	}


}
void StateMachine1::setActorsForRampQueue(void) {
	for (std::deque<PukStateMachine>::iterator it = queueLbRamp.begin();
			it != queueLbRamp.end(); ++it) {
		queueLbRamp.front().runStates();
		actorState[MSG_MOTOR_RIGHT] = actorState[MSG_MOTOR_RIGHT]
				|| queueLbRamp.front().getPukActor(MSG_MOTOR_RIGHT);
		actorState[MSG_MOTOR_LEFT] = actorState[MSG_MOTOR_LEFT]
				|| queueLbRamp.front().getPukActor(MSG_MOTOR_LEFT);
		actorState[MSG_MOTOR_SLOW] = actorState[MSG_MOTOR_SLOW]
				|| queueLbRamp.front().getPukActor(MSG_MOTOR_SLOW);
		actorState[MSG_MOTOR_STOP] = actorState[MSG_MOTOR_STOP]
				|| queueLbRamp.front().getPukActor(MSG_MOTOR_STOP);
		actorState[MSG_SWITCH] = actorState[MSG_SWITCH]
				|| queueLbRamp.front().getPukActor(MSG_SWITCH);
	}

}
void StateMachine1::setActorsForEndQueue(void) {
	for (std::deque<PukStateMachine>::iterator it = queueLbEnd.begin();
			it != queueLbEnd.end(); ++it) {
		queueLbEnd.front().runStates();
		actorState[MSG_MOTOR_RIGHT] = actorState[MSG_MOTOR_RIGHT]
				|| queueLbEnd.front().getPukActor(MSG_MOTOR_RIGHT);
		actorState[MSG_MOTOR_LEFT] = actorState[MSG_MOTOR_LEFT]
				|| queueLbEnd.front().getPukActor(MSG_MOTOR_LEFT);
		actorState[MSG_MOTOR_SLOW] = actorState[MSG_MOTOR_SLOW]
				|| queueLbEnd.front().getPukActor(MSG_MOTOR_SLOW);
		actorState[MSG_MOTOR_STOP] = actorState[MSG_MOTOR_STOP]
				|| queueLbEnd.front().getPukActor(MSG_MOTOR_STOP);
		actorState[MSG_SWITCH] = actorState[MSG_SWITCH]
				|| queueLbEnd.front().getPukActor(MSG_SWITCH);
	}

}
void StateMachine1::updateProgressQueueStart() {
	for (std::deque<PukStateMachine>::iterator it = queueLbStart.begin();
			it != queueLbStart.end(); ++it) {
		(*it).updateProcess(getSpeed());

	}

}
void StateMachine1::updateProgressQueueHight() {
	for (std::deque<PukStateMachine>::iterator it = queueLbHight.begin();
			it != queueLbHight.end(); ++it) {
		(*it).updateProcess(getSpeed());

	}
}
void StateMachine1::updateProgressQueueSwitch() {
	for (std::deque<PukStateMachine>::iterator it = queueLbSwitch.begin();
			it != queueLbSwitch.end(); ++it) {
		(*it).updateProcess(getSpeed());

	}
}
void StateMachine1::updateProgressQueueRamp() {
	for (std::deque<PukStateMachine>::iterator it = queueLbRamp.begin();
			it != queueLbRamp.end(); ++it) {
		(*it).updateProcess(getSpeed());

	}
}
void StateMachine1::updateProgressQueueEnd() {
	for (std::deque<PukStateMachine>::iterator it = queueLbEnd.begin();
			it != queueLbEnd.end(); ++it) {
		(*it).updateProcess(getSpeed());
	}
	if (outPuk != NULL) {
		outPuk->updateProcess(getSpeed());
	}

}

double StateMachine1::getSpeed(void) {
	if (actorState[MSG_MOTOR_STOP]) {
		return IDLE;
	} else if (actorState[MSG_MOTOR_SLOW] && actorState[MSG_MOTOR_RIGHT]) {
		return calData->slowStep;
	} else if (actorState[MSG_MOTOR_SLOW] && actorState[MSG_MOTOR_LEFT]) {
		return -calData->slowStep;
	} else if (actorState[MSG_MOTOR_LEFT]) {
		return -calData->fastStep;
	} else {
		return calData->fastStep;
	}

}
bool StateMachine1::checkError() {

	if (StateMachine::checkError()) {
		return true;
	}

	error = false;
	rampFull=false;

	if(adcEnable && hightvalue>PUK_HIGHT_MICM+PUK_HIGHT_TOLERANCE){
		return true;
	}
	if (!sensorData.sn_lb_start && !queueLbHight.empty()
			&& queueLbHight.back().getProgress() < calData->halfPukLength * 3) {
		error = true;
		std::cout << "Error" << std::endl;
	}
	if (queueLbHight.empty() && !sensorData.sn_lb_hightSn) {
		error = true;
//		std::cout << "Error Sensor 1" << std::endl;
	}
	if (queueLbSwitch.empty() && !sensorData.sn_lb_switch) {
		error = true;
//		std::cout << "Error Sensor 2" << std::endl;
	}
	if (queueLbRamp.empty() && !sensorData.sn_lb_ramp) {
		rampFull = true;
	}
	if (queueLbEnd.empty() && !sensorData.sn_lb_end) {
		error = true;
//		std::cout << "Error Sensor 3" << std::endl;
	}

	int size = queueLbEnd.size() + queueLbHight.size() + queueLbRamp.size()
			+ queueLbStart.size() + queueLbSwitch.size();
	if (size > MAX_PUK_SIZE) {
		return true;
	}
	for (std::deque<PukStateMachine>::iterator it = queueLbStart.begin();
			it != queueLbStart.end(); ++it) {
		if ((*it).checkError()) {
			return true;
		}
	}
	for (std::deque<PukStateMachine>::iterator it = queueLbHight.begin();
			it != queueLbHight.end(); ++it) {
		if ((*it).checkError()) {
			return true;
		}
	}
	for (std::deque<PukStateMachine>::iterator it = queueLbSwitch.begin();
			it != queueLbSwitch.end(); ++it) {
		if ((*it).checkError()) {
			return true;
		}
	}
	for (std::deque<PukStateMachine>::iterator it = queueLbRamp.begin();
			it != queueLbRamp.end(); ++it) {
		if ((*it).checkError()) {
			return true;
		}
	}
	for (std::deque<PukStateMachine>::iterator it = queueLbEnd.begin();
			it != queueLbEnd.end(); ++it) {
		if ((*it).checkError()) {
			return true;
		}
	}

	return error;
}

void StateMachine1::makeTikStep() {
	if(StateMachine::getActualState()==States::startMachine){
		updatePukProgress();
		runStates();
	}

	systemTime++;
}
char StateMachine1::getType() {
	return STATEMACHINE1;
}

void StateMachine1::reset() {
	StateMachine::reset();
	pukId = 0;
	error = false;
	queueLbStart.clear();
	queueLbHight.clear();
	queueLbSwitch.clear();
	queueLbEnd.clear();
	queueLbRamp.clear();

}

} /* namespace automaton */
