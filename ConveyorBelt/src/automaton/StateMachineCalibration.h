/*
 * StateMachineCalibration.h
 *
 *  Created on: 09.06.2018
 *      Author: LuisNickel
 */

#ifndef AUTOMATON_STATEMACHINECALIBRATION_H_
#define AUTOMATON_STATEMACHINECALIBRATION_H_

#include "StateMachine.h"
#include <string>
#include <queue>

namespace automaton {
enum CalStates {
	initCal,
	startWorstcase,
	fastWorstCase,
	backToStart1,
	fastBestCase,
	toHight,
	toSwitch,
	toEnd,
	backToStart2,
	slowRun,
	backToStart3,
	toRamp,
	waitForLeaveRamp,
	endCal
};

class StateMachineCalibration: public StateMachine {
private:
	int tikCounter;
	int fastTiksToLeaveStart;
	int fastTiksToHightBest;
	int fastTiksToHightWorst;
	int fastTiksToReachSwitch;
	int fastTiksToReachEnd;
	int slowTiksToReachEnd;
	int tiksToReachRamp;
	int tiksToPassRamp;
	CalStates actualCalState;
	CalStates lastCalState;
	std::string stateString;
	std::queue<int> hightRawValues;

	void calculateCalibration();
	void addHigth(void);
protected:
	virtual void updateStates(void);
	virtual void runStates(void);
public:
	StateMachineCalibration(struct CalibrationData* calData);
	virtual ~StateMachineCalibration();
	virtual bool checkError(void);
	virtual void makeTikStep(void);
	virtual char getType(void);
	virtual void reset(void);
};

} /* namespace automaton */

#endif /* AUTOMATON_STATEMACHINECALIBRATION_H_ */
