/*
 * HardwareClient.cpp
 *
 *  Created on: 30.04.2018
 *      Author: Johannes Kruber
 */

#include "HardwareClient.h"
#include "../ClientId.h"
#include <iostream>

namespace communication
{

HardwareClient::HardwareClient(int channelId)
		: Client(channelId), adcEnable(false)
{
	controller.ac_motor->reset();
	controller.led_start->ledOn();
}

HardwareClient::~HardwareClient()
{

}

void HardwareClient::work()
{
	switch (msg.senderId)
	{
	case MAIN_CNTR:
		if (msg.actorStatus[MSG_MOTOR_RIGHT])
		{
			controller.ac_motor->setRight();
		}

		if (msg.actorStatus[MSG_MOTOR_LEFT])
		{
			controller.ac_motor->setLeft();
		}

		if (msg.actorStatus[MSG_MOTOR_SLOW])
		{
			controller.ac_motor->setSlow();
		} else
		{
			controller.ac_motor->resetSlow();
		}

		if (msg.actorStatus[MSG_MOTOR_STOP])
		{
			controller.ac_motor->stop();
		} else
		{
			controller.ac_motor->start();
		}

		if (msg.actorStatus[MSG_SWITCH])
		{
			controller.ac_switch->open();
		} else
		{
			controller.ac_switch->close();
		}

		//LEds
		if (msg.actorStatus[MSG_LED_START])
		{
			controller.led_start->ledOn();
		} else
		{
			controller.led_start->ledOff();
		}

		if (msg.actorStatus[MSG_LED_RESET])
		{
			controller.led_reset->ledOn();
		} else
		{
			controller.led_reset->ledOff();
		}

		if (msg.actorStatus[MSG_LED_Q1])
		{
			controller.led_q1->ledOn();
		} else
		{
			controller.led_q1->ledOff();
		}

		if (msg.actorStatus[MSG_LED_Q2])
		{
			controller.led_q2->ledOn();
		} else
		{
			controller.led_q2->ledOff();
		}
		if(msg.adcEnable)
		{
			if(!adcEnable)
			{
				controller.enableAdc();
				adcEnable = msg.adcEnable;
			}
		}
		else
		{
			if(adcEnable)
			{
				controller.disableAdc();
				adcEnable= msg.adcEnable;
			}
		}

		msg.receiverId = ACLIGHT_CONTR;
		break;
	case ACLIGHT_CONTR:
		if (msg.actorStatus[MSG_ACTLAMP_RED])
		{
			controller.ac_lamp->setRedOn();
		} else
		{
			controller.ac_lamp->setRedOff();
		}

		if (msg.actorStatus[MSG_ACTLAMP_YELLOW])
		{
			controller.ac_lamp->setYellowOn();
		} else
		{
			controller.ac_lamp->setYellowOff();
		}

		if (msg.actorStatus[MSG_ACTLAMP_GREEN])
		{
			controller.ac_lamp->setGreenOn();
		} else
		{
			controller.ac_lamp->setGreenOff();
		}
		msg.receiverId = EMPTY;
		break;
	case PULSE_RECIEVER:
		msg.sensorData[MSG_SN_LB_START] = controller.sn_lb_start->isTrue();
		msg.sensorData[MSG_SN_LB_HIGHTSN] = controller.sn_lb_hightSn->isTrue();
		msg.sensorData[MSG_SN_HIGHTCONTROLE] =
				controller.sn_hightControle->isTrue();
		msg.sensorData[MSG_SN_LB_SWITCH] = controller.sn_lb_switch->isTrue();
		msg.sensorData[MSG_SN_METALL] = controller.sn_metall->isTrue();
		msg.sensorData[MSG_SN_SWITCHOPEN] = controller.sn_switchOpen->isTrue();
		msg.sensorData[MSG_SN_LB_RAMP] = controller.sn_lb_ramp->isTrue();
		msg.sensorData[MSG_SN_LB_END] = controller.sn_lb_end->isTrue();
		msg.sensorData[MSG_BTN_START] = controller.btn_start->isTrue();
		msg.sensorData[MSG_BTN_STOP] = controller.btn_stop->isTrue();
		msg.sensorData[MSG_BTN_RESET] = controller.btn_reset->isTrue();
		msg.sensorData[MSG_BTN_EMERGENCY] = controller.btn_emergency->isTrue();

		msg.hightSensor = controller.getHightValue();

		//std::cout<< msg.hightSensor << std::endl;

		msg.receiverId = MAIN_CNTR;
		break;
	default:
		msg.receiverId = EMPTY;
	}

	msg.senderId = HARDWARE_CONTR;
}

hardware::Controller* HardwareClient::getController()
{
	return &controller;
}

} /* namespace communication */
