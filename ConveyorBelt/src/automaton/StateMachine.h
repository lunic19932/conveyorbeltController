/*
 * StateMachine.h
 *
 *  Created on: 25.04.2018
 *      Author: Johannes Kruber
 */

#ifndef AUTOMATON_STATEMACHINE_H_
#define AUTOMATON_STATEMACHINE_H_

#include "../communication/Message.h"
#include "HightMeasureStateMachine.h"
#include "../Puk.h"
#define STATEMACHINE1 0
#define STATEMACHINE2 1
#define STATEMACHINECALIBRATION 2

#define DEFAULT_FAST_STEP 	0.17331
#define DEFAULT_SLOW_STEP 	0.0719942
#define DEFAULT_HALF_PUK_LENGTH	8.31889
#define DEFAULT_WORST_HIGHT	50.6066
#define DEFAULT_BEST_HIGHT	40.5546
#define DEFAULT_REACH_SWITCH	61.5251
#define DEFAULT_REACH_RAMP	88.9081
#define DEFAULT_LEAVE_RAMP	93.0676
#define DEFAULT_NORM_QUOTIENT	0.0525
#define DEFAULT_HIGHT_NOPUKRAW	3639

#define TIK_TOLERANCE 5

#define PUK_HIGHT_MICM		28000

namespace automaton
{
struct OtherStateMachine
{
	bool beltFree;
	bool rampFull;
	bool metalExpected;
	bool emergency;

};
struct CalibrationData
{
public:
	double fastStep;
	double slowStep;
	double halfPukLength;
	double reach_lb_HightBest;
	double reach_lb_HightWorst;
	double reach_lb_Switch;
	double reach_lb_Ramp;
	double leave_lb_Ramp;
	double hight_normQuotient;
	int hight_noPukRaw;

	void reset(void)
	{
		fastStep = DEFAULT_FAST_STEP;
		slowStep = DEFAULT_SLOW_STEP;
		halfPukLength = DEFAULT_HALF_PUK_LENGTH;
		reach_lb_HightBest = DEFAULT_BEST_HIGHT;
		reach_lb_HightWorst = DEFAULT_WORST_HIGHT;
		reach_lb_Switch = DEFAULT_REACH_SWITCH;
		reach_lb_Ramp = DEFAULT_REACH_RAMP;
		leave_lb_Ramp = DEFAULT_LEAVE_RAMP;
		hight_normQuotient = DEFAULT_NORM_QUOTIENT;
		hight_noPukRaw = DEFAULT_HIGHT_NOPUKRAW;
	}
};
struct SensorData
{

	bool sn_lb_start;		//Eingangslichtschranke					0
	bool sn_lb_hightSn;	//Lichtschranke am Hoehensensor			1
	bool sn_hightControle;	//Indikator ob hoehe ok ist			2
	bool sn_lb_switch;	//Lichtschranke  vor Weiche				3
	bool sn_metall;		// Metallsensor							4
	bool sn_switchOpen;	//Indikator ob schranke offen ist		5
	bool sn_lb_ramp;		//Lichtschranke an Rampe (rampe voll)	6
	bool sn_lb_end;		//Lichtschranke ende Laufband			7

	bool btn_start;		//Startbutton							8
	bool btn_stop;		//Stopbutton							9
	bool btn_reset;		//resetButton							10
	bool btn_emergency;	//E_stopButton							11
};


enum States
{
	//StateMachine Interface States
	init,
	startMachine,
	resetMachine,
	errorMachine,
	quiterror
};

class StateMachine
{
private:
	States actualState;

protected:
	OtherStateMachine otherStateMachine;
	SensorData sensorData;
	Puk puk;
	bool rampFull;
	bool beltFree;
	bool metallexp;
	bool eStop;
	int hightvalue;
	int pulseType;
	struct CalibrationData* calData;
	virtual void updateStates(void) = 0;
	virtual void runStates(void) = 0;
	HightMeasureStateMachine hightMeasureStatmachine;


public:
	StateMachine(struct CalibrationData* calData);
	virtual
	~StateMachine();
	void setOtherBeltFree(bool beltFree);
	void setOtherMetalExpected(bool metalExpected);
	void setOtherRampFull(bool rampFull);
	void setOtherEmergency(bool emergency);

	bool getRampFull(void);
	bool getBeltFree(void);
	bool getMetallExp(void);
	bool getEstop(void);

	bool actorState[ACTOR_ARRAY_SIZE];
	bool blink[BLINK_ARRAY_SIZE];
	bool adcEnable;
	bool machineComplete;
	void automatonStep(void);
	void updateSensorData(bool data[12], int higth, int pulseType);
	States getActualState();
	void setPuk(Puk puk);
	Puk getPuk(void);
	virtual bool checkError()=0;
	virtual void makeTikStep(void)=0;
	virtual char getType(void)=0;
	virtual void reset(void) =0;
	void errorState(bool error);

};

} /* namespace automaton */

#endif /* AUTOMATON_STATEMACHINE_H_ */
