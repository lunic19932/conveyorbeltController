/*
 * Isr.h
 *
 *  Created on: 17.04.2018
 *      Author: Luis
 */

#ifndef SRC_ISR_H_
#define SRC_ISR_H_


#include <sys/siginfo.h>
#include "hardware/Controller.h"


#define	LEVELDETECT(n) 	(0x140 + (n * 4))
#define	RISINGDETECT    0x148
#define	FALLINGDETECT   0x14C
#define	DEBOUNCENABLE   0x150
#define	DEBOUNCINGTIME  0x154
#define	CLEARDATAOUT    0x190
#define	SETDATAOUT      0x194
#define	IRQSTATUS_RAW(n)	(0x024 + (n * 4))
#define	IRQSTATUS(n)  		(0x02C + (n * 4))
#define	IRQSTATUS_SET(n)	(0x034 + (n * 4))
#define	IRQSTATUS_CLR(n)	(0x03C + (n * 4))
#define	IRQWAKEN(n)		(0x044 + (n * 4))

#define GPIO_INT_LINE_1 0x0
#define GPIO_INT_LINE_2 0x1

#define LOW_LEVEL_DETECTION	0x0
#define HIGH_LEVEL_DETECTION	0x1

#define HW_INTERRUPT 97
#define ADC_TSC_GENINT 	 		16



class Isr
{
    private:
    int getPendingIntFlags();
    void clearPendingIntFlag(int bit);
    void clearAllPendingIntFlag();
    void enableInterrupts(int pinMask);
    void disableInterrupts(int pinMask);
    void setInterruptEdgeSensitive(int pinMask);
    static const struct sigevent* IsrGPIO(void* arg, int id);
    static const struct sigevent* IsrADC(void* arg, int id);

    struct sigevent isrEvent_;
    int baseAddress_;
    hardware::Controller *controller_;
    int isrGpioId_;
    int isrAdcId_;
    int connectionId_;

  public:
    Isr (hardware::Controller &controller,int connectionId);
    virtual ~Isr ();
    void registerISR(int pinMask);
    void unregisterISR(void);



};

#endif /* SRC_ISR_H_ */
