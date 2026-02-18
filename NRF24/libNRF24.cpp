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
    LPM0;
}

void NRF24::setRegistreMult(char nb, unsigned char *add) {
    NRF24::selectON();
    NRF24::send(W_REGISTER | *add);
    do {
        while (!(IFG2 & UCB0TXIFG));
        add++;
        NRF24::send(*add);
    } while ((--nb) != 0);
    NRF24::selectOFF();
}

void NRF24::getRegistreMult(char nb, unsigned char *add) {
    NRF24::selectON();
    NRF24::send(*add);
    do {
        while (!(IFG2 & UCB0RXIFG));
        add++;
        NRF24::send(*add);
        *(add-1) = NRF24::get();
    } while (--nb);
    NRF24::selectOFF();
}

unsigned char NRF24::setRegister(unsigned char r, unsigned char v) {
    unsigned char ret;
    r = r & 0x1F;
    NRF24::selectON();
    NRF24::send(W_REGISTER | r);
    while (UCB0STAT & UCBUSY);
    ret = NRF24::get();
    NRF24::send(v);
    NRF24::selectOFF();
    return ret;
}

unsigned char NRF24::getRegister(unsigned char r, unsigned char* v) {
    unsigned char ret = 0;
    r = r & 0x1F;
    NRF24::selectON();
    NRF24::send(r);
    while (UCB0STAT & UCBUSY);
    NRF24::send(ret);
    ret = NRF24::get();
    while (UCB0STAT & UCBUSY);
    *v = NRF24::get();
    NRF24::selectOFF();
    return ret;
}

inline void NRF24::send(unsigned char s) {
#ifdef NRF24SPIA0
    UCA0TXBUF = s;
#endif
#ifdef NRF24SPIB0
    UCB0TXBUF = s;
#endif
}

inline unsigned char NRF24::get() {
#ifdef NRF24SPIA0
    return UCA0RXBUF;
#endif
#ifdef NRF24SPIB0
    return UCB0RXBUF;
#endif
}

void NRF24::selectON() {
    saveSMCLK = BCSCTL2;
    BCSCTL2 &= 0xF1;  // désactivation du potentiel diviseur de SMCLK et utilisation de DCO
#ifdef NRF24SPIA0
    saveUCCTL1 = UCA0CTL1;
    UCA0CTL1 |= 0xC0;
    savePrescal = UCA0BR0 | (UCA0BR1 << 8);
    UCA0BR0 = 2;
    UCA0BR1 = 0;
#endif
#ifdef NRF24SPIB0
    saveUCCTL1 = UCB0CTL1;
    UCB0CTL1 |= 0xC0;
    savePrescal = UCB0BR0 | (UCB0BR1 << 8);
    UCB0BR0 = 2;
    UCB0BR1 = 0;
#endif

    #if defined(NRF24SELP1)
        P1OUT &= ~NRF24SELBIT;
    #elif defined(NRF24SELP2)
        P2OUT &= ~NRF24SELBIT;
    #elif defined(NRF24SELP3)
        P3OUT &= ~NRF24SELBIT;
    #endif
}

void NRF24::selectOFF() {
// reload save vars
    while (UCB0STAT & 0x01); // tant que le SPI fonctionne, on attend
    unsigned char v = NRF24::get(); // purge la dernière lecture valide
    #if defined(NRF24SELP1)
        P1OUT |= NRF24SELBIT;
    #elif defined(NRF24SELP2)
        P2OUT |= NRF24SELBIT;
    #elif defined(NRF24SELP3)
        P3OUT |= NRF24SELBIT;
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

// TODO : faire fonction Active/Desactive NRF et mettre CEON et CEOFF en inline
void NRF24::CEON() {
#if defined(NRF24CEP1)
    P1OUT |= NRF24CEBIT;
#elif defined(NRF24CEP2)
    P2OUT |= NRF24CEBIT;
#elif defined(NRF24CEP3)
    P3OUT |= NRF24CEBIT;
#endif
}

void NRF24::CEOFF() {
#if defined(NRF24CEP1)
    P1OUT &= ~NRF24CEBIT;
#elif defined(NRF24CEP2)
    P2OUT &= ~NRF24CEBIT;
#elif defined(NRF24CEP3)
    P3OUT &= ~NRF24CEBIT;
#endif
}
