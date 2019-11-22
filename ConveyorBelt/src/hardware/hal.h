#ifndef HAL_H
#define HAL_H

#include "Bib/GPIO.h"
#include "Bib/TSCADC.h"

//GPIO0
#define SN_LB_ENTRY 2  //Werkstück im Einlauf = low when true
#define SN_LB_IN_HIGHT 3  //Werkstück in Höhenmessung = low when true
#define SN_HIGHT_OK 4  //Werkstück Höhe OK = high when true
#define SN_SWITCH_IN 5  //Werkstück in Weiche = low when true
#define SN_METALL 7  //Werkstück Metall = high when true
#define SN_SWITCH_OPEN 14 //Weiche offen = high when true
#define SN_LB_RAMP 15 //Rutsche voll = low when true
#define SN_LB_OUT 20 //Werkstück im Auslauf = low when true
#define BTN_START 22 //TasteStart = high, wenn betätigt
#define BTN_STOP 23 //TasteStop = low, wenn betätigt
#define BTN_RESET 26 //TasteReset = high, wenn betätigt
#define BTN_E_STOP 27 //E-Stop = low, wenn betätigt

// GPIO1
#define MOTOR_RIGHT 12
#define MOTOR_LEFT 13
#define MOTOR_SLOW 14
#define MOTOR_STOP 15
#define ACTLAMP_RED 16
#define ACTLAMP_YELLOW 17
#define ACTLAMP_GREEN 18
#define SWITCH 19

//GPIO2
#define LED_START 2 //LED Taste Start
#define LED_RESET 3 //LED Taste Reset
#define LED_Q1 4 //Signalleuchte Q1
#define LED_Q2 5 //Signalleuchte Q2

namespace hardware
{
class Hal
{
private:
	Gpio gpio0, gpio1, gpio2;
	TSCADC tscadc;
	int gpioValues, higthAdc;

public:
	Hal(void);
	virtual ~Hal();

	/**
	 * Sets the given Pin to 1 on the given gpio
	 */
	void writeBit(unsigned char gpio, unsigned char pin);

	/**
	 * sets the given pin to 0 on the given gpio
	 */
	void clearBit(unsigned char gpio, unsigned char pin);

	/**
	 * Reads the given Gpio and saves it in a variable
	 */
	void readGpio(unsigned char gpio);

	void readAdc(void);

	void enableADC(unsigned int steps);

	void disableADC(void);

	/**
	 * Gives the value of the pin, saved in the variable.
	 * To update the variable with the actual Live data readGPIO(gpio) needs to called
	 */
	int getPinValue(unsigned char pin);

	int getGpioAddress(char gpio);

	int getHightValue();
	void clearADCInterrupt(void);

	//---------------- ADC Functions

	void initADC(void);

private:

	void stepConfigure(unsigned int stepSel, Fifo fifo,
			PositiveInput positiveInpChannel);

};
}
#endif
