/*
 * libFlash.cpp
 *
 *  Created on: 24 janv. 2025
 *      Author: david
 */

#include <libFlash.hpp>

unsigned char FLASH::getFlash(unsigned char pos) {
    unsigned int *ptr;
    unsigned char i;
    switch (pos) {
        case FLASHB:
            ptr = (unsigned int *)0x1080;
            break;
        case FLASHC:
            ptr = (unsigned int *)0x1040;
            break;
        case FLASHD:
            ptr = (unsigned int *)0x1000;
            break;
        default:
            return 0;
    }

    i = 0;
    while (i < 32) {
        FLASH::liste[i].ui= *ptr;             // récupère la mémoire flash
        ptr += 2;
        i++;
    }

    return 1;
}

unsigned char FLASH::save(unsigned char pos) {
    unsigned int sWDTCTL = WDTCTL & 0x00FF;
    unsigned int *ptr;
    unsigned char i;
    mDIVrRET diviseur;

    switch (pos) {
        case FLASHB:
            ptr = (unsigned int *)0x1080;
            break;
        case FLASHC:
            ptr = (unsigned int *)0x1040;
            break;
        case FLASHD:
            ptr = (unsigned int *)0x1000;
            break;
        default:
            return 0;
    }

    WDTCTL = WDTPW | WDTHOLD;        // stop le watchdog par sécurité

    diviseur = mDivR((unsigned int)(DCO::getMCLK() >> 12), 89);
    diviseur.quotient++;

    FCTL2 |= FWKEY + FSSEL_1 + diviseur.quotient;
                        // maintient du DCO et application du diviseur de fréquence
                        // pour la flash

    FCTL1 = FWKEY + ERASE;                    // Set Erase bit
    FCTL3 = FWKEY + LOCKA;                    // Clear Lock bit, segment A reste protégé
    *ptr = 0;                                 // écriture pour effacement de la flash

    FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

    i = 0;
    while (i < 32) {
        *ptr = FLASH::liste[i].ui;             // écrit la flash
        ptr += 2;
        i++;
    }

    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY + LOCK + LOCKA;             // Set LOCK bit

    WDTCTL = sWDTCTL | WDTPW;                // restore le Watchdog
    return 1;
}
