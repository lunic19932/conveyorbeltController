/*
 * Switch.h
 *
 *  Created on: 03.04.2018
 *      Author: Johannes Kruber
 */

#ifndef SRC_SWITCH_H_
#define SRC_SWITCH_H_

#include "hal.h"

namespace hardware
{
class Switch
{
  private:
    Hal *hal;

  public:
    Switch (Hal *hal);
    virtual
    ~Switch ();

    void open(void);
    void close(void);
};
}
#endif /* SRC_SWITCH_H_ */
