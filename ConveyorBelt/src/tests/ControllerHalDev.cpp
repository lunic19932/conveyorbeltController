#include <iostream>
#include <bitset>

#include "../hardware/hal.h"
#include "../hardware/Motor.h"
#include "../hardware/Switch.h"
#include "../hardware/ActivityLight.h"
#include "../hardware/Controller.h"
#include <sys/neutrino.h>


using namespace std;
using namespace hardware;

void halTestGPIO1(void)
{
  cout << "HAL Test" << endl; // prints Hello World!!!
  Hal hal;
  hal.writeBit(1, MOTOR_RIGHT);
  sleep(2);
  hal.writeBit(1, MOTOR_STOP);
  sleep(2);
  hal.clearBit(1, MOTOR_STOP);
  sleep(2);
  hal.writeBit(1, MOTOR_SLOW);
  sleep(2);
  hal.clearBit(1, MOTOR_RIGHT);
  hal.writeBit(1, MOTOR_LEFT);
  sleep(2);
  hal.clearBit(1, MOTOR_SLOW);
  hal.writeBit(1, ACTLAMP_GREEN);
  sleep(1);
  hal.writeBit(1, ACTLAMP_RED);
  sleep(1);
  hal.writeBit(1, ACTLAMP_YELLOW);
  sleep(1);
  hal.writeBit(1, ACTLAMP_GREEN);
  sleep(1);
  hal.writeBit(1, SWITCH);
  sleep(1);

  hal.clearBit(1, MOTOR_LEFT);
  hal.clearBit(1, ACTLAMP_RED);
  hal.clearBit(1, ACTLAMP_YELLOW);
  hal.clearBit(1, ACTLAMP_GREEN);
  hal.clearBit(1, SWITCH);

}

void controlTestGPIO1()
{
  cout << "Controller Test" << endl; // prints Hello World!!!

  Hal hal;
  Motor motor(&hal);
  Switch switch1(&hal);
  ActivityLight light(&hal);

  motor.setLeft();
  motor.start();
  sleep(1);
  motor.setRight();
  sleep(1);
  motor.stop();
  sleep(1);
  motor.setSlow();
  motor.start();
  sleep(1);
  motor.hold();
  sleep(1);
  motor.resetSlow();
  motor.setRight();
  sleep(1);

  motor.stop();

  switch1.open();
  sleep(1);
  switch1.close();

  light.setGreenOn();
  sleep(1);
  light.setYellowOn();
  sleep(1);
  light.setRedOn();
  sleep(1);
  light.setGreenOff();
  sleep(1);
  light.setYellowOff();
  sleep(1);
  light.setRedOff();
}


void snTest()
{
  cout << "snTest" << endl;
  Controller cont;
  while(1)
    {
      cont.updateSensorValues();
      cout<< "LB start: " << cont.sn_lb_start->isTrue() <<endl;
      cout<< "LB hight sn: " << cont.sn_lb_hightSn->isTrue() <<endl;
      cout<< "Hight Controle: " << cont.sn_hightControle->isTrue() <<endl;
      cout<< "Metall SN: " << cont.sn_metall->isTrue() <<endl;
      cout<< "lb in switch: " << cont.sn_lb_switch->isTrue() <<endl;
      cout<< "LB Rampe: " << cont.sn_lb_ramp->isTrue() <<endl;
      cout<< "Switch open: " << cont.sn_switchOpen->isTrue() <<endl;
      cout<< "LB end: " << cont.sn_lb_end->isTrue() <<endl << endl;

      cout<< "BTN start " << cont.btn_start->isTrue() <<endl;
      cout<< "BTN stop " << cont.btn_stop->isTrue() <<endl;
      cout<< "BTN reset " << cont.btn_reset->isTrue() <<endl;
      cout<< "BTN EStop " << cont.btn_emergency->isTrue() <<endl<< endl<< endl << endl;

      sleep(1);
    }
}


void ledTest()
{

  cout << "ledTest" << endl;
  Controller cont;

  cont.led_start->ledOff();
  cont.led_reset->ledOff();
  cont.led_q1->ledOff();
  cont.led_q2->ledOff();

  sleep(1);
  cont.led_start->ledOn();
  sleep(1);
  cont.led_reset->ledOn();
  sleep(1);
  cont.led_q1->ledOn();
  sleep(1);
  cont.led_q2->ledOn();
  sleep(1);

  cont.led_start->ledOff();
  cont.led_reset->ledOff();
  cont.led_q1->ledOff();
  cont.led_q2->ledOff();
}
