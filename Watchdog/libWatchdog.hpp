/*
 * libWatchdog.hpp
 *
 *  Created on: 12 janv. 2025
 *      Author: david
 */

#ifndef INCLUDE_LIBWATCHDOG_HPP_
#define INCLUDE_LIBWATCHDOG_HPP_

#define SECURITE        // commenter la ligne pour désactiver
#define WDTTIMER 512    // nombre d'incrémentation avant RESET (max = 65 535)

#include <msp430.h>

namespace WDT {
    void init();
    void WDTInterrupt();

    unsigned int flag;
};

#endif /* INCLUDE_LIBWATCHDOG_HPP_ */
