/*
 * Switch.cpp
 *
 *  Created on: 03.04.2018
 *      Author: Johannes Kruber
 */

#include "Switch.h"

using namespace hardware;

Switch::Switch (Hal *hal)
: hal(hal)
{

}

Switch::~Switch ()
{
  // TODO Auto-generated destructor stub
}

/**
 * Open the Gate
 */
void Switch::open()
{
  hal->writeBit(1, SWITCH);
}

/**
 * Close the Gate
 */
void Switch::close()
{
  hal->clearBit(1, SWITCH);
}
