/*
 * Statemachine3.h
 *
 *  Created on: 14.06.2018
 *      Author: Johannes Kruber
 */

#ifndef AUTOMATON_STATEMACHINE3_H_
#define AUTOMATON_STATEMACHINE3_H_

#include <queue>
#include "StateMachine.h"

namespace automaton
{

enum States3
{
	//StateMachine3 States
	start_Machine3,
	ls1_Machine3,
	hs_Machine3,
	ms_Machine3,
	ramp_Machine3,
	rampFull_Machine3,
	transToLs2_Machine3,
	ls2_Machine3
};

struct Puk
{
	States3 actualState;
	bool switchControle = false;
};

class StateMachine3: public StateMachine
{
private:
	std::queue<Puk> pukQueue1;
	std::queue<Puk*> pukQueue2;
	std::queue<Puk*> pukQueue3;
	std::queue<Puk*> pukQueue4;

	double progress;
	bool out, tikerror;
	bool hole;


	void updatePukstate(Puk *Puk);
	void runPukstate(Puk *Puk);
protected:
	virtual void updateStates(void);
	virtual void runStates(void);

public:
	StateMachine3(struct CalibrationData* calData);
	virtual ~StateMachine3();
	virtual bool checkError();
	virtual void makeTikStep(void);
	virtual char getType(void);
	virtual void reset(void);
};

} /* namespace automaton */

#endif /* AUTOMATON_STATEMACHINE3_H_ */
