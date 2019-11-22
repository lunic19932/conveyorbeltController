/*
 * StateMachine.cpp
 *
 *  Created on: 25.04.2018
 *      Author: Johannes Kruber
 */

#include <iostream>
#include "StateMachine.h"

namespace automaton
{
StateMachine::StateMachine(struct CalibrationData* calData)
		: actualState(init), rampFull(false), beltFree(true), metallexp(true), eStop(false), hightvalue(-1), pulseType(-1), calData(
				calData), adcEnable(false), machineComplete(false)
{
	otherStateMachine.beltFree=false;
	otherStateMachine.rampFull=false;
	otherStateMachine.metalExpected=true;

	for (int i = 0; i < ACTOR_ARRAY_SIZE; i++)
	{
		actorState[i] = false;
	}

	for (int i = 0; i < BLINK_ARRAY_SIZE; i++)
	{
		blink[i] = false;
	}
}

StateMachine::~StateMachine()
{
}

void StateMachine::updateSensorData(bool data[12], int higth, int pulseType)
{
	sensorData.sn_lb_start = data[MSG_SN_LB_START];
	sensorData.sn_lb_hightSn = data[MSG_SN_LB_HIGHTSN];
	sensorData.sn_hightControle = data[MSG_SN_HIGHTCONTROLE];
	sensorData.sn_lb_switch = data[MSG_SN_LB_SWITCH];
	sensorData.sn_metall = data[MSG_SN_METALL];
	sensorData.sn_switchOpen = data[MSG_SN_SWITCHOPEN];
	sensorData.sn_lb_ramp = data[MSG_SN_LB_RAMP];
	sensorData.sn_lb_end = data[MSG_SN_LB_END];

	sensorData.btn_start = data[MSG_BTN_START];
	sensorData.btn_stop = data[MSG_BTN_STOP];
	sensorData.btn_reset = data[MSG_BTN_RESET];
	sensorData.btn_emergency = data[MSG_BTN_EMERGENCY];

	hightvalue = higth;
	this->pulseType = pulseType;
}

void StateMachine::automatonStep(void)
{
	if (sensorData.btn_reset && eStop)
	{
		actualState = States::quiterror;
	}
	else if (sensorData.btn_reset)
	{
		actualState = States::resetMachine;
	}
	else if(!sensorData.btn_stop)
	{
		actualState = States::init;
	}
	switch (actualState)
	{
	case States::init:
		if (sensorData.btn_start)
		{
			actualState = startMachine;
		}
		actorState[MSG_MOTOR_STOP] = true;
		actorState[MSG_ACTLAMP_GREEN] = false;
		actorState[MSG_ACTLAMP_RED] = false;
		actorState[MSG_ACTLAMP_YELLOW] = false;
		if(getType() == STATEMACHINECALIBRATION)
		{
			actorState[MSG_ACTLAMP_GREEN] = true;
			blink[BLINK_GREEN] = true;
		}
		else
		{
			blink[BLINK_GREEN] = false;

		}
		blink[BLINK_YELLOW] = false;
		blink[BLINK_RED] = false;
		actorState[MSG_LED_START] = true;
		actorState[MSG_LED_Q1] = false;
		actorState[MSG_LED_Q2] = false;
		actorState[MSG_LED_RESET] = false;
		break;
	case States::startMachine:
		actorState[MSG_MOTOR_STOP]= false;
		updateStates();
		runStates();
		actorState[MSG_ACTLAMP_GREEN] = true;
		actorState[MSG_ACTLAMP_RED] = false;
		actorState[MSG_ACTLAMP_YELLOW] = false;
		if(getType() == STATEMACHINECALIBRATION)
		{
			blink[BLINK_GREEN] = true;
		}
		else
		{
			blink[BLINK_GREEN] = false;

		}
		blink[BLINK_YELLOW] = false;
		if(rampFull)
		{
			actorState[MSG_ACTLAMP_YELLOW] = true;
			blink[BLINK_YELLOW] = true;
		}
		blink[BLINK_RED] = false;
		actorState[MSG_LED_START] = false;
		actorState[MSG_LED_Q1] = false;
		actorState[MSG_LED_Q2] = false;
		actorState[MSG_LED_RESET] = false;
		break;
	case States::resetMachine:
//		std::cout << "reset" << std::endl;
		reset();
		break;
	case States::errorMachine:
		actorState[MSG_ACTLAMP_RED] = true;
		actorState[MSG_ACTLAMP_GREEN] = false;
		actorState[MSG_ACTLAMP_YELLOW] = false;
		//actorState[MSG_MOTOR_STOP] = true;
		blink[BLINK_GREEN] = false;
		blink[BLINK_YELLOW] = false;
		blink[BLINK_RED] = true;
		actorState[MSG_LED_Q1] = false;
		actorState[MSG_LED_Q2] = false;
		actorState[MSG_LED_RESET] = true;
		actorState[MSG_LED_START] = false;
		break;
	case States::quiterror:
		eStop = false;
		actorState[MSG_ACTLAMP_RED] = true;
		actorState[MSG_ACTLAMP_GREEN] = false;
		actorState[MSG_ACTLAMP_YELLOW] = true;
		//actorState[MSG_MOTOR_STOP] = true;
		blink[BLINK_GREEN] = false;
		blink[BLINK_YELLOW] = false;
		blink[BLINK_RED] = true;
		actorState[MSG_LED_Q1] = false;
		actorState[MSG_LED_Q2] = false;
		actorState[MSG_LED_RESET] = true;
		actorState[MSG_LED_START] = false;
	}
}

void StateMachine::setOtherBeltFree(bool beltFree){
	otherStateMachine.beltFree=beltFree;
}
void StateMachine::setOtherMetalExpected(bool metalExpected){
	otherStateMachine.metalExpected=metalExpected;
}
void StateMachine::setOtherRampFull(bool rampFull){
	otherStateMachine.rampFull=rampFull;
}
void StateMachine::setOtherEmergency(bool emergency){
	otherStateMachine.emergency=emergency;
}
States StateMachine::getActualState()
{
	return actualState;
}

void StateMachine::reset()
{
	for (int i = 0; i < ACTOR_ARRAY_SIZE; i++)
	{
		actorState[i] = false;
	}

	for (int i = 0; i < BLINK_ARRAY_SIZE; i++)
	{
		blink[i] = false;
	}

	actorState[MSG_MOTOR_STOP] = true;
	actualState = init;
	rampFull = false;
	beltFree = true;
	metallexp = true;
	eStop = false;
	hightvalue = 0;
	//calData->reset();
	adcEnable = false;
	machineComplete = false;
	pulseType = -1;
	hightMeasureStatmachine.reset();
}

bool StateMachine::checkError()
{
	if(sensorData.sn_lb_ramp && rampFull)
	{
		rampFull = false;
	}
	if(otherStateMachine.rampFull && rampFull)
	{
		return true;
	}
	else if(rampFull)
	{
		actorState[MSG_ACTLAMP_YELLOW] = true;
		blink[BLINK_YELLOW] = true;
	}
	if(!sensorData.btn_emergency || eStop)
	{
		eStop = true;
		return true;
	}
	else if(otherStateMachine.emergency)
	{
		//eStop = true;
		return true;
	}
	else if(actualState == States::init)
	{
		automatonStep();
	}
	return false;
}

void StateMachine::errorState(bool error)
{
	if(error)
	{
		actualState = States::errorMachine;
	}
	else if(actualState == States::errorMachine)
	{
		actualState = States::init;
	}
}

bool StateMachine::getRampFull()
{
	return rampFull;
}

bool StateMachine::getBeltFree()
{
	return beltFree;
}

bool StateMachine::getMetallExp()
{
	return metallexp;
}

bool StateMachine::getEstop()
{
	return eStop;
}

void StateMachine::setPuk(Puk puk){
	this->puk=puk;
}
Puk StateMachine::getPuk(void){
	return puk;
}


} /* namespace automaton */
