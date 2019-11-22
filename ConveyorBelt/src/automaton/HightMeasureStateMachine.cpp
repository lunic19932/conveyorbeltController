/*
 * HightMeasureStateMachine.cpp
 *
 *  Created on: 07.06.2018
 *      Author: Johannes Kruber
 */

#include "HightMeasureStateMachine.h"

#include <iostream>

#define BIN0 22000
#define BIN1 19000
#define MAX_HIGHT 25000

#define MAX_VALUE_NUMBER 8
#define NORM_ROUND_TOLERANCE 500

#include <iostream>

namespace automaton
{

HightMeasureStateMachine::HightMeasureStateMachine()
		: actualState(entry), hightValue(-1), result(-1), hightvalueMid(0)
{

}

HightMeasureStateMachine::~HightMeasureStateMachine()
{
	// TODO Auto-generated destructor stub
}

void HightMeasureStateMachine::updateStates(int hightValue)
{
//	std::cout << "Hightstatemachine hightvalue: " << hightValue << std::endl;

	this->hightValue = hightValue;
	switch (actualState)
	{
	case entry:
//		std::cout << "entry" << std::endl;
		if (hightValue < MAX_HIGHT - PUK_HIGHT_TOLERANCE
				&& hightValue >= BIN1 - PUK_HIGHT_TOLERANCE)
		{
			actualState = value1;
		} else
		{
			actualState = noCode;
		}
		break;

	case value1:
//		std::cout << "value1" << std::endl;
		if (hightValue >= MAX_HIGHT - PUK_HIGHT_TOLERANCE)
		{
			actualState = trans1;
		}
		break;

	case trans1:
//		std::cout << "trans1" << std::endl;
		if (hightValue < MAX_HIGHT - PUK_HIGHT_TOLERANCE
				&& hightValue >= BIN1 - PUK_HIGHT_TOLERANCE)
		{
			actualState = value2;
		}
		break;

	case value2:
//		std::cout << "value2" << std::endl;
		if (hightValue >= MAX_HIGHT - PUK_HIGHT_TOLERANCE)
		{
			actualState = trans2;
		}
		break;

	case trans2:
//		std::cout << "trans2" << std::endl;
		if (hightValue < MAX_HIGHT - PUK_HIGHT_TOLERANCE
				&& hightValue >= BIN1 - PUK_HIGHT_TOLERANCE)
		{
			actualState = value3;
		}
		break;

	case value3:
//		std::cout << "value3" << std::endl;
		if (hightValue >= MAX_HIGHT - PUK_HIGHT_TOLERANCE)
		{
			actualState = calc;
		}
		break;

	case calc:
//		std::cout << "calc" << std::endl;
		actualState = ende;
		break;

	case noCode:
//		std::cout << "noCode" << std::endl;
		actualState = ende;
		break;

	case ende:
//		std::cout << "ende" << std::endl;
		break;

	default:
		break;
	}
}

void HightMeasureStateMachine::runStates()
{
	//bool temp = 0;
//	if (hightValue == BIN0)
//	{
//		temp = 0;
//	} else if (hightValue == BIN1)
//	{
//		temp = 1;
//	}
//	std::cout << "State: " << actualState << std::endl
//			<< "temp: "<< temp << std::endl;

	switch (actualState)
	{
	case entry:
		break;

	case value1:
		if (valueQueue.size() < MAX_VALUE_NUMBER)
		{
			valueQueue.push(hightValue);
		}
		//values[2] = temp;
		break;

	case trans1:
		break;

	case value2:
		if (valueQueue.size() < MAX_VALUE_NUMBER * 2)
		{
			valueQueue.push(hightValue);
		}
		//values[1] = temp;
		break;

	case trans2:
		break;

	case value3:
		if (valueQueue.size() < MAX_VALUE_NUMBER * 3)
		{
			valueQueue.push(hightValue);
		}
		//values[0] = temp;
		break;

	case calc:
		//Calc value 1
		hightvalueMid = 0;
		while (valueQueue.size() > MAX_VALUE_NUMBER * 2)
		{
			hightvalueMid += valueQueue.front();
//			std::cout << "first value front " << valueQueue.front() << std::endl;
			valueQueue.pop();
		}

		hightvalueMid /= MAX_VALUE_NUMBER;
		roundHightvalueMid();
		hightValuesOutput[0] = hightvalueMid; //TODO Leo
//		std::cout << "Hightvalue Mid: " << hightvalueMid << std::endl;
		getBinValueOfHight(2);

		//calc Value 2
		hightvalueMid = 0;
		while (valueQueue.size() > MAX_VALUE_NUMBER)
		{
			hightvalueMid += valueQueue.front();
			valueQueue.pop();
		}

		hightvalueMid /= MAX_VALUE_NUMBER;
		roundHightvalueMid();
		hightValuesOutput[1] = hightvalueMid; // TODO Leo
//		std::cout << "Hightvalue Mid: " << hightvalueMid << std::endl;
		getBinValueOfHight(1);

		//Calc value 3
		hightvalueMid = 0;
		while (!valueQueue.empty())
		{
			hightvalueMid += valueQueue.front();
			valueQueue.pop();
		}

		hightvalueMid /= MAX_VALUE_NUMBER;
		roundHightvalueMid();
		hightValuesOutput[2] = hightvalueMid; //TODO Leo
//		std::cout << "Hightvalue Mid: " << hightvalueMid << std::endl;
		getBinValueOfHight(0);

		//calc result
		result = 0;
		for (bool i : values)
		{
			if (i == 0)
			{
				result <<= 1;
			} else
			{
				result <<= 1;
				result++;
			}
		}
		break;

	case noCode:
		result = -1;
		break;

	case ende:
		break;

	default:
		break;
	}

}

void HightMeasureStateMachine::roundHightvalueMid()
{
	int temp = hightvalueMid / 1000;
	temp *= 1000;

	if ((hightvalueMid - temp) >= NORM_ROUND_TOLERANCE)
	{
		hightvalueMid = temp + 1000;
	} else
	{
		hightvalueMid = temp;
	}
}

void HightMeasureStateMachine::getBinValueOfHight(int valueInt)
{
	bool temp = 0;
	if (hightvalueMid <= BIN0 + PUK_HIGHT_TOLERANCE
			&& hightvalueMid >= BIN0 - PUK_HIGHT_TOLERANCE)
	{
		temp = 0;
	} else if (hightvalueMid <= BIN1 + PUK_HIGHT_TOLERANCE
			&& hightvalueMid >= BIN1 - PUK_HIGHT_TOLERANCE)
	{
		temp = 1;
	}
	values[valueInt] = temp;
}

int HightMeasureStateMachine::getResult()
{
	return result;
}

void HightMeasureStateMachine::reset()
{
	actualState = HightStates::entry;
	result = -1;
}

//TODO Leo
int HightMeasureStateMachine::getHightValuesOutput(int index)
{
	return hightValuesOutput[index];
}
} /* namespace automaton */
