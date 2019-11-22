/*
 * HightMeasureStateMachine.h
 *
 *  Created on: 07.06.2018
 *      Author: Johannes Kruber
 */

#ifndef AUTOMATON_HIGHTMEASURESTATEMACHINE_H_
#define AUTOMATON_HIGHTMEASURESTATEMACHINE_H_

#include <queue>

namespace automaton {

#define PUK_HIGHT_TOLERANCE 1000

enum HightStates {
	entry,
	value1,
	trans1,
	value2,
	trans2,
	value3,
	calc,
	noCode,
	ende
};

class HightMeasureStateMachine {
private:
	HightStates actualState;
	bool values[3];
	std::queue<int> valueQueue;
	int hightValue;
	int result;
	int hightvalueMid;
	int hightValuesOutput[3];

	void roundHightvalueMid(void);
	void getBinValueOfHight(int valueInt);
public:
	HightMeasureStateMachine(void);
	virtual ~HightMeasureStateMachine(void);
	void updateStates(int hightValue);
	void runStates(void);
	int getResult(void);
	void reset(void);
	int getHightValuesOutput(int index); //TODO Leo
};

} /* namespace automaton */

#endif /* AUTOMATON_HIGHTMEASURESTATEMACHINE_H_ */
