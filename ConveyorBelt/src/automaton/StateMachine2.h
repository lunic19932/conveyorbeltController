/*
 * StateMachine2.h
 *
 *  Created on: 13.05.2018
 *      Author: Johannes Kruber
 */

#ifndef AUTOMATON_STATEMACHINE2_H_
#define AUTOMATON_STATEMACHINE2_H_

#include "StateMachine.h"

namespace automaton
{

enum States2
{
	//StateMachine2 States
	//States 1
	start1,
	ls1_1,
	hs1,
	ms1,
	ramp1,
	rampFull1,
	transToLs2_1,
	ls2_1,

	//states 2
	start2,
	ls1_2,
	hs2,
	ms2,
	ramp2,
	rampFull2,
	transToLs2_2,
	ls2_2,

	//States 3
	start3,
	ls1_3,
	hs3,
	ms3,
	ramp3,
	rampFull3,
	transToLs2_3,
	ls2_3,
};

class StateMachine2: public StateMachine
{
private:
	double progress;
	States2 actualState;
	bool out, tikerror;
	bool hole;
	int tikRampFull;
	//TODO Leo
	//Puk puk; // Warum????? gibt es doch schon in Statemachine.h----------------------------------------------------

//	double speed;
protected:
	virtual void updateStates(void);
	virtual void runStates(void);

public:
	StateMachine2(struct CalibrationData* calData);
	virtual ~StateMachine2();
	virtual bool checkError();
	virtual void makeTikStep(void);
	virtual char getType(void);
	virtual void reset(void);
};

} /* namespace automaton */

#endif /* AUTOMATON_STATEMACHINE2_H_ */
