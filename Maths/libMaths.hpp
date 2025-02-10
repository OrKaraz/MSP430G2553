/*
 * libMaths.hpp
 *
 *  Created on: 12 janv. 2025
 *      Author: david
 */

#ifndef INCLUDE_LIBMATHS_HPP_
#define INCLUDE_LIBMATHS_HPP_

#include <msp430.h>

typedef struct {
    unsigned int quotient;
    unsigned int reste;
} mDIVrRET;

mDIVrRET mDivR(unsigned int Dividende, unsigned int Diviseur);
unsigned long mMult(unsigned int a, unsigned int b);

#endif /* INCLUDE_LIBMATHS_HPP_ */
