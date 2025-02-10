/*
 * libDCO.hpp
 *
 *  Created on: 21 janv. 2025
 *      Author: david
 */

#ifndef INCLUDE_LIBDCO_HPP_
#define INCLUDE_LIBDCO_HPP_

// quartz de 32768Hz obligatoire sur P2.6 et P2.7

#include <msp430.h>
#include <libWatchdog.hpp>

namespace DCO {
    void init(int Hz4096, int errHz4096);
    // errHz = Ecart de fréquence toléré (Hz). Valeur maximale sinon tronqué : 1000000
    unsigned long getMCLK();
};

#endif /* INCLUDE_LIBDCO_HPP_ */
