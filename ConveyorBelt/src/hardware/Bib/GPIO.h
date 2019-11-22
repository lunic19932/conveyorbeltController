#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIO_0_BASE 0x44E07000
#define GPIO_1_BASE 0x4804C000
#define GPIO_2_BASE 0x481AC000



class Gpio {
public:
	Gpio(void);
	virtual ~Gpio();

    void gainAccess(unsigned int baseAddress);
    void write(unsigned int val);
    void set(unsigned int val);
    void clear(unsigned int val);
    int read(void);
    void configureOutput(uint32_t mask);
    int getBaseAddress(void);
private:
    uintptr_t baseAddress_;
};

#endif
