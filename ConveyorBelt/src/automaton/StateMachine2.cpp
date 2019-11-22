/*
 * StateMachine2.cpp
 *
 *  Created on: 13.05.2018
 *      Author: Johannes Kruber
 */

#include "StateMachine2.h"
#include <iostream>

namespace automaton
{

#define RAMP_FULL_TIKS 100 //1s

StateMachine2::StateMachine2(struct CalibrationData* calData)
		: StateMachine(calData), progress(0), actualState(start1), out(true), tikerror(
				false), hole(false), tikRampFull(0)
{
	//std::cout << "Statemachine 2 created" << std::endl;
}

StateMachine2::~StateMachine2()
{

}

void StateMachine2::updateStates(void)
{
	if(sensorData.sn_lb_ramp)
	{
		rampFull = false;
		//std::cout <<"rampfull: " << rampFull<< std::endl;
	}
	//std::cout <<"State: " << actualState << std::endl;
	switch (actualState)
	{
//	case init:
//		if (sensorData.btn_start)
//		{
//			actualState = start1;
//		}
//		break;
	case start1:
		if (!sensorData.sn_lb_start)
		{
			actualState = ls1_1;
			out = false;
			progress = 0;
			hole = false;
		}
		break;
	case ls1_1:
		if (!sensorData.sn_lb_hightSn)
		{
			actualState = hs1;
		}
		break;
	case hs1:	//hightMeasureResult
		//std::cout << sensorData.sn_hightControle << std::endl
		//		<< sensorData.sn_lb_hightSn << std::endl;

		if (sensorData.sn_hightControle && !sensorData.sn_lb_hightSn && hole)
		{
			actualState = ms1;
		} else if (!sensorData.sn_hightControle && sensorData.sn_lb_hightSn)
		{
			actualState = ramp1;
		} else if (sensorData.sn_lb_hightSn)
		{
			actualState = ramp1;
		}

		break;
	case ms1:
		//std::cout << sensorData.sn_metall << std::endl;
		if (sensorData.sn_metall && !sensorData.sn_lb_switch)
		{
			actualState = transToLs2_1;
			out = false;
		} else if (!sensorData.sn_metall && !sensorData.sn_lb_switch)
		{
			actualState = ramp1;
		}
		break;
	case ramp1:
		if (!sensorData.sn_lb_ramp)
		{
			actualState = rampFull1;
		}
		break;
	case rampFull1:

		if (sensorData.sn_lb_ramp)
		{
			actualState = start1;
		}
		break;
	case transToLs2_1:
		if (!sensorData.sn_lb_end)
		{
			actualState = ls2_1;
		}
		break;
	case ls2_1:
		if (sensorData.sn_lb_end)
		{
			actualState = start2;
			progress = 0;
		}
		break;
	case start2:
		if (!sensorData.sn_lb_start)
		{
			actualState = ls1_2;
			out = false;
			progress = 0;
			hole = false;
		}
		break;
	case ls1_2:
		if (!sensorData.sn_lb_hightSn)
		{
			actualState = hs2;
			progress = calData->reach_lb_HightBest;
		}
		break;
	case hs2:
//		std::cout << sensorData.sn_hightControle << std::endl
//				<< sensorData.sn_lb_hightSn << std::endl;

		if (sensorData.sn_hightControle && !sensorData.sn_lb_hightSn && hole)
		{
			actualState = ms2;
		} else if (!sensorData.sn_hightControle && sensorData.sn_lb_hightSn)
		{
			actualState = ramp2;
		} else if (sensorData.sn_lb_hightSn)
		{
			actualState = ramp1;
		}

		break;
	case ms2:
		//std::cout << sensorData.sn_metall << std::endl;
		if ((sensorData.sn_metall || puk.codeSecond!=-1) && !sensorData.sn_lb_switch )
		{
			actualState = ramp2;
		} else if (!sensorData.sn_metall && !sensorData.sn_lb_switch)
		{
			actualState = transToLs2_2;
			out = false;
		}
		break;
	case ramp2:
		if (!sensorData.sn_lb_ramp)
		{
			actualState = rampFull2;
		}
		break;
	case rampFull2:

		if (sensorData.sn_lb_ramp)
		{
			actualState = start2;
		}
		break;
	case transToLs2_2:
		if (!sensorData.sn_lb_end)
		{
			actualState = ls2_2;
		}
		break;
	case ls2_2:
		if (sensorData.sn_lb_end)
		{
			actualState = start3;
			progress = 0;
		}
		break;
	case start3:
		if (!sensorData.sn_lb_start)
		{
			actualState = ls1_3;
			out = false;
			progress = 0;
			hole = false;
		}
		break;
	case ls1_3:
		if (!sensorData.sn_lb_hightSn)
		{
			actualState = hs3;
			progress = calData->reach_lb_HightBest;
		}
		break;
	case hs3:	//hightMeasureResult
		//std::cout << sensorData.sn_hightControle << std::endl
		//		<< sensorData.sn_lb_hightSn << std::endl;

		if (sensorData.sn_hightControle && !sensorData.sn_lb_hightSn && hole)
		{
			actualState = ms3;
		} else if (!sensorData.sn_hightControle && sensorData.sn_lb_hightSn)
		{
			actualState = ramp3;
		} else if (sensorData.sn_lb_hightSn)
		{
			actualState = ramp1;
		}

		break;
	case ms3:
		//std::cout << sensorData.sn_metall << std::endl;
		if ((sensorData.sn_metall || puk.codeSecond!=-1) && !sensorData.sn_lb_switch)
		{
			actualState = ramp3;
		} else if (!sensorData.sn_metall && !sensorData.sn_lb_switch)
		{
			actualState = transToLs2_3;
			out = false;
		}
		break;
	case ramp3:
		if (!sensorData.sn_lb_ramp)
		{
			actualState = rampFull3;
		}
		break;
	case rampFull3:

		if (sensorData.sn_lb_ramp)
		{
			actualState = start3;
		}
		break;
	case transToLs2_3:
		if (!sensorData.sn_lb_end)
		{
			actualState = ls2_3;
		}
		break;
	case ls2_3:
		if (sensorData.sn_lb_end)
		{
			actualState = start1;
			progress = 0;
		}
		break;
	}
}

void StateMachine2::runStates(void)
{
	if (adcEnable)
	{
		hightMeasureStatmachine.updateStates(hightvalue);
		hightMeasureStatmachine.runStates();
	}
	switch (actualState)
	{
//	case init:
//		actorState[MSG_LED_START] = true;
//		break;
	case start1:
		actorState[MSG_LED_START] = false;
		actorState[MSG_MOTOR_STOP] = true;
		//std::cout<<"Start1"<<std::endl;
		beltFree = true;
		tikRampFull = 0;
		metallexp = true;
		break;
	case start2:
	case start3:
		actorState[MSG_MOTOR_STOP] = true;
		//std::cout<<"Start23"<<std::endl;
		beltFree = true; //TODO Leo - Laufband 2 ist frei
		tikRampFull = 0;
		metallexp = false;
		break;
	case ls1_1:
	case ls1_2:
	case ls1_3:
		//Starte Motor
		actorState[MSG_MOTOR_RIGHT] = true;
		actorState[MSG_MOTOR_LEFT] = false;
		actorState[MSG_MOTOR_STOP] = false;
		actorState[MSG_MOTOR_SLOW] = false;
		if (sensorData.sn_lb_start && !out)
		{
			out = true;
		}
		//std::cout<<"LS1"<<std::endl;
		beltFree = false; //TODO Leo - Laufband 2 ist besetzt
		break;
	case hs1:
	case hs2:
	case hs3:
		//actorState[MSG_MOTOR_SLOW] = true;
		//std::cout<<"HS1"<<std::endl;
		if (!sensorData.sn_hightControle)
		{
			hole = true;
		}
		adcEnable = true;
		break;
	case ms1:
	case ms2:
	case ms3:
		if (sensorData.sn_lb_hightSn && adcEnable)
		{
			adcEnable = false;
			puk.codeSecond = hightMeasureStatmachine.getResult();

			for (int i = 0; i < 3; i++) {
			puk.hightSecond[i] = hightMeasureStatmachine.getHightValuesOutput(i);
			}

			hightMeasureStatmachine.reset();
			std::cout << "PukCode: " << puk.codeSecond << std::endl;

			if (puk.codeSecond != -1)
			{
				std::cout<<"Zeit(s): "<<(puk.tiksFirst+puk.hightTikSecond)/100<<"\nPUK-ID: "<<puk.id<<
				"\nCode: "<<puk.codeSecond<<"\n Höhenmesswert: "<<puk.hightSecond[0]<<" | "<<puk.hightSecond[1]<<" | "<<puk.hightSecond[2]<<std::endl;
			}
//	actorState[MSG_MOTOR_SLOW] = false;
		}
		break;
	case ramp1:
	case ramp2:
	case ramp3:
		if (sensorData.sn_lb_hightSn && adcEnable)
		{
			adcEnable = false;
		}
		//Schliese Switch
		actorState[MSG_SWITCH] = false;

		//TODO Leo - Ausgabe bei Binärcode-erkennung  
		// warum HIER die Ausgabe???------------------------------------------------------------------------

		break;
	case rampFull1:
	case rampFull2:
	case rampFull3:
		tikRampFull = puk.tiksSecond;
//		std::cout <<"tikrampfull: " << tikRampFull<< std::endl;
		break;
	case transToLs2_1:
	case transToLs2_2:
	case transToLs2_3:
		actorState[MSG_MOTOR_SLOW] = false;
		actorState[MSG_SWITCH] = true;
		if (sensorData.sn_lb_switch && !out)
		{
			out = true;
		}
		break;
	case ls2_1:
	case ls2_2:
	case ls2_3:
		actorState[MSG_MOTOR_STOP] = true;
		actorState[MSG_SWITCH] = false;

		//TODO Leo - Ausgabe am Ende des FB2
		//Warum pukHightFirst ausgeben???  wo wird der sauber resetet??? sollte hier eigentlich nicht vorhanden sein!!!---------------------------------------
		std::cout<<"PUK-ID: "<<puk.id<<"\nTyp: "<<puk.codeSecond<<"\nHöhenmesswert Laufband 1: "<<
		puk.hightFirst[0]<<" | "<<puk.hightFirst[1]<<" | "<<puk.hightFirst[2]<<"\nHöhenmesswert Laufband 2: "<<
		puk.hightSecond[0]<<" | "<<puk.hightSecond[1]<<" | "<<puk.hightSecond[2]<<std::endl;

		break;
	}
}

bool StateMachine2::checkError()
{
	if (StateMachine::checkError())
	{
		return true;
	}
	//std::cout <<"actual state " << actualState <<std::endl;
	switch (actualState)
	{
	case start1:
	case start2:
	case start3:
		break;
	case ls1_1:
	case ls1_2:
	case ls1_3:
		if ((!sensorData.sn_lb_start && out) || !sensorData.sn_lb_switch
				|| !sensorData.sn_lb_end)
		{
			std::cout <<"Fehler Sensor ls1"<<std::endl;
			return true;
		} else if (progress - TIK_TOLERANCE > calData->reach_lb_HightWorst)
		{
			std::cout << "Fehler tik hs too slow" << std::endl;
			return true;
		}
		break;
	case hs1:
	case hs2:
	case hs3:
		//std::cout << "Higth Error Check: " << tikerror << " "<< progress << " " << calData->reach_lb_HightBest<< std::endl;
		if (!sensorData.sn_lb_start || !sensorData.sn_lb_switch
				|| !sensorData.sn_lb_end)
		{
			std::cout << "Fehler sensor" << std::endl;
			return true;
		}
//		else if (hightvalue > PUK_HIGHT_MICM + PUK_HIGHT_TOLERANCE)
//		{
//			std::cout << "Fehler Hoehe " << hightvalue << std::endl;
//			return true;
//		}
		else if (progress + TIK_TOLERANCE < calData->reach_lb_HightBest
				|| tikerror)
		{
			std::cout << "Fehler tik hs to fast" << std::endl;
			tikerror = true;
			return true;
		}
		else
		{
			progress = calData->reach_lb_HightBest;
		}

		break;
	case ms1:
	case ms2:
	case ms3:
		//std::cout <<"ms error check "<< tikerror << std::endl;
		if (!sensorData.sn_lb_start || !sensorData.sn_lb_end)
		{
			std::cout << "Fehler sensor" << std::endl;
			return true;
		} else if (hightvalue > PUK_HIGHT_MICM + PUK_HIGHT_TOLERANCE)
		{
			std::cout << " ms Fehler Hoehe " << hightvalue << std::endl;
			return true;
		} else if (progress - TIK_TOLERANCE > calData->reach_lb_Switch)
		{
			std::cout << "Fehler tik ms to slow" << std::endl;
			return true;
		}
		break;
	case transToLs2_1:
	case transToLs2_2:
	case transToLs2_3:
		//std::cout <<"trans error check"<<std::endl;
		if (!sensorData.sn_lb_start || !sensorData.sn_lb_hightSn
				|| (!sensorData.sn_lb_switch && out))
		{
			//std::cout <<"Fehler"<<std::endl;
			return true;
		} else if (progress + TIK_TOLERANCE < calData->reach_lb_Switch
				|| tikerror)
		{
			std::cout << "Fehler tik reached switch too fast" << std::endl;
			tikerror = true;
			return true;
		} else if (progress - TIK_TOLERANCE > 100)
		{
			std::cout << "Fehler tik reached ls2 too slow" << std::endl;
			return true;
		}
		break;
	case ls2_1:
	case ls2_2:
	case ls2_3:
		if (!sensorData.sn_lb_start || !sensorData.sn_lb_switch
				|| !sensorData.sn_lb_hightSn)
		{
			//std::cout <<"Fehler"<<std::endl;
			return true;
		} else if (progress + TIK_TOLERANCE < 100 || tikerror)
		{
			std::cout << "Fehler tik reched ls2 too fast" << std::endl;
			tikerror = true;
			return true;
		}
		break;
	case ramp1:
	case ramp2:
	case ramp3:
		if (!sensorData.sn_lb_start || !sensorData.sn_lb_end)
		{
			//std::cout <<"Fehler"<<std::endl;
			return true;
		}
		if(rampFull)
		{
			return true;
		}
		break;
	case rampFull1:
		if(puk.tiksSecond - tikRampFull >= RAMP_FULL_TIKS)
		{
			rampFull = true;
			actualState = States2::start1;
			actorState[MSG_MOTOR_STOP] = true;
			//std::cout <<"rampfull: " << rampFull<< std::endl;
		}

		break;
	case rampFull2:
		if(puk.tiksSecond - tikRampFull >= RAMP_FULL_TIKS)
		{
			rampFull = true;
			actualState = States2::start1;
			actorState[MSG_MOTOR_STOP] = true;
			//std::cout <<"rampfull: " << rampFull<< std::endl;
		}

		break;
	case rampFull3:
		if(puk.tiksSecond - tikRampFull >= RAMP_FULL_TIKS)
		{
			rampFull = true;
			actualState = States2::start1;
			actorState[MSG_MOTOR_STOP] = true;
			beltFree = true;
			//std::cout <<"rampfull: " << rampFull<< std::endl;
		}

		break;
	}
	return false;
}

void StateMachine2::makeTikStep()
{
//	if (actorState[MSG_MOTOR_STOP]) {
//			speed = 0;
//		} else if (actorState[MSG_MOTOR_SLOW] && actorState[MSG_MOTOR_RIGHT]) {
//			speed = calData->slowStep;
//		} else if (actorState[MSG_MOTOR_RIGHT]) {
//			speed = calData->fastStep;
//		}
//	runStates();

	if(StateMachine::getActualState()==States::startMachine)
	{
		progress += calData->fastStep;
	}
	puk.hightTikSecond++;
	puk.tiksSecond++;
}

char StateMachine2::getType()
{
	return STATEMACHINE2;
}

void StateMachine2::reset()
{
	StateMachine::reset();
	actualState = States2::start1;
}

} /* namespace automaton */
