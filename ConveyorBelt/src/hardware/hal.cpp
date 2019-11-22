#include "Bib/Util.h"
#include "hal.h"

using namespace hardware;

/**
 * Gain Access to the GPIO Ports for QNX
 */
Hal::Hal()
		: gpioValues(-1), higthAdc(-1)
{
	gpio0.gainAccess(GPIO_0_BASE);
	gpio1.gainAccess(GPIO_1_BASE);
	gpio2.gainAccess(GPIO_2_BASE);
	gpio2.configureOutput(0xffffffc3);
}

Hal::~Hal()
{
	tscadc.eventInterruptDisable(END_OF_SEQUENCE_INT);
}

/**
 * Sets the given Pin to 1 on the given gpio
 */
void Hal::writeBit(unsigned char gpio, unsigned char pin)
{
	int test = gpio;
	switch (test)
	{
	case 0:
		gpio0.set(_BV(pin));
		break;

	case 1:
		gpio1.set(_BV(pin));
		break;

	case 2:
		gpio2.set(_BV(pin));
		break;

	default:
		break;
	}
}

/**
 * sets the given pin to 0 on the given gpio
 */
void Hal::clearBit(unsigned char gpio, unsigned char pin)
{
	switch (gpio)
	{
	case 0:
		gpio0.clear(_BV(pin));
		break;

	case 1:
		gpio1.clear(_BV(pin));
		break;

	case 2:
		gpio2.clear(_BV(pin));
		break;

	default:
		break;
	}
}

/**
 * Reads the given Gpio and saves it in a variable
 */
void Hal::readGpio(unsigned char gpio)
{
	switch (gpio)
	{
	case 0:
		gpioValues = gpio0.read();
		break;

	case 1:
		gpioValues = gpio1.read();
		break;

	case 2:
		gpioValues = gpio2.read();
		break;

	default:
		gpioValues = -1;
		break;
	}
}

void Hal::readAdc()
{
	higthAdc = tscadc.fifoADCDataRead(Fifo::FIFO_0);
}

void Hal::enableADC(unsigned int steps)
{
	tscadc.moduleStateSet(false);
	int maxSteps = steps < 16 ? steps : 16;
	for (int i = 1; i <= maxSteps; i++)
	{
		tscadc.configureStepEnable(i, true);
	}
	tscadc.moduleStateSet(true);

}

void Hal::disableADC()
{
	tscadc.moduleStateSet(false);
}

/**
 * Gives the value of the pin, saved in the variable.
 * To update the variable with the actual Live data readGPIO(gpio) needs to called
 */
int Hal::getPinValue(unsigned char pin)
{
	return gpioValues & 1 << pin;
}

int Hal::getGpioAddress(char gpio)
{
	switch (gpio)
	{
	case 0:
		return gpio0.getBaseAddress();
		break;

	case 1:
		return gpio1.getBaseAddress();
		break;

	case 2:
		return gpio2.getBaseAddress();
		break;

	default:
		return -1;
		break;
	}
}

int Hal::getHightValue()
{
	return higthAdc;
}


void Hal::clearADCInterrupt()
{
	unsigned int status = tscadc.intStatus();
	tscadc.intStatusClear(status);

}

//-------------------------------------- ADC Functions

void Hal::initADC(void)
{
	/* Configures ADC to 3Mhz */
	tscadc.configureAFEClock(24000000, 3000000);

	/* Disable Transistor bias */
	tscadc.tsTransistorConfig(false);

	tscadc.stepIDTagConfig(true);

	/* Disable Write Protection of Step Configuration regs*/
	tscadc.stepConfigProtectionDisable();

	/* Configure step 1 for channel 1(AN0)*/
	stepConfigure(0, Fifo::FIFO_0, POSITIVE_INP_CHANNEL1);

	/* General purpose inputs */
	tscadc.tsModeConfig(GENERAL_PURPOSE_MODE);
	tscadc.fifoIRQThresholdLevelConfig(Fifo::FIFO_0, 1);

	/* Clear the status of all interrupts */
	//cleanUpInterrupts();
	tscadc.intStatusClear(0x7FF);
	tscadc.intStatusClear(0x7FF);
	tscadc.intStatusClear(0x7FF);

	/* End of sequence interrupt is enable */
	tscadc.eventInterruptEnable(END_OF_SEQUENCE_INT);

}

void Hal::stepConfigure(unsigned int stepSel, Fifo fifo,
		PositiveInput positiveInpChannel)
{
	/* Configure ADC to Single ended operation mode */
	tscadc.tsStepOperationModeControl(SINGLE_ENDED_OPER_MODE, stepSel);

	/* Configure step to select Channel, refernce voltages */
	tscadc.tsStepConfig(stepSel, NEGATIVE_REF_VSSA, positiveInpChannel,
			NEGATIVE_INP_CHANNEL1, POSITIVE_REF_VDDA);

	/* XPPSW Pin is on, Which pull up the AN0 line*/
	tscadc.tsStepAnalogSupplyConfig(true, false, false, stepSel);

	/* XNNSW Pin is on, Which pull down the AN1 line*/
	tscadc.tsStepAnalogGroundConfig(true, false, false, false, stepSel);

	/* select fifo 0 or 1*/
	tscadc.tsStepFIFOSelConfig(stepSel, fifo);

	/* Configure ADC to one shot mode */
	//tscadc.tsStepModeConfig(stepSel, CONTINIOUS_SOFTWARE_ENABLED); //changed---------------------------
	tscadc.tsStepModeConfig(stepSel, ONE_SHOT_SOFTWARE_ENABLED);

	tscadc.tsStepAverageConfig(stepSel, NO_SAMPLES_AVG);
}

