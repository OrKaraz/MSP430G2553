/*
 * libWatchdog.cpp
 *
 *  Created on: 12 janv. 2025
 *      Author: david
 */

#include <libWatchdog.hpp>

void WDT::init() {
    WDT::flag = 0;
    WDTCTL = WDT_ADLY_1_9; // tous les 1,953125ms (ACLK avec diviseur = 1)
    IE1 = WDTIE; // activation des interruptions watchdog
    __enable_interrupt();
}

#pragma vector=WDT_VECTOR
__interrupt void WDT::WDTInterrupt() {
    WDT::flag++;
#ifdef SECURITE
    if (WDT::flag == WDTTIMER) WDTCTL = 0;
#endif
    //__bic_SR_register_on_exit(LPM0_bits);
    LPM0_EXIT;
}
