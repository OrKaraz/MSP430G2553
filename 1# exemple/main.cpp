/*
 * main.cpp
 *
 *  Created on: 12 janv. 2025
 *      Author: david
 */

#include <libILI9341.hpp>
#include <libMaths.hpp>
#include <libWatchdog.hpp>
#include <libPORT.hpp>
#include <libDCO.hpp>
#include <libADC10.hpp>
#include <libFlash.hpp>
#include <libCOM.hpp>

#define tauxUpdate 8    // nombre d'IPS

void main(void) {
    unsigned int l;
    unsigned int deadTime = 0;
    WDTCTL = WDTPW | WDTHOLD;

    COM::init();

    PORT::init( 0,
                ili9341RESET | ili9341COMP | ili9341SELECT,
                0);

    DCO::init(16000000/4096, 100000/4096);
    FLASH::getFlash(FLASHD);

    ILI9341::init();
    ADC10::init();

    l = 239;
    do {
        ILI9341::drawLine(0, l, 319, l, ILI9341::rvb(l, l, l));
    } while (l--);

    ILI9341::drawLine(160, 120, 170, 125, 0xF800);
    ILI9341::drawLine(160, 120, 165, 130, 0xF800);
    ILI9341::drawLine(160, 120, 155, 130, 0xF800);
    ILI9341::drawLine(160, 120, 150, 125, 0xF800);
    ILI9341::drawLine(160, 120, 150, 115, 0xF800);
    ILI9341::drawLine(160, 120, 155, 110, 0xF800);
    ILI9341::drawLine(160, 120, 165, 110, 0xF800);
    ILI9341::drawLine(160, 120, 170, 115, 0xF800);

    unsigned int bcolor = ILI9341::rvb(99, 45, 5);
    unsigned int fcolor = ILI9341::rvb(27, 99, 96);
    ILI9341::fillrect(0, 0, 319, 34, bcolor);
    ILI9341::drawTxt((unsigned char *)"\x01\x02: 100%", 5, 5, 0x001F, bcolor, 1);
    ILI9341::drawTxt((unsigned char *)"\x03\x04: 100%", 186, 5, 0x001F, bcolor, 1);

    while (1) {
        unsigned int i;
        unsigned int y;
        // début de calcul de la charge de travail
        WDT::init();
        WDT::flag = 0;
        // faire des actions

        ILI9341::drawInt(deadTime, 311, 228, 0xFFFF, 0x0000, 1);

        i = 5;
        y = 100;

        do {
            ILI9341::drawInt(FLASH::liste[i].ui, 10, y, 0xFFFF, 0x0000, 0);
            y += 12;
        } while (--i != 0);

        // fin du travail
        deadTime = WDT::flag;
        while (WDT::flag < (512 / tauxUpdate)) {
            LPM0;
        }
    }
}
