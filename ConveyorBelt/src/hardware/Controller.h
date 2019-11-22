/*
 * Controller.h
 *
 *  Created on: 10.04.2018
 *      Author: Johannes Kruber
 */

#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_

#include <thread>
#include <condition_variable>
#include <mutex>

#include "Motor.h"
#include "ActivityLight.h"
#include "Switch.h"
#include "SensorBool.h"
#include "Led.h"

namespace hardware
{
class Controller
{
  private:
    Hal *hal;
    bool running, enabled;
    std::mutex mutex;
    std::condition_variable waitVar;
    std::thread *adcThread;


  public:

    Motor *ac_motor;			//Motor steuerung
    ActivityLight *ac_lamp;		//Ampel Steuerung
    Switch *ac_switch;			//Weichen Steuerung

    SensorBool *sn_lb_start;		//Eingangslichtschranke
    SensorBool *sn_lb_hightSn;		//Lichtschranke am Hoehensensor
    SensorBool *sn_hightControle;	//Indikator ob hoehe ok ist
    SensorBool *sn_lb_switch;		//Lichtschranke  vor Weiche
    SensorBool *sn_metall;		// Metallsensor
    SensorBool *sn_switchOpen;		//Indikator ob schranke offen ist
    SensorBool *sn_lb_ramp;		//Lichtschranke an Rampe (rampe voll)
    SensorBool *sn_lb_end;		//Lichtschranke ende Laufband

    SensorBool *btn_start;		//Startbutton
    SensorBool *btn_stop;		//Stopbutton
    SensorBool *btn_reset;		//resetButton
    SensorBool *btn_emergency;		//E_stopButton

    Led *led_start;			//Start Led
    Led *led_reset;			//Reset Led
    Led *led_q1;			//Q1 Led
    Led *led_q2;			//Q2 Led

    //sn_hight fehlt (analog sensor)
    Controller ();
    Controller (Hal *hal);
    virtual
    ~Controller ();

    void updateSensorValues();
    void updateHightADCSensor();

    int getSensorGpioAddress();

    void resetAdcInterrupt(void);

    void enableAdc(void);

    void disableAdc(void);

    int getHightValue(void);

    void operator()();
};
}
#endif /* SRC_CONTROLLER_H_ */
