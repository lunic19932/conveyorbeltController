/*
 * SensorBool.cpp
 *
 *  Created on: 10.04.2018
 *      Author: Johannes Kruber
 */

#include "SensorBool.h"

using namespace hardware;

SensorBool::SensorBool (Hal *hal, char pin)
: hal(hal), pin(pin)
{

}

SensorBool::~SensorBool ()
{
  // TODO Auto-generated destructor stub
}

/**
 * Return Sensor Value
 */
bool SensorBool::isTrue(void)
{
  int value = hal->getPinValue(pin);

  return value;
}
