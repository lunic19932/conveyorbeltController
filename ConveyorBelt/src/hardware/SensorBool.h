/*
 * SensorBool.h
 *
 *  Created on: 10.04.2018
 *      Author: Johannes Kruber
 */

#ifndef SRC_SENSORBOOL_H_
#define SRC_SENSORBOOL_H_

#include "hal.h"

namespace hardware
{
class SensorBool
{
  public:

    Hal *hal;
    char pin;

    SensorBool (Hal *hal,  char pin);
    virtual
    ~SensorBool ();

    bool isTrue(void);
};
}
#endif /* SRC_SENSORBOOL_H_ */
