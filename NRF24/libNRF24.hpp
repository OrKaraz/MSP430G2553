/*
 * libNRF24.hpp
 *
 *  Created on: 24 févr. 2025
 *      Author: david
 */

#ifndef INCLUDE_LIBNRF24_HPP_
#define INCLUDE_LIBNRF24_HPP_

#define NRF24SPIB0       // NRF24SPIxx : SPI utilisé (B0, A0, B1, A1, ...)
#define NRF24selP2       // NRF24selPx : x étant le port (1, 2, 3) utilisé pour le CS
#define NRF24selBIT BIT2 // NRF24selBIT : position de la broche
//#define NRF24CEP2        // utilisation de la broche CE du module sur le port Px
//#define NRF24CEBIT BIT1  // CE connecté à la broche numéro x
//#define NRF24rstP2       // NRF24rstPx : x étant le port (1, 2, 3) utilisé pour le RESET
//#define NRF24rstBIT BIT2 // NRF24rstBIT : position de la broche

#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define REUSE_TX_PL 0xE3
#define R_RX_PL_WID 0x60
#define W_ACK_PAYLOAD 0xA8         // ajouter 0 à 5 le numéro du pipe utilisé
#define W_TX_PAYLOAD_NO_ACK 0xB0
#define NOP 0xFF

// Liste des registres :
#define CONFIG 0x00
    #define MASK_RX_DR 0x40
    #define MASK_TX_DS 0x20
    #define MASK_MAX_TR 0x10
    #define EN_CRC 0x08
    #define CRCO 0x04
    #define PWR_UP 0x02
    #define PRIM_RX 0x01
#define EN_AA 0x01
    #define ENAA_P5 0x20
    #define ENAA_P4 0x10
    #define ENAA_P3 0x08
    #define ENAA_P2 0x04
    #define ENAA_P1 0x02
    #define ENAA_P0 0x01
#define EN_RXADDR 0x02
    #define ERX_P5 0x20
    #define ERX_P4 0x10
    #define ERX_P3 0x08
    #define ERX_P2 0x04
    #define ERX_P1 0x02
    #define ERX_P0 0x01
#define SETUP_AW 0x03
    #define AW_5 0x03       // 5 octets d'adresse
    #define AW_4 0x02       // 4 octets d'adresse
    #define AW_3 0x01       // 3 octets d'adresse
#define SETUP_RETR 0x04
    #define ARD_250 0x00    // 250 en microsecondes
    #define ARD_500 0x10
    #define ARD_750 0x20
    #define ARD_1000 0x30
    #define ARD_1250 0x40
    #define ARD_1500 0x50
    #define ARD_1750 0x60
    #define ARD_2000 0x70
    #define ARD_2250 0x80
    #define ARD_2500 0x90
    #define ARD_2750 0xA0
    #define ARD_3000 0xB0
    #define ARD_3250 0xC0
    #define ARD_3500 0xD0
    #define ARD_3750 0xE0
    #define ARD_4000 0xF0
    #define ARC_0 0x00      // nombre de tentative
    #define ARC_1 0x01
    #define ARC_2 0x02
    #define ARC_3 0x03
    #define ARC_4 0x04
    #define ARC_5 0x05
    #define ARC_6 0x06
    #define ARC_7 0x07
    #define ARC_8 0x08
    #define ARC_9 0x09
    #define ARC_10 0x0A
    #define ARC_11 0x0B
    #define ARC_12 0x0C
    #define ARC_13 0x0D
    #define ARC_14 0x0E
    #define ARC_15 0x0F
#define RF_CH 0x05
    // envoyer en paramètre le numéro du canal souhaité (0 à 127)
#define RF_SETUP 0x06
    #define CONT_WAVE 0x80
    #define RF_DR_250k 0x20
    #define RF_DR_1M 0x00
    #define RF_DR_2M 0x08
    #define RF_PWR_0 0x06
    #define RF_PWR_6 0x04
    #define RF_PWR_12 0x02
    #define RF_PWR_18 0x00
#define STATUS 0x07       // techniquement, envoyer un NOP suffit
    #define RX_DR 0x40
    #define TX_DS 0x20
    #define MAX_RT 0x10
    #define RX_P_NO_0 0x00
    #define RX_P_NO_1 0x02
    #define RX_P_NO_2 0x04
    #define RX_P_NO_3 0x06
    #define RX_P_NO_4 0x08
    #define RX_P_NO_5 0x0A
    #define RX_P_NO_Empty 0x0E
    #define TX_Full 0x01
#define OBSERVE_TX 0x08
#define RPD 0x09
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17
    #define TX_REUSE 0x40
    #define TX_FULL 0x20
    #define TX_EMPTY 0x10
    #define RX_FULL 0x02
    #define RX_EMPTY 0x01
#define DYNPD 0x1C
    #define DPL_5 0x20
    #define DPL_4 0x10
    #define DPL_3 0x08
    #define DPL_2 0x04
    #define DPL_1 0x02
    #define DPL_0 0x01
#define FEATURE 0x1D
    #define EN_DPL 0x04
    #define EN_ACK_PAY 0x02
    #define EN_DYN_ACK 0x01

#include <msp430.h>
#include <libWatchdog.hpp>

namespace NRF24 {
    void init();
    void sendArray(unsigned char nb, unsigned char* l);

    void selectON();
    void selectOFF();

    inline void send(unsigned char s);
#if defined(NRF24CEP1) || defined(NRF24CEP2) || defined(NRF24CEP3)
    inline void veille();
    inline void reveil();
#endif
#if defined(NRF24rstP1) || defined(NRF24rstP2) || defined(NRF24rstP3)
    inline void RESETON();
    inline void RESETOFF();
#endif

    const unsigned char tInit[] = {
        2,   // nombre de d'octet + 1
        W_REGISTER + FEATURE, EN_DPL + EN_ACK_PAY,
        W_REGISTER + SETUP_RETR, ARD_500 + ARC_3,


        W_REGISTER + CONFIG, MASK_MAX_TR + MASK_TX_DS + PWR_UP
    };
};

#endif /* INCLUDE_LIBNRF24_HPP_ */
