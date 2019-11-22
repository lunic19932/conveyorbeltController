/*
 * PukStateMachine.cpp
 *
 *  Created on: 04.06.2018
 *      Author: acd219
 */

#include "PukStateMachine.h"

#include <sstream>
#define FULLRUN	100
#define TOLERANCE 10
namespace automaton {
PukStateMachine::PukStateMachine(int pukID, struct CalibrationData* data,
		struct OtherStateMachine* festo2, bool* rampFull, int systemTime) :
		pukState(startPuk), festo2(festo2), calData(data), pukID(pukID), pukCode(
				0), tikCounter(0), progress(0), progressLs2(0), sortOut(true), hole(
				false), rampFull(rampFull)

{
	puk.id = pukID;
	puk.startTime = systemTime;
	for (int i = 0; i < ACTOR_ARRAY_SIZE; i++) {
		pukActor[i] = false;
	}
}
PukStateMachine::PukStateMachine() :
		PukStateMachine(0, new CalibrationData, new OtherStateMachine, new bool,
				0) {
}

PukStateMachine::~PukStateMachine() {
	// TODO Auto-generated destructor stub
}

void PukStateMachine::updateStates(SensorData sensorData) {
	std::ostringstream stream;

	switch (pukState) {
	case startPuk:
		pukState = ls1;
		sortOut = true;
		hole = false;
		break;

	case ls1:
		if (sensorData.sn_lb_start) {
			pukState = transToHs;
			//std::cout<<"PukId: "<<pukID<<" transToHs"<<std::endl;

		}
		break;

	case transToHs:
		if (!sensorData.sn_lb_hightSn) {
			pukState = hs;
//			std::cout<<"PukId: "<<pukID<<" hs"<<std::endl;
			if (!checkProcessIsInTime(calData->reach_lb_HightBest,
					calData->reach_lb_HightWorst, TOLERANCE)) {
				pukState = error;
//				std::cout << "Error hs" << std::endl;
			}
			progress = calData->reach_lb_HightBest;
			if (!sensorData.sn_hightControle) {
				hole = true;
			}
		}
		break;

	case hs:
		if (sensorData.sn_hightControle && hole) {
			sortOut = false;
		}
		if (!festo2->rampFull && *rampFull && sensorData.sn_lb_hightSn) {
			pukState = PukState::transToMs;
			sortOut = false;
			std::cout << "ramp to transto ms" << std::endl;
		} else if (sortOut && sensorData.sn_lb_hightSn) {
			pukState = toRamp1;

//			std::cout<<"PukId: "<<pukID<<" toRamp"<<std::endl;
		} else if (sensorData.sn_lb_hightSn) {
			pukState = transToMs;
		}
		break;

	case transToMs:
		if(pukCode>=0){
			std::cout << "StartTimer(s): " << puk.startTime/100 << " |PukID: "
					<< puk.id  << " |Code: " << puk.codeFirst
					<< " |TimeHight(s): " << (puk.startTime+puk.hightTikFirst)/100<< std::endl;
		}
		if (festo2->rampFull && pukCode >= 0) {
			pukState = toRamp1;
			std::cout << "PukId: " << pukID << " toRamp" << std::endl;
		} else if (pukCode == 1 || pukCode == 4) {
			pukState = toRamp1;
		} else if (!sensorData.sn_lb_switch) {

			if (festo2->rampFull
					&& (sensorData.sn_metall != festo2->metalExpected)) {
				pukState = toRamp1;
			} else {
				pukState = ms;
			}
		}
		break;

	case ms:
		if (sensorData.sn_lb_switch) {
			pukState = transToLs2;
			std::cout << "PukId: " << sensorData.sn_lb_switch << " transToLs2"
					<< std::endl;
		}
		break;

	case toRamp1:
		if (!sensorData.sn_lb_ramp) {
			pukState = ramp;
//			std::cout<<"PukId: "<<pukID<<" ramp"<<std::endl;
		}
		break;

	case ramp:
		if (sensorData.sn_lb_ramp) {
			pukState = end;

		}
		break;

	case transToLs2:
		if (!sensorData.sn_lb_end) {
			pukState = ls2;
//			std::cout<<"PukId: "<<pukID<<" ls2"<<std::endl;

		}
		break;

	case ls2:
		if (!festo2->beltFree && sensorData.sn_lb_end) {
			pukState = error;
			std::cout << "Puk disappear in Ls2" << std::endl;

		}
		if (festo2->beltFree && sensorData.sn_lb_end) {
			pukState = transToFesto2;
			festo2->beltFree = false;
//			std::cout << "PukCode" << pukCode << std::endl;
//			std::cout<<"PukId: "<<pukID<<" transToFesto2"<<std::endl;
			progressLs2 = progress;
			puk.tiksFirst = tikCounter;
		}
		break;

	case PukState::rampFull:
		if (sensorData.sn_lb_ramp) {
			pukState = end;
//			std::cout<<"PukId: "<<pukID<<" end"<<std::endl;
		}
		break;

	case transToFesto2:
		break;

	case end:
		break;

	case error:
		break;
	}
	if (stream.str().size() > 3) {
		//	std::cout << stream.str() << std::endl;
	}
}

void PukStateMachine::runStates() {
	switch (pukState) {
	case startPuk:
	case ls1:
	case transToHs:
	case hs:
	case transToMs:
	case toRamp1:
		pukActor[MSG_MOTOR_RIGHT] = true;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = false;
		pukActor[MSG_SWITCH] = false;
		break;

	case ms:
		pukActor[MSG_MOTOR_RIGHT] = true;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = false;
		pukActor[MSG_SWITCH] = false;
		if (!sortOut) {
			pukActor[MSG_SWITCH] = true;
		}
		break;

	case ramp:
		pukActor[MSG_MOTOR_RIGHT] = false;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = false;
		pukActor[MSG_SWITCH] = false;
		break;

	case transToLs2:
		pukActor[MSG_MOTOR_RIGHT] = true;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = false;
		pukActor[MSG_SWITCH] = true;
		if ((progress > (calData->reach_lb_Switch + 2 * calData->halfPukLength))) {
			pukActor[MSG_SWITCH] = false;
		}
		break;

	case ls2:
		pukActor[MSG_MOTOR_RIGHT] = false;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = true;
		//pukActor[MSG_SWITCH] = false;
		if (festo2->beltFree) {
			pukActor[MSG_MOTOR_RIGHT] = true;
			pukActor[MSG_MOTOR_STOP] = false;
		}
		break;

	case PukState::rampFull:
		pukActor[MSG_MOTOR_RIGHT] = false;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = false;
		pukActor[MSG_SWITCH] = false;
		break;

	case transToFesto2:
		pukActor[MSG_MOTOR_RIGHT] = true;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = false;
		//pukActor[MSG_SWITCH] = false;
		if (progress > progressLs2 + 2 * calData->halfPukLength) {
			pukState = end;
		}
		break;

	case end:
//		std::cout << "End" << std::endl;
		pukActor[MSG_MOTOR_RIGHT] = false;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = true;
		//	pukActor[MSG_SWITCH] = false;
		break;

	case error:
		pukActor[MSG_MOTOR_RIGHT] = false;
		pukActor[MSG_MOTOR_LEFT] = false;
		pukActor[MSG_MOTOR_SLOW] = false;
		pukActor[MSG_MOTOR_STOP] = true;
		pukActor[MSG_SWITCH] = false;
		break;
	}
}

void PukStateMachine::updateProcess(double speed) {
	progress += speed;
	tikCounter++;
	std::ostringstream stream;
	runStates();
}
double PukStateMachine::getProgress() {
	return progress;
}
bool PukStateMachine::checkProcessIsInTime(double minimalProgress,
		double maximalProgress, double tolerance) {
	if (((minimalProgress - tolerance) < progress)
			&& ((maximalProgress + tolerance) > progress)) {
		return true;
	}
	return false;
}

PukState PukStateMachine::getState() {
	return pukState;
}

int PukStateMachine::getPukID() {
	return pukID;
}

bool PukStateMachine::getPukActor(int index) {
	return pukActor[index];
}

void PukStateMachine::setPukCode(int pukCode, int hightValue1, int hightValue2,
		int hightValue3) {
	this->pukCode = pukCode;
	puk.codeFirst = pukCode;
	puk.hightFirst[0] = hightValue1;
	puk.hightFirst[1] = hightValue2;
	puk.hightFirst[2] = hightValue3;
	puk.hightTikFirst = tikCounter;
}

Puk PukStateMachine::getPuk() {
	return puk;
}
bool PukStateMachine::checkError() {
	switch (pukState) {
	case startPuk:
		break;

	case ls1:
		if (!checkProcessIsInTime(0, calData->halfPukLength * 2.5, TOLERANCE)) {
			std::cout << "Error ls1" << std::endl;
			std::cout << "minimalProgress: " << 0 << " maximalProgress: "
					<< calData->halfPukLength * 2.5 << " Progress: " << progress
					<< std::endl;
			pukState = error;
		}
		break;

	case transToHs:
		if (!checkProcessIsInTime(0, calData->reach_lb_HightWorst, TOLERANCE)) {
			pukState = error;
			std::cout << "Error transHs" << std::endl;
			std::cout << "minimalProgress: " << 0 << " maximalProgress: "
					<< calData->reach_lb_HightWorst << " Progress: " << progress
					<< std::endl;
		}
		break;

	case hs:
		if (!checkProcessIsInTime(calData->reach_lb_HightBest,
				calData->reach_lb_HightBest + 2 * calData->halfPukLength,
				TOLERANCE)) {
			std::cout << "Error hs" << std::endl;
			std::cout << "minimalProgress: " << calData->reach_lb_HightBest
					<< " maximalProgress: "
					<< calData->reach_lb_HightBest + 2 * calData->halfPukLength
					<< " Progress: " << progress << std::endl;
			pukState = error;
		}
		break;

	case transToMs:

		if (!checkProcessIsInTime(
				calData->reach_lb_HightBest + calData->halfPukLength,
				calData->reach_lb_Switch, TOLERANCE)) {
			std::cout << "Error transToMs" << std::endl;
			std::cout << "minimalProgress: "
					<< calData->reach_lb_HightBest + calData->halfPukLength
					<< " maximalProgress: " << calData->reach_lb_Switch
					<< " Progress: " << progress << std::endl;
			pukState = error;
		}
		break;

	case ms:
		if (!checkProcessIsInTime(calData->reach_lb_Switch,
				calData->reach_lb_Switch + 2 * calData->halfPukLength,
				TOLERANCE)) {
			pukState = error;
			std::cout << "Error ms" << std::endl;
			std::cout << "minimalProgress: " << calData->reach_lb_Switch
					<< " maximalProgress: "
					<< calData->reach_lb_Switch + 2 * calData->halfPukLength
					<< " Progress: " << progress << std::endl;
		}
		break;

	case toRamp1:
		if (!checkProcessIsInTime(calData->reach_lb_HightBest,
				calData->reach_lb_Ramp * 2, TOLERANCE)) {
			pukState = error;
			std::cout << "Error toRamp1" << std::endl;
			std::cout << "minimalProgress: " << calData->reach_lb_HightBest
					<< " maximalProgress: " << calData->reach_lb_Ramp
					<< " Progress: " << progress << std::endl;
		}
		break;

	case ramp:

		if (!checkProcessIsInTime(calData->reach_lb_Ramp,
				calData->leave_lb_Ramp, TOLERANCE)) {
			pukState = PukState::rampFull;
			std::cout << "rampFull" << std::endl;
			std::cout << "minimalProgress: " << calData->reach_lb_Ramp
					<< " maximalProgress: " << calData->leave_lb_Ramp
					<< " Progress: " << progress << std::endl;
		}
		break;

	case transToLs2:
		if (!checkProcessIsInTime(calData->reach_lb_Switch, FULLRUN,
		TOLERANCE)) {
			std::cout << "Error transToLs2: " << pukID << std::endl;
			std::cout << "minimalProgress: " << calData->reach_lb_Switch
					<< " maximalProgress: " << FULLRUN << " Progress: "
					<< progress << std::endl;
			pukState = error;
		}
		break;

	case ls2:
		if (!checkProcessIsInTime(FULLRUN, FULLRUN + 2 * calData->halfPukLength,
		TOLERANCE)) {
			pukState = error;
			std::cout << "minimalProgress: " << FULLRUN << " maximalProgress: "
					<< FULLRUN + 2 * calData->halfPukLength << " Progress: "
					<< progress << std::endl;
		}
		break;

	case PukState::rampFull:
		break;

	case end:
		break;

	case transToFesto2:
		break;

	case error:
		break;
	}
	if (pukState == error) {
		return true;
	}
	return false;
}

} /* namespace automaton */
