/*
 * libPORT.cpp
 *
 *  Created on: 18 janv. 2025
 *      Author: david
 */

#include <libPORT.hpp>

void PORT::init(unsigned char P1, unsigned char P2, unsigned char P3) {
#ifdef QUARTZ32
    #define P2SELQuartz BIT6|BIT7
    #define P2SEL2Quartz 0
    #define P2DIRQuartz BIT7
#else
    #define P2SELQuartz 0
    #define P2SEL2Quartz 0
    #define P2DIRQuartz 0
#endif

#ifdef SPIB
    #define P1SELSPIB BIT5|BIT6|BIT7
    #define P1SEL2SPIB BIT5|BIT6|BIT7
#else
    #define P1SELSPIB 0
    #define P1SEL2SPIB 0
#endif

#ifdef SPIA
    #define P1SELSPIA BIT1|BIT2|BIT4
    #define P1SEL2SPIA BIT1|BIT2|BIT4
#else
    #define P1SELSPIA 0
    #define P1SEL2SPIA 0
#endif

    P1SEL = P1SELSPIB | P1SELSPIA;
    P1SEL2 = P1SEL2SPIB | P1SEL2SPIA;
    P1DIR = P1;

    P2SEL = P2SELQuartz;
    P2SEL2 = P2SEL2Quartz;
    P2DIR = P2 | P2DIRQuartz;

    P3SEL = 0;
    P3SEL2 = 0;
    P3DIR = P3;
}
