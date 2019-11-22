/*
 * Motor.h
 *
 *  Created on: 29.03.2018
 *      Author: Johannes Kruber
 */


#ifndef SRC_MOTOR_H_
#define SRC_MOTOR_H_

#include "hal.h"

namespace hardware
{
class Motor
{
private:
  Hal *hal;
public:
  Motor (Hal *hal);
  virtual
  ~Motor ();

  void setLeft(void);
  void setRight(void);
  void setSlow(void);
  void resetSlow(void);
  void start(void);
  void hold(void);
  void stop(void);
  void reset(void);
};
}
#endif /* SRC_MOTOR_H_ */
