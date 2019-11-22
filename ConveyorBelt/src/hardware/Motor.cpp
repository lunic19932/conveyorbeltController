/*
 * Motor.cpp
 *
 *  Created on: 29.03.2018
 *      Author: Johannes Kruber
 */

#include "Motor.h"

using namespace hardware;

#define PORT 1

Motor::Motor (Hal *hal)
: hal(hal)
{
  reset();
}

Motor::~Motor ()
{
  // TODO Auto-generated destructor stub
}

/**
 * Set the Motor to run left round
 */
void
Motor::setLeft (void)
{
  hal->clearBit(PORT, MOTOR_RIGHT);
  hal->writeBit(PORT, MOTOR_LEFT);
}

/**
 * Set the Motor to run right round right round
 */
void
Motor::setRight (void)
{
  hal->clearBit(PORT, MOTOR_LEFT);
  hal->writeBit(PORT, MOTOR_RIGHT);
}

/**
 * Set the Motor to run slow
 */
void
Motor::setSlow (void)
{
  hal->writeBit(PORT, MOTOR_SLOW);
}

/**
 * Set the Motor to run on normal speed
 */
void
Motor::resetSlow (void)
{
  hal->clearBit(PORT, MOTOR_SLOW);
}

/**
 * Set the motor to start running
 */
void
Motor::start (void)
{
  hal->clearBit(PORT, MOTOR_STOP);
}

/**
 * Holds the motor in its Status but stops the movement of the belt
 */
void
Motor::hold (void)
{
  hal->clearBit(PORT, MOTOR_LEFT);
  hal->clearBit(PORT, MOTOR_RIGHT);
}

/**
 * Stops the motor
 */
void
Motor::stop (void)
{
  hal->writeBit(PORT, MOTOR_STOP);
}

/**
 * Resets the Motor settings
 */
void
Motor::reset (void)
{
  hal->writeBit(PORT, MOTOR_STOP);
  hal->clearBit(PORT, MOTOR_LEFT);
  hal->clearBit(PORT, MOTOR_RIGHT);
  hal->clearBit(PORT, MOTOR_SLOW);
}
