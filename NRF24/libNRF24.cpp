/*
 * libNRF24.cpp
 *
 *  Created on: 24 févr. 2025
 *      Author: david
 */

#include <libNRF24.hpp>

static unsigned char saveSMCLK;
static unsigned int savePrescal;
static unsigned char saveUCCTL1;

void NRF24::init() {
    unsigned char *l = (unsigned char *)NRF24::tInit;
    // attente de 100ms du reveil
    WDT::init();
    do {
        LPM0;
    } while (WDT::flag < 53);
    // envoyer la configuration
    NRF24::sendArray(*l, l + 1);
    // attendre 2ms d'action du power on
    WDT::init();
    LPM0;
}

void NRF24::sendArray(unsigned char nb, unsigned char* l) {
    NRF24::selectON();
    if (!nb) return;

    do {
        #if defined(NRF24SPIA0)
            while (!(IFG2 & UCA0TXIFG));
        #elif defined(NRF24SPIB0)
            while (!(IFG2 & UCB0TXIFG));
        #endif
        NRF24::send(*l);
        l++;
    } while (--nb);

#if defined(NRF24SPIA0)
    while (UCA0STAT & UCBUSY);
#elif defined(NRF24SPIB0)
    while (UCB0STAT & UCBUSY);
#endif

    NRF24::selectOFF();
}

inline void NRF24::send(unsigned char s) {
    #if defined(NRF24SPIA0)
        UCA0TXBUF = s;
    #elif defined(NRF24SPIB0)
        UCB0TXBUF = s;
    #endif
}

void NRF24::selectON() {
    saveSMCLK = BCSCTL2;
    BCSCTL2 &= 0xF1;  // désactivation du potentiel diviseur de SMCLK et utilisation de DCO
#ifdef NRF24SPIA0
    saveUCCTL1 = UCA0CTL1;
    UCA0CTL1 |= 0xC0;
    savePrescal = UCA0BR0 | (UCA0BR1 << 8);
    UCA0BR0 = 1;
    UCA0BR1 = 0;
#endif
#ifdef NRF24SPIB0
    saveUCCTL1 = UCB0CTL1;
    UCB0CTL1 |= 0xC0;
    savePrescal = UCB0BR0 | (UCB0BR1 << 8);
    UCB0BR0 = 2;
    UCB0BR1 = 0;
#endif

    #if defined(NRF24selP1)
        P1OUT &= ~NRF24selBIT;
    #elif defined(NRF24selP2)
        P2OUT &= ~NRF24selBIT;
    #elif defined(NRF24selP3)
        P3OUT &= ~NRF24selBIT;
    #endif
}

void NRF24::selectOFF() {
// reload save vars
    #if defined(NRF24selP1)
        P1OUT |= NRF24selBIT;
    #elif defined(NRF24selP2)
        P2OUT |= NRF24selBIT;
    #elif defined(NRF24selP3)
        P3OUT |= NRF24selBIT;
    #endif

    BCSCTL2 = saveSMCLK; // restauration de l'état de SMCLK
#ifdef NRF24SPIA0
    UCA0CTL1 = saveUCCTL1;
    UCA0BR0 = savePrescal & 0x00FF;
    UCA0BR1 = (savePrescal >> 8);
#endif
#ifdef NRF24SPIB0
    UCB0CTL1 = saveUCCTL1;
    UCB0BR0 = savePrescal & 0x00FF;
    UCB0BR1 = (savePrescal >> 8);
#endif
}

#if defined(NRF24CEP1) || defined(NRF24CEP2) || defined(NRF24CEP3)
    inline void NRF24::veille() {
        #if defined(NRF24CEP1)
            P1OUT &= ~NRF24CEBIT;
        #elif defined(NRF24CEP2)
            P2OUT &= ~NRF24CEBIT;
        #elif defined(NRF24CEP3)
            P3OUT &= ~NRF24CEBIT;
        #endif
    }

    inline void NRF24::reveil() {
        #if defined(NRF24CEP1)
            P1OUT |= NRF24CEBIT;
        #elif defined(NRF24CEP2)
            P2OUT |= NRF24CEBIT;
        #elif defined(NRF24CEP3)
            P3OUT |= NRF24CEBIT;
        #endif
    }
#endif

#if defined(NRF24rstP1) || defined(NRF24rstP2) || defined(NRF24rstP3)
    inline void NRF24::RESETON() {
        #if defined(NRF24rstP1)
            P1OUT &= ~NRF24rstBIT;
        #elif defined(NRF24rstP2)
            P2OUT &= ~NRF24rstBIT;
        #elif defined(NRF24rstP3)
            P3OUT &= ~NRF24rstBIT;
        #endif
    }

    inline void NRF24::RESETOFF() {
        #if defined(NRF24rstP1)
            P1OUT |= NRF24rstBIT;
        #elif defined(NRF24rstP2)
            P2OUT |= NRF24rstBIT;
        #elif defined(NRF24rstP3)
            P3OUT |= NRF24rstBIT;
        #endif
    }
#endif
