/*
 * ActivityLight.cpp
 *
 *  Created on: 29.03.2018
 *      Author: Johannes Kruber
 */

#include "ActivityLight.h"

using namespace hardware;

#define PORT 1

ActivityLight::ActivityLight (Hal *hal)
: hal(hal)
{

}

ActivityLight::~ActivityLight ()
{
}

/**
 * Sets the Green Light On
 */
void ActivityLight::setGreenOn(void)
{
  hal->writeBit(PORT, ACTLAMP_GREEN);
}

/**
 * Sets the Green Light Off
 */
void ActivityLight::setGreenOff(void)
{
  hal->clearBit(PORT, ACTLAMP_GREEN);
}

/**
 * Sets the Red Light On
 */
void ActivityLight::setRedOn(void)
{
  hal->writeBit(PORT, ACTLAMP_RED);
}

/**
 * Sets the Red Light Off
 */
void ActivityLight::setRedOff(void)
{
  hal->clearBit(PORT, ACTLAMP_RED);
}

/**
 * Sets the Yellow Light On
 */
void ActivityLight::setYellowOn(void)
{
  hal->writeBit(PORT, ACTLAMP_YELLOW);
}

/**
 * Sets the Yellow Light Off
 */
void ActivityLight::setYellowOff(void)
{
  hal->clearBit(PORT, ACTLAMP_YELLOW);
}
