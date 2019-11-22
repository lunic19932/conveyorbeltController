/*
 * Controller.cpp
 *
 *  Created on: 10.04.2018
 *      Author: Johannes Kruber
 */

#include "Controller.h"
#include <iostream>

using namespace hardware;

#define ADC_INTERVAL_NANOSEC 1000000

Controller::Controller(Hal *hal)
		: hal(hal), running(true), enabled(false), adcThread(new std::thread(std::ref(*this)))
{
	ac_motor = new Motor(hal);
	ac_lamp = new ActivityLight(hal);
	ac_switch = new Switch(hal);

	sn_lb_start = new SensorBool(hal, SN_LB_ENTRY);
	sn_lb_hightSn = new SensorBool(hal, SN_LB_IN_HIGHT);
	sn_hightControle = new SensorBool(hal, SN_HIGHT_OK);
	sn_lb_switch = new SensorBool(hal, SN_SWITCH_IN);
	sn_metall = new SensorBool(hal, SN_METALL);
	sn_switchOpen = new SensorBool(hal, SN_SWITCH_OPEN);
	sn_lb_ramp = new SensorBool(hal, SN_LB_RAMP);
	sn_lb_end = new SensorBool(hal, SN_LB_OUT);

	btn_start = new SensorBool(hal, BTN_START);
	btn_stop = new SensorBool(hal, BTN_STOP);
	btn_reset = new SensorBool(hal, BTN_RESET);
	btn_emergency = new SensorBool(hal, BTN_E_STOP);

	led_start = new Led(hal, LED_START);
	led_reset = new Led(hal, LED_RESET);
	led_q1 = new Led(hal, LED_Q1);
	led_q2 = new Led(hal, LED_Q2);

	hal->initADC();

	updateSensorValues();

	//adcThread->detach();

}

/**
 * Creates a new Controller to access the HAL
 */
Controller::Controller()
		: Controller(new Hal())
{
}

/**
 * Frees the Controller
 */
Controller::~Controller()
{
	running = false;

//	if(adcThread->joinable())
//	{
//		adcThread->join();
//	}
}

/**
 * Triggers the Controller to Update the Sensor Values
 */
void Controller::updateSensorValues()
{
	hal->readGpio(0);
}

void Controller::updateHightADCSensor()
{
	hal->readAdc();
}

/**
 * Gets the Sensor Gpio Address for the ISR Routine
 */
int Controller::getSensorGpioAddress()
{
	return hal->getGpioAddress(0);
}

void Controller::resetAdcInterrupt()
{
	hal->clearADCInterrupt();
}

void Controller::operator()()
{
	std::unique_lock<std::mutex> lock(mutex);
	while (running)
	{
		if(enabled)
		{
			hal->enableADC(16);
			std::this_thread::sleep_for(std::chrono::nanoseconds(ADC_INTERVAL_NANOSEC));
		}
		else
		{
			waitVar.wait(lock);
		}
		//std::this_thread::sleep_for(std::chrono::nanoseconds(ADC_INTERVAL_NANOSEC));
	}
}

void Controller::enableAdc()
{
	//std::cout << "enable adc"<< std::endl;
	enabled = true;
	waitVar.notify_one();
}

void Controller::disableAdc()
{
	enabled = false;
}

int Controller::getHightValue()
{
	return hal->getHightValue();
}

