/*
 * PukStateMachine.h
 *
 *  Created on: 04.06.2018
 *      Author: acd219
 */

#ifndef AUTOMATON_PUKSTATEMACHINE_H_
#define AUTOMATON_PUKSTATEMACHINE_H_


#include "StateMachine.h"
#include "../communication/Message.h"
#include <iostream>
#include "../Puk.h"
#define NUMBER_OF_CASES 17

namespace automaton {

enum PukState {
	//PukStateMachine States
	startPuk,
	ls1,
	transToHs,
	hs,
	toRamp1,
	ramp,
	ms,
	transToMs,
	transToLs2,
	ls2,
	rampFull,
	transToFesto2,
	error,
	end
};

class PukStateMachine {
private:
	Puk puk;
	PukState pukState;
	struct OtherStateMachine* festo2;
	struct CalibrationData* calData;
	int pukID;
	int pukCode;
	int tikCounter;
	double progress;
	double progressLs2;
	bool sortOut;
	bool hole;
	bool pukActor[ACTOR_ARRAY_SIZE];
	bool* rampFull;


	bool checkProcessIsInTime(double minimalProgress,double maximalProgress,double tolerance);
public:
	bool operator == (const PukStateMachine& s) const { return pukID == s.pukID; }
	bool operator != (const PukStateMachine& s) const { return !operator==(s); }
	PukStateMachine(int pukID,CalibrationData* data, OtherStateMachine* festo2,bool* rampFull,int systemTime);
	PukStateMachine(void);
	~PukStateMachine(void);
	int getPukID(void);
	bool checkError(void);
	bool getPukActor(int index);
	void updateProcess(double speed);
	void runStates();
	void updateStates(SensorData sensorData);
	PukState getState(void);
	void setPukCode(int pukCode, int hightValue1,int hightValue2,int hightValue3);
	double getProgress();
	Puk getPuk(void);
;


};

} /* namespace automaton */

#endif /* AUTOMATON_PUKSTATEMACHINE_H_ */

