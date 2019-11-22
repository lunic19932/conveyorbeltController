/*
 * ActivityLight.h
 *
 *  Created on: 29.03.2018
 *      Author: Johannes Kruber
 */

#ifndef SRC_ACTIVITYLIGHT_H_
#define SRC_ACTIVITYLIGHT_H_

#include "hal.h"

namespace hardware
{
class ActivityLight
{
  private:
    Hal *hal;
  public:
    ActivityLight (Hal *hal);
    virtual
    ~ActivityLight ();

    void setGreenOn(void);
    void setGreenOff(void);

    void setYellowOn(void);
    void setYellowOff(void);

    void setRedOn(void);
    void setRedOff(void);
};
}
#endif /* SRC_ACTIVITYLIGHT_H_ */
