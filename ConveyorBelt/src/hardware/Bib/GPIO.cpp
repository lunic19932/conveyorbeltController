#include <sys/neutrino.h>
#include <hw/inout.h>
#include <sys/mman.h>

#include "Util.h"
#include "GPIO.h"

#define BASE_SIZE 		0x00001000
#define SETDATAOUT		0x194		//Offset for register to set output bits
#define CLEARDATAOUT	0x190		//Offset for register to clear output bits
#define DATAIN			0x138		//Offset for register to read input bits
#define DATAOUT			0x13C		//Offset for register to write directly output bits
#define OE				0x134		//set as OUTPUT

Gpio::Gpio(void)
: baseAddress_(MAP_DEVICE_FAILED)
{

}

Gpio::~Gpio(){
	if(baseAddress_!=MAP_DEVICE_FAILED){
		munmap_device_io(baseAddress_, BASE_SIZE);
	}
}

void Gpio::gainAccess(unsigned int baseAddress){
	int err = ThreadCtl(_NTO_TCTL_IO_PRIV, 0);
	if (err < 0) {
		DBG_ERROR("ThreadCtl");
	}
	baseAddress_ = mmap_device_io(BASE_SIZE, baseAddress);
	if(baseAddress == MAP_DEVICE_FAILED){
		DBG_ERROR("failed to mmap device");
	}
}

void Gpio::set(unsigned int val){
    out32(baseAddress_+SETDATAOUT, val);
}

void Gpio::write(unsigned int val){
    out32(baseAddress_+DATAOUT, val);
}

void Gpio::clear(unsigned int val){
	out32(baseAddress_+CLEARDATAOUT, val);
}

int Gpio::read(void){
    return in32(baseAddress_+(int)DATAIN);
}

void Gpio::configureOutput(uint32_t mask){
	out32(baseAddress_+(int)OE, mask);
}

int Gpio::getBaseAddress(void){
    return baseAddress_;
}

