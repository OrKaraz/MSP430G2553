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

// TODO : ADDR 2 -> 5 seulement 1 oct
void NRF24::setADDR(unsigned char *add, unsigned char p) {
    if (p > 6) p = 6; // 6 = TXADDR
    NRF24::selectON();
    NRF24::send(W_REGISTER | RX_ADDR_P0 + p);
    for (int a = 4; a >= 0; a--) {
        __delay_cycles(7);
        NRF24::send(add[4-a]);
    }
    NRF24::selectOFF();
}

// TODO : corriger le fait de devoir utiliser un tableau de 6 valeurs
void NRF24::getADDR(unsigned char *add, unsigned char p) {
    int a = 5;
    if (p > 6) p = 6;
    NRF24::selectON();
    NRF24::send(RX_ADDR_P0 + p);
    while (1) {
        if (IFG2 & UCB0RXIFG) {
            add[5-a] = NRF24::get();
            a--;
            if (a < 0) break;
        }
        if (IFG2 & UCB0TXIFG) NRF24::send(0); // si UCB0TXBUF est vide
    }
    NRF24::selectOFF();
}

unsigned char NRF24::setRegister(unsigned char r, unsigned char v) {
    unsigned char ret;
    r = r & 0x1F;
    NRF24::selectON();
    NRF24::send(W_REGISTER | r);
    NRF24::send(v);
    while (1) {
        if (IFG2 & UCB0RXIFG) {
            ret = NRF24::get();
            break;
        }
    }
    NRF24::selectOFF();
    return ret;
}

unsigned char NRF24::getRegister(unsigned char r, unsigned char* v) {
    unsigned char ret = 0;
    r = r & 0x1F;
    NRF24::selectON();
    NRF24::send(r);
    NRF24::send(0);
    while (1) {
        if (IFG2 & UCB0RXIFG) {
            if (!ret) ret = NRF24::get();
            else {
                *v = NRF24::get();
                break;
            }
        }
    }
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
