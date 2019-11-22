/*
 * StateMachineCalibration.cpp
 *
 *  Created on: 09.06.2018
 *      Author: LuisNickel
 */

#include "StateMachineCalibration.h"
#include <iostream>
//PUKHIGHT/25000
namespace automaton {

#define MAX_HIGHT_RESULTS 20
#define HIGHT_TOLERANCE 100
#define PUK_HIGHT_MICMCALC		25000

StateMachineCalibration::StateMachineCalibration(
		struct CalibrationData* calData) :
		StateMachine(calData), tikCounter(0), fastTiksToLeaveStart(0), fastTiksToHightBest(
				0), fastTiksToHightWorst(0), fastTiksToReachSwitch(0), fastTiksToReachEnd(
				0), slowTiksToReachEnd(0), tiksToReachRamp(0), tiksToPassRamp(
				0), actualCalState(initCal), lastCalState(initCal), stateString(
				"initCal") {

}

StateMachineCalibration::~StateMachineCalibration() {

}
bool StateMachineCalibration::checkError(void) {
	return false;
}
void StateMachineCalibration::makeTikStep(void) {
	tikCounter++;

}
void StateMachineCalibration::calculateCalibration() {
	calData->fastStep = (double) 100 / fastTiksToReachEnd;
	calData->slowStep = (double) 100 / slowTiksToReachEnd;
	calData->reach_lb_HightBest = calData->fastStep * fastTiksToHightBest;
	calData->reach_lb_HightWorst = calData->fastStep * fastTiksToHightWorst;
	calData->reach_lb_Switch = calData->fastStep * fastTiksToReachSwitch;
	calData->leave_lb_Ramp = calData->fastStep * tiksToPassRamp;
	calData->reach_lb_Ramp = calData->fastStep * tiksToReachRamp;
	calData->halfPukLength = calData->fastStep * fastTiksToLeaveStart;

	int hightNoPuk = 0;
	int hightPuk = 0;
	while(hightRawValues.size() != 0)
	{
		if(hightRawValues.size() > MAX_HIGHT_RESULTS)
		{
			hightNoPuk += hightRawValues.front();
			hightRawValues.pop();
		}
		else
		{
			hightPuk += hightRawValues.front();
			hightRawValues.pop();
		}
	}
	hightNoPuk /= MAX_HIGHT_RESULTS;
	hightPuk /= MAX_HIGHT_RESULTS;

	calData->hight_noPukRaw = hightNoPuk;
	std::cout << "hight puk " << hightPuk << std::endl;
	calData->hight_normQuotient = (double) (hightNoPuk - hightPuk)/PUK_HIGHT_MICMCALC;

}
char StateMachineCalibration::getType(void) {
	return STATEMACHINECALIBRATION;
}
void StateMachineCalibration::updateStates(void) {
		switch (actualCalState) {
		case initCal:
			if (!sensorData.sn_lb_start && sensorData.btn_start) {
				actualCalState = startWorstcase;
				tikCounter = 0;
			}
			break;
		case startWorstcase:
			if (sensorData.sn_lb_start) {
				fastTiksToLeaveStart = tikCounter;
				actualCalState = fastWorstCase;
			}
			break;
		case fastWorstCase:
			if (!sensorData.sn_lb_hightSn) {
				actualCalState = backToStart1;
				fastTiksToHightWorst = tikCounter;
			}
			break;
		case backToStart1:
			if (!sensorData.sn_lb_start) {
				actualCalState = fastBestCase;
				tikCounter = 0;
			}
			break;
		case fastBestCase:
			if (sensorData.sn_lb_start) {
				actualCalState = toHight;
			}
			break;

		case toHight:
			if (!sensorData.sn_lb_hightSn) {
				actualCalState = toSwitch;
				fastTiksToHightBest = tikCounter;
			}
			break;

		case toSwitch:
			if (!sensorData.sn_lb_switch) {
				actualCalState = toEnd;
				fastTiksToReachSwitch = tikCounter;
			}
			break;

		case toEnd:
			if (!sensorData.sn_lb_end) {
				actualCalState = backToStart2;
				fastTiksToReachEnd = tikCounter;

			}
			break;

		case backToStart2:
			if (!sensorData.sn_lb_start) {
				actualCalState = slowRun;
				tikCounter = 0;
			}
			break;
		case slowRun:
			if (!sensorData.sn_lb_end) {
				actualCalState = backToStart3;
				slowTiksToReachEnd = tikCounter;
			}
			break;

		case backToStart3:
			if (!sensorData.sn_lb_start) {
				actualCalState = toRamp;
				tikCounter = 0;
			}
			break;
		case toRamp:
			if (!sensorData.sn_lb_ramp) {
				actualCalState = waitForLeaveRamp;
				tiksToReachRamp = tikCounter;
			}
			break;

		case waitForLeaveRamp:
			if (sensorData.sn_lb_ramp) {
				actualCalState = endCal;
				tiksToPassRamp = tikCounter;
				calculateCalibration();
				machineComplete=true;
			}
			break;
		case endCal:

			break;

		}
}

void StateMachineCalibration::runStates(void) {
	switch (actualCalState) {
	case initCal:
		actorState[MSG_MOTOR_RIGHT] = false;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = true;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;
		stateString = "initCal";
		break;
	case startWorstcase:
		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;

		stateString = "startWorstCase";
		break;
	case fastWorstCase:
		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;
		stateString = "fastRun1";

		break;
	case backToStart1:
		actorState[MSG_MOTOR_RIGHT] = false;
		actorState[MSG_MOTOR_LEFT] = true;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;

		stateString = "backToStart1";
		break;

	case fastBestCase:

		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;
		stateString = "fastRun2";
		adcEnable = true;

		break;
	case toHight:
		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;
		addHigth();

		stateString = "toHight";
		break;

	case toSwitch:
		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;
		if(sensorData.sn_lb_hightSn)
		{
			adcEnable = false;
		}
		else
		{
			addHigth();
		}
		stateString = "toSwitch";
		break;

	case toEnd:
		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = true;

		stateString = "toEnd1";
		break;
	case backToStart2:
		actorState[MSG_MOTOR_RIGHT] = false;
		actorState[MSG_MOTOR_LEFT] = true;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = true;

		stateString = "backToStart2";
		break;
	case slowRun:
		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = true;
		actorState[MSG_SWITCH] = true;

		stateString = "slowRun";

		break;

	case backToStart3:
		actorState[MSG_MOTOR_RIGHT] = false;
		actorState[MSG_MOTOR_LEFT] = true;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = true;

		stateString = "backToStart3";
		break;
	case toRamp:
		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;

		stateString = "toRamp";
		break;

	case waitForLeaveRamp:
		actorState[MSG_MOTOR_RIGHT] = false;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = true;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;


		stateString = "waitForLeaveRamp";
		break;

	case endCal:
		actorState[MSG_MOTOR_RIGHT] = false;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = true;
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = false;
		stateString = "endCal";
		break;

	}
	if (lastCalState != actualCalState) {
//		std::cout << "actualCalState: " << stateString << std::endl;
//
//		std::cout << "actor MotorRight: " << actorState[MSG_MOTOR_RIGHT]
//				<< std::endl;
//		std::cout << "actor MotorLeft: " << actorState[MSG_MOTOR_LEFT]
//				<< std::endl;
//		std::cout << "actor MotorStop: " << actorState[MSG_MOTOR_STOP]
//				<< std::endl;
//		std::cout << "actor MotorSlow: " << actorState[MSG_MOTOR_SLOW]
//				<< std::endl;
//		std::cout << "actor SwitchOpen: " << actorState[MSG_SWITCH] << "\n"
//				<< std::endl;

		if (actualCalState == endCal) {
			std::cout << "TIKSTOLEAVESTART: " << fastTiksToLeaveStart<< std::endl;
						std::cout << "tiksbestCaseHight: " << fastTiksToHightBest << std::endl;
						std::cout << "tiksWorstCaseHight:" << fastTiksToHightWorst
								<< std::endl;
						std::cout << "tiksReachSwitch: "
								<< fastTiksToReachSwitch << std::endl;
						std::cout << "fastTiksToReachEnd: "
								<< fastTiksToReachEnd << std::endl;
						std::cout << "slowTiksToReachEnd: " << slowTiksToReachEnd
								<< std::endl;
						std::cout << "tiksToReachRamp: " << tiksToReachRamp
								<< std::endl;
						std::cout << "tiksToPassRamp: " << tiksToPassRamp<< "\n"
								<< std::endl;

			std::cout << "FastStep / %: " << calData->fastStep << std::endl;
			std::cout << "SlowStep / %: " << calData->slowStep << std::endl;
			std::cout << "halfPukLength:" << calData->halfPukLength
					<< std::endl;
			std::cout << "Reach Hight WorseCase/ %: "
					<< calData->reach_lb_HightWorst << std::endl;
			std::cout << "Reach Hight BestCase/ %: "
					<< calData->reach_lb_HightBest << std::endl;
			std::cout << "Reach Switch/ %: " << calData->reach_lb_Switch
					<< std::endl;
			std::cout << "Reach Ramp/ %: " << calData->reach_lb_Ramp
					<< std::endl;
			std::cout << "Leave Ramp/ %: " << calData->leave_lb_Ramp
					<< std::endl;
			std::cout << "Quotient: " << calData->hight_normQuotient
								<< std::endl;
			std::cout << "NoPukRaw: " << calData->hight_noPukRaw
											<< std::endl;

		}
	}
	lastCalState = actualCalState;

}

void StateMachineCalibration::addHigth()
{
	if(pulseType == MSG_PULS_ADCISR)
	{
		if(hightRawValues.size() == 0)
		{
			hightRawValues.push(hightvalue);
		}
		else if((hightvalue <= hightRawValues.front() + HIGHT_TOLERANCE || hightvalue >= hightRawValues.front() - HIGHT_TOLERANCE) && hightRawValues.size() < MAX_HIGHT_RESULTS)
		{
			hightRawValues.push(hightvalue);
			std::cout << "Push no Puk" << hightRawValues.size() << " "<< hightvalue << std::endl;
		}
		else if(hightRawValues.size() >= MAX_HIGHT_RESULTS &&(!sensorData.sn_lb_hightSn /*hightvalue+HIGHT_TOLERANCE  < hightRawValues.front()- HIGHT_TOLERANCE */) && hightRawValues.size() < MAX_HIGHT_RESULTS*2)
		{
			hightRawValues.push(hightvalue);
			std::cout << "Push Puk" << hightRawValues.size() << " "<< hightvalue << std::endl;
		}
		std::cout << "actual hight " << hightvalue<< std::endl;
	}
}

void StateMachineCalibration::reset()
{
	StateMachine::reset();
	actualCalState=initCal;
	//TODO Reset StateMachineCalibration
}

} /* namespace automaton */
