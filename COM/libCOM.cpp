/*
 * libCOM.cpp
 *
 *  Created on: 27 janv. 2025
 *      Author: david
 */

#include <libCOM.hpp>

void COM::init() {
#ifdef SPIA
    UCA0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;
    UCA0CTL1 = UCSSEL_3;
#endif
#ifdef SPIB
    UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;
    UCB0CTL1 = UCSSEL_3;
#endif
}
