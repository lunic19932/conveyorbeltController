/*
 * Led.h
 *
 *  Created on: 12.04.2018
 *      Author: Johannes Kruber
 */

#ifndef SRC_LED_H_
#define SRC_LED_H_

#include "hal.h"

namespace hardware
{
class Led
{
  public:
    Hal *hal;
    char pin;

    Led (Hal *hal, char pin);
    virtual
    ~Led ();

    void ledOn(void);
    void ledOff(void);
};
}
#endif /* SRC_LED_H_ */
