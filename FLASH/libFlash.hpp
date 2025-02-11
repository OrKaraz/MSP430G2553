/*
 * libFlash.hpp
 *
 *  Created on: 24 janv. 2025
 *      Author: david
 */

#ifndef INCLUDE_LIBFLASH_HPP_
#define INCLUDE_LIBFLASH_HPP_

#include <msp430.h>
#include <libDCO.hpp>
#include <libMaths.hpp>

#define FLASHB 2
#define FLASHC 1
#define FLASHD 0

/*
Vitesse d'écriture :   257kHz -> 476kHz
*/

namespace FLASH {
    typedef union {
        unsigned int ui[32];
        int i[32];
        unsigned char uc[64];
        signed char c[64];
    } typeint;

    unsigned char getFlash(unsigned char pos);
    unsigned char save(unsigned char pos);

    typeint liste;
};

#endif /* INCLUDE_LIBFLASH_HPP_ */
