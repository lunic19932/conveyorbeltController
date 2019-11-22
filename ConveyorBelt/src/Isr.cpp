/*
 * Isr.cpp
 *
 *  Created on: 17.04.2018
 *      Author: Luis
 */

#include "Isr.h"
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include "hardware/Bib/GPIO.h"
#include "hardware/Bib/Util.h"
#include "communication/PulseId.h"

using namespace std;


Isr::Isr (hardware::Controller &controller,int channelID)
: baseAddress_(controller.getSensorGpioAddress())
,controller_(&controller)
, isrGpioId_(-1)
, isrAdcId_(-1)
{
  int pinMask=_BV(SN_LB_ENTRY)|_BV(SN_LB_IN_HIGHT) |_BV(SN_HIGHT_OK) |_BV(SN_SWITCH_IN) |
	      _BV(SN_METALL)|_BV(SN_SWITCH_OPEN)|_BV(SN_LB_RAMP )|_BV(SN_LB_OUT)|_BV(BTN_START) |_BV(BTN_STOP)|
	      _BV(BTN_RESET)|_BV(BTN_E_STOP);

  connectionId_ = ConnectAttach_r(0, 0, channelID, 0, 0);
     if (connectionId_ < 0) {
     		DBG_ERROR("Fail Channel create:Isr" + connectionId_);
     		exit (-1);
     }
  registerISR(pinMask);

}

Isr::~Isr ()
{
  if(isrGpioId_<0){
      unregisterISR();
  }
}
int Isr::getPendingIntFlags(){
   return in32(baseAddress_ + IRQSTATUS(GPIO_INT_LINE_2));
}

void Isr::disableInterrupts(int pinMask) {
	out32(baseAddress_ + IRQSTATUS_CLR(GPIO_INT_LINE_2), pinMask);
}

void Isr::enableInterrupts(int pinMask) {
	out32(baseAddress_ + IRQSTATUS_SET(GPIO_INT_LINE_2), pinMask);
}

void Isr::clearPendingIntFlag(int bit) {
	out32(baseAddress_ + IRQSTATUS(GPIO_INT_LINE_2), 1 << bit);
}

void Isr::clearAllPendingIntFlag() {
	out32(baseAddress_ + IRQSTATUS(GPIO_INT_LINE_2), 0xffffffff);
}

void Isr::setInterruptEdgeSensitive(int pinMask){
	/* Enabling rising edge detect interrupt generation. */
	*(int*)(baseAddress_ + RISINGDETECT) |= pinMask;

	/* Enabling falling edge detect interrupt generation. */
	*(int*)(baseAddress_ + FALLINGDETECT) |= pinMask;

	/* Disabling logic LOW level detect interrupt generation. */
	*(int*)(baseAddress_ + LEVELDETECT(0)) &= ~pinMask;

	/* Disabling logic HIGH level detect interrupt generation. */
	*(int*)(baseAddress_ + LEVELDETECT(1)) &= ~pinMask;
}

const struct sigevent* Isr::IsrGPIO(void* arg, int id) {
    Isr* isr = (Isr*) arg;
    isr->controller_->updateSensorValues();
    isr->isrEvent_.sigev_value.sival_int = ISRGPIO;

    //TODO implement sigevent
    //Int variable?

    isr->clearAllPendingIntFlag();
    return &(isr->isrEvent_);
}
const struct sigevent* Isr::IsrADC(void* arg, int id) {
    Isr* isr = (Isr*) arg;
    isr->isrEvent_.sigev_value.sival_int = ISRADC;
    isr->controller_->updateHightADCSensor();
    isr->controller_->resetAdcInterrupt();
    return &(isr->isrEvent_);
}
void Isr::registerISR(int pinMask){

    // disable IRQ
    disableInterrupts(0xffffffff);

    // clear any possible pending irq
    clearAllPendingIntFlag();

    setInterruptEdgeSensitive(pinMask);

	//initialize event structure
    SIGEV_PULSE_INIT(&isrEvent_, connectionId_, SIGEV_PULSE_PRIO_INHERIT, ISR, 0);

    // Attach mainISR handler to hardware interrupt
    isrGpioId_ = InterruptAttach(HW_INTERRUPT, IsrGPIO, this, sizeof(isrEvent_), 0);

    //cout<<"isrId: "<<isrId_<<endl;
    if (isrGpioId_ == -1) {
        DBG_ERROR("Registrierung der ISR-Gpio Fehlgeschlagen ");
        exit(EXIT_FAILURE);
    }

    isrAdcId_ = InterruptAttach(ADC_TSC_GENINT, IsrADC, this, sizeof(isrEvent_), 0);
    if (isrAdcId_ == -1) {
           DBG_ERROR("Registrierung der ISR-ADC Fehlgeschlagen ");
           exit(EXIT_FAILURE);
       }

    // enable IRQ
    enableInterrupts(pinMask);
}

void Isr::unregisterISR(){

    // Detach interrupt handler
    if( InterruptDetach(isrGpioId_) < 0 ){
        DBG_ERROR("Could not detach interrupt handler");
        exit(EXIT_FAILURE);
    }

    // disable IRQ
    disableInterrupts(0xffffffff);

    // clear any possible pending irq
    clearAllPendingIntFlag();
}


