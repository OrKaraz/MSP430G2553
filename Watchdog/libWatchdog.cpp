/*
 * libWatchdog.cpp
 *
 *  Created on: 12 janv. 2025
 *      Author: david
 */

/*
 * Librairie permettant une pause de 1,953125ms sous LPM0
 * 512 pulses par secondes
 *
 * utilisation si pause de 2ms :
 *      WDT::init();
 *      WDT::flag = 0;
 *      LPM0;
 *
 * utilisation si pause de plus de 2ms : (prendre la valeur entière immédiatement supérieur)
 *      WDT::init();
 *      WDT::flag = 0;
 *      do {
 *          LPM0;                      //      pause en ms
 *      } while (WDT::flag < x);       // x =  -----------
 *                                     //       1.953125
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
    //__bic_SR_register_on_exit(LPM0_bits);
    LPM0_EXIT;
}
