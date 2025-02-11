/*
 * libPORT.hpp
 *
 *  Created on: 18 janv. 2025
 *      Author: david
 */

#ifndef INCLUDE_LIBPORT_HPP_
#define INCLUDE_LIBPORT_HPP_

#define QUARTZ32
#define SPIB
//#define SPIA

#include <msp430.h>

namespace PORT {
    void init(unsigned char P1, unsigned char P2, unsigned char P3); // port souhaité en sortie
};

#endif /* INCLUDE_LIBPORT_HPP_ */
