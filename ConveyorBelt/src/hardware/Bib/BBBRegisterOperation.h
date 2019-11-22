/**
 * @file 		BBBRegisterOperation.h
 * @brief
 * @date 		18.02.2016
 * @author: 	T Jaehnichen
 *				HAW-Hamburg
 *          	Labor für technische Informatik
 *          	Berliner Tor  7
 *          	D-20099 Hamburg
 * @version 	
 * @details		
 *				
 */

#ifndef BBBREGISTEROPERATION_H_
#define BBBREGISTEROPERATION_H_

#include <stdint.h>

	/**
     * @brief This API is for setting bits in the given register.
     * @param __addr 	Address of the Register to be manipulated
     * @param __data 	The bitmask. A 1 at bit position(s) sets the corresponding
     * 					bit in the Register, a 0 has no effect.
     */
    static __inline__ void __attribute__((__unused__))
    setBitsInReg(_Uintptrt __addr, _Uint32t __data)
    {
    	*(volatile _Uint32t *)__addr |= __data;
    }

    /**
     * @brief This API is for clearing bits in the given register.
     * @param __addr	Address of the Register to be manipulated
     * @param __data	The bitmask. A 1 at bit position(s) clears the corresponding
     * 					bit in the register, a 0 has no effect.
     */
    static __inline__ void __attribute__((__unused__))
    clearBitsInReg(_Uintptrt __addr, _Uint32t __data)
    {
    	*(volatile _Uint32t *)__addr &= ~(__data);
    }

    /**
       * @brief This API is for toggling bits in the given register.
       * @param __addr	Address of the Register to be manipulated
       * @param __data	The bitmask. A 1 at bit position(s) toggles the corresponding
       * 					bit in the register, a 0 has no effect.
       */
      static __inline__ void __attribute__((__unused__))
      toggleBitsInReg(_Uintptrt __addr, _Uint32t __data)
      {
      	*(volatile _Uint32t *)__addr ^= __data;
      }


#endif /* BBBREGISTEROPERATION_H_ */
