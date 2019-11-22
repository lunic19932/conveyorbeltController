///*
// * Statemachine3.cpp
// *
// *  Created on: 14.06.2018
// *      Author: Johannes Kruber
// */
//
//#include "StateMachine3.h"
//#include <iostream>
//
//namespace automaton
//{
//
//StateMachine3::StateMachine3(struct CalibrationData* calData)
//		: StateMachine(calData), progress(0), out(true), tikerror(false), hole(
//				false)
//{
//
//}
//
//void StateMachine3::updatePukstate(Puk *puk)
//{
//	switch (puk->actualState)
//	{
//	case start_Machine3:
//		if (!sensorData.sn_lb_start)
//		{
//			puk->actualState = ls1_Machine3;
//			out = false;
//			progress = 0;
//			hole = false;
//		}
//		break;
//	case ls1_Machine3:
//		std::cout << "update ls1_Machine3" << std::endl;
//		if (!sensorData.sn_lb_hightSn)
//		{
//			puk->actualState = hs_Machine3;
//			progress = calData->reach_lb_HightBest;
//		}
//		break;
//	case hs_Machine3:
//		//hightMeasureResult
//		//std::cout << sensorData.sn_hightControle << std::endl
//		//		<< sensorData.sn_lb_hightSn << std::endl;
//
//		if (sensorData.sn_hightControle && !sensorData.sn_lb_hightSn && hole)
//		{
//			puk->actualState = ms_Machine3;
//		} else if (!sensorData.sn_hightControle && sensorData.sn_lb_hightSn)
//		{
//			puk->actualState = ramp_Machine3;
//		} else if (sensorData.sn_lb_hightSn)
//		{
//			puk->actualState = ramp_Machine3;
//		}
//
//		break;
//	case ms_Machine3:
//		//std::cout << sensorData.sn_metall << std::endl;
//		if (sensorData.sn_metall && !sensorData.sn_lb_switch)
//		{
//			puk->actualState = transToLs2_Machine3;
//			out = false;
//		} else if (!sensorData.sn_metall && !sensorData.sn_lb_switch)
//		{
//			puk->actualState = ramp_Machine3;
//		}
//		break;
//	case ramp_Machine3:
//		if (!sensorData.sn_lb_ramp)
//		{
//			puk->actualState = rampFull_Machine3;
//		}
//		break;
//	case rampFull_Machine3:
//
//		if (sensorData.sn_lb_ramp)
//		{
//			puk->actualState = start_Machine3;
//		}
//		break;
//	case transToLs2_Machine3:
//		if (!sensorData.sn_lb_end)
//		{
//			puk->actualState = ls2_Machine3;
//		}
//		break;
//	case ls2_Machine3:
//		if (sensorData.sn_lb_end)
//		{
//			puk->actualState = start_Machine3;
//			progress = 0;
//		}
//		break;
//	}
//
//}
//void StateMachine3::runPukstate(Puk *puk)
//{
//	switch (puk->actualState)
//	{
//	//	case init:
//	//		actorState[MSG_LED_START] = true;
//	//		break;
//	case start_Machine3:
//		actorState[MSG_LED_START] = false;
//		actorState[MSG_MOTOR_STOP] = true;
//		//std::cout<<"Start1"<<std::endl;
//		break;
//	case ls1_Machine3:
//		std::cout << "LS1" << std::endl;
//		//Starte Motor
////		actorState[MSG_MOTOR_RIGHT] = true;
////		actorState[MSG_MOTOR_LEFT] = false;
////		actorState[MSG_MOTOR_STOP] = false;
////		actorState[MSG_MOTOR_SLOW] = false;
//		if (sensorData.sn_lb_start && !out)
//		{
//			out = true;
//		}
//		break;
//	case hs_Machine3:
//		//actorState[MSG_MOTOR_SLOW] = true;
//		std::cout << "HS" << std::endl;
//		if (!sensorData.sn_hightControle)
//		{
//			hole = true;
//		}
//		adcEnable = true;
//		break;
//	case ms_Machine3:
//		std::cout << "MS" << std::endl;
//		if (sensorData.sn_lb_hightSn && adcEnable)
//		{
//			adcEnable = false;
//			pukCode = hightMeasureStatmachine.getResult();
//			hightMeasureStatmachine.reset();
//			std::cout << "PukCode: " << pukCode << std::endl;
//			//	actorState[MSG_MOTOR_SLOW] = false;
//		}
//		break;
//	case ramp_Machine3:
//		std::cout << "ramp" << std::endl;
//		if (sensorData.sn_lb_switch)
//		{
//			actorState[MSG_SWITCH] = false;
//		}
//		//Schliese Switch
//
//		break;
//	case rampFull_Machine3:
//		std::cout << "rampFull" << std::endl;
//		break;
//	case transToLs2_Machine3:
//		std::cout << "TransTols2" << std::endl;
//		actorState[MSG_MOTOR_SLOW] = false;
//		if (!sensorData.sn_lb_switch)
//		{
//			actorState[MSG_SWITCH] = true;
//		}
//		if (sensorData.sn_lb_switch && !out)
//		{
//			out = true;
//		}
//		break;
//	case ls2_Machine3:
//		std::cout << "ls2" << std::endl;
//		actorState[MSG_MOTOR_STOP] = true;
//		actorState[MSG_SWITCH] = false;
//		break;
//	}
//
//}
//
//StateMachine3::~StateMachine3()
//{
//	// TODO Auto-generated destructor stub
//}
//
//void StateMachine3::updateStates(void)
//{
//	if (!sensorData.sn_lb_start)
//	{
//		States3 newPuk = ls1_Machine3;
//		pukQueue1.push(newPuk);
//	}
//
//	if (!pukQueue1.empty())
//	{
//		std::cout << "front Queue1 " << (pukQueue1.front()) << std::endl;
//		updatePukstate(&pukQueue1.front());
//
//		if (pukQueue1.front() == ms_Machine3)
//		{
//			pukQueue2.push(&pukQueue1.front());
//			pukQueue1.pop();
//		} else if (pukQueue1.front() == ramp_Machine3)
//		{
//			pukQueue4.push(&pukQueue1.front());
//			pukQueue1.pop();
//		}
//	}
//
//	if (!pukQueue2.empty())
//	{
//		updatePukstate(pukQueue2.front());
//
//		if (*pukQueue2.front() == transToLs2_Machine3)
//		{
//			pukQueue3.push(pukQueue2.front());
//			pukQueue2.pop();
//		} else if (*pukQueue2.front() == ramp_Machine3)
//		{
//			pukQueue4.push(pukQueue2.front());
//			pukQueue2.pop();
//		}
//	}
//
//	if (!pukQueue3.empty())
//	{
//		updatePukstate(pukQueue3.front());
//
//		if (*pukQueue3.front() == start_Machine3)
//		{
//			pukQueue3.pop();
//		}
//	}
//
//	if (!pukQueue4.empty())
//	{
//		updatePukstate(pukQueue4.front());
//	}
//}
//
//void StateMachine3::runStates(void)
//{
//	if (!pukQueue1.empty() || !pukQueue2.empty() || !pukQueue3.empty()
//			|| !pukQueue4.empty())
//	{
//		actorState[MSG_MOTOR_RIGHT] = true;
//		actorState[MSG_MOTOR_LEFT] = false;
//		actorState[MSG_MOTOR_STOP] = false;
//		actorState[MSG_MOTOR_SLOW] = false;
//	} else
//	{
//		actorState[MSG_MOTOR_RIGHT] = false;
//		actorState[MSG_MOTOR_LEFT] = false;
//		actorState[MSG_MOTOR_STOP] = true;
//		actorState[MSG_MOTOR_SLOW] = false;
//	}
//	if (!pukQueue1.empty())
//	{
//		runPukstate(&pukQueue1.front());
//	}
//
//	if (!pukQueue2.empty())
//	{
//		runPukstate(pukQueue2.front());
//	}
//
//	if (!pukQueue3.empty())
//	{
//		runPukstate(pukQueue3.front());
//	}
//
//	if (!pukQueue4.empty())
//	{
//		runPukstate(pukQueue4.front());
//	}
//}
//
//bool StateMachine3::checkError()
//{
//	return false;
//}
//void StateMachine3::makeTikStep(void)
//{
//
//}
//
//char StateMachine3::getType(void)
//{
//	return STATEMACHINE1;
//}
//
//void StateMachine3::reset(void)
//{
//
//}
//
//} /* namespace automaton */
