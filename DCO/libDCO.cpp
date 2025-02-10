/*
 * libDCO.cpp
 *
 *  Created on: 21 janv. 2025
 *      Author: david
 */

#include <libDCO.hpp>

void DCO::init(int Hz4096, int errHz4096) {
    // sauvegarde l'état en cas d'utilisation en cours de programme
    unsigned int sTA0CCR0 = TA0CCR0;
    unsigned int sTA0CCTL0 = TA0CCTL0;
    unsigned int sTA0CTL = TA0CTL;
    unsigned char sBCSCTL1 = BCSCTL1;
    unsigned char sBCSCTL2 = BCSCTL2;
    unsigned char sBCSCTL3 = BCSCTL3;
    int compare;
    int difference;
    unsigned int securite = 0;  // pour ne pas faire une boucle infinie
    unsigned int subsecurite = 3600;   // securite d'attente du timer
    unsigned char comptage = 4;

    BCSCTL1 |= DIVA_3;                        // ACLK = LFXT1CLK/8
    BCSCTL2 = 0;                              // pas de diviseur sur MCLK et SMCLK
    BCSCTL3 = 0x05;                           // valeur par défaut pour utiliser le quartz 32 768Hz
    TA0CCTL0 = CM_1 + CCIS_1 + CAP;            // CAP, ACLK (inbuild CCI0B connecté à ACLK)
    TA0CTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

    WDT::init();
    WDT::flag = 0;
    LPM0;        // pause de 2ms pour attendre la stabilité de ACLK

    while (--securite > 0) {              // max 32 * 7 * 16 boucles
        subsecurite = 600;
        while (!(CCIFG & TA0CCTL0) && (--subsecurite > 0)) ;
                                        // attente du signal du timer ou de la fin de comptage
                                        // 9 cycles d'horloge donc :
                                        //       - à 20MHz / 4096 = 4883 cycles pour le timer
                                        //       - 4883 / 9 = 543 boucles
        compare = TA0CCR0;                       // récupération de la valeur
        TA0CTL = TASSEL_2 + MC_2 + TACLR;
        TA0CCTL0 &= ~CCIFG;                      // effacement du drapeau

        difference = Hz4096 - compare;                   // calcul de l'écart
        if (difference < 0) difference = -difference;   // évite l'utilisation de abs() en appel de fonction

        if (difference < errHz4096) {
            if (--comptage == 0) break;      // si bonne valeur 4 fois => sortie de la boucle
        }
        else {
            comptage = 4;
            if (Hz4096 < compare) {
                --DCOCTL;                             // DCO trop rapide
                if (__get_SR_register() & BIT8)       // DCO overflow ?
                    if (BCSCTL1 & 0x0f) --BCSCTL1;    // Sélectionner le RSEL précédent
            }
            else {
                ++DCOCTL;                             // DCO trop lent
                if (__get_SR_register() & BIT8)                   // DCO overfow ?
                    if ((BCSCTL1 & 0x0f) != 0x0f) ++BCSCTL1;      // Sélectionner le RSEL suivant
            }
        }
    }

    // restaure l'état
    BCSCTL3 = sBCSCTL3;
    BCSCTL2 = sBCSCTL2;
    BCSCTL1 = (sBCSCTL1 & 0xF0) | (BCSCTL1 & 0x0F) ; // garde le RSEL actf
    TA0CTL = sTA0CTL;
    TA0CCTL0 = sTA0CCTL0;
    TA0CCR0 = sTA0CCR0;
}

unsigned long DCO::getMCLK() {
// sauvegarde l'état en cas d'utilisation en cours de programme
    unsigned int sTA0CCR0 = TA0CCR0;
    unsigned int sTA0CCTL0 = TA0CCTL0;
    unsigned int sTA0CTL = TA0CTL;
    unsigned char sBCSCTL1 = BCSCTL1;
    unsigned char sBCSCTL2 = BCSCTL2;
    unsigned char sBCSCTL3 = BCSCTL3;
    int capture;

    BCSCTL1 |= DIVA_3;                        // ACLK = LFXT1CLK/8
    BCSCTL2 = 0;                              // pas de diviseur sur MCLK et SMCLK
    BCSCTL3 = 0x05;                           // valeur par défaut pour utiliser le quartz 32 768Hz
    TA0CCTL0 = CM_1 + CCIS_1 + CAP;            // CAP, ACLK (inbuild CCI0B connecté à ACLK)
    TA0CTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

    while (!(CCIFG & TA0CCTL0)) ;   // attente du signal du timer     ~0.2ms de calcul max
    TA0CCTL0 &= ~CCIFG;                      // effacement du drapeau
    while (!(CCIFG & TA0CCTL0)) ;   // première valeur peut être erronée (ACLK != 0 au premier run)
                                    //     ~0.2ms de calcul max
    TA0CCTL0 &= ~CCIFG;                      // effacement du drapeau
    capture = TA0CCR0;                       // récupération de la valeur

    // restaure l'état
    BCSCTL3 = sBCSCTL3;
    BCSCTL2 = sBCSCTL2;
    BCSCTL1 = sBCSCTL1;
    TA0CTL = sTA0CTL;
    TA0CCTL0 = sTA0CCTL0;
    TA0CCR0 = sTA0CCR0;

    return ((unsigned long)(capture) << 12);
}
