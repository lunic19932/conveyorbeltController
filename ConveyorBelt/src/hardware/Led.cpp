/*
 * Led.cpp
 *
 *  Created on: 12.04.2018
 *      Author: Johannes Kruber
 */

#include "Led.h"

using namespace hardware;

Led::Led (Hal *hal, char pin)
: hal(hal), pin(pin)
{

}

Led::~Led ()
{
  // TODO Auto-generated destructor stub
}

/**
 * Activate LED on Pin
 */
void Led::ledOn()
{
  hal->writeBit(2, pin);
}

/**
 * Deactivate LED on Pin
 */
void Led::ledOff()
{
  hal->clearBit(2, pin);
}
