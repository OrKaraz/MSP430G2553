/*
 * libILI9341.hpp
 *
 *  Created on: 12 janv. 2025
 *      Author: david
 */

#ifndef INCLUDE_LIBILI9341_HPP_
#define INCLUDE_LIBILI9341_HPP_

#define ili9341SPIB0       // SPIA0 ou SPIB0
#define ili9341rstP2       // rstP1, rstP2, rstP3
#define ili9341RESET BIT5  // informer du bit connecté
#define ili9341comP2       // comP1, comP2, comP3  (port commande ou data)
#define ili9341COMP BIT1   // bit du port com
#define ili9341selP2       // port du SELECT
#define ili9341SELECT BIT0 // bit CS

#include <msp430.h>
#include <libWatchdog.hpp>
#include <libMaths.hpp>

namespace ILI9341 {
    void init();
    void addr(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

    void fillrect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int c = 0);
    void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int c = 0xFFFF);
    void drawInt(unsigned int p, unsigned int x, unsigned int y, unsigned int f = 0xFFFF, unsigned int b = 0, unsigned char a = 0, unsigned char size = 0);
                                                                 // s = 1 : aligné à droite
    void drawChar(unsigned char p, unsigned int x, unsigned int y, unsigned int f = 0xFFFF, unsigned int b = 0);
    void drawBigChar(unsigned char p, unsigned int x, unsigned int y, unsigned int f = 0xFFFF, unsigned int b = 0);
    void drawTxt(unsigned char *p, unsigned int x, unsigned int y, unsigned int f = 0xFFFF, unsigned int b = 0, unsigned char size = 0);

    unsigned int rvb(unsigned char r, unsigned char v, unsigned char b);

    void sendX(unsigned char val, unsigned int x);

    void selectON(); // selectionner l'écran
    void selectOFF();

    inline void send(unsigned char s);
    inline void send16(unsigned int s);

    inline void rstON();    // activation du reset
    inline void rstOFF();
    inline void cmdON();    // mode commande
    inline void cmdOFF();

    const unsigned char tInitScreen[101] = {
            19,   // nombre de ligne de commande
            0xCB, 6, 0x39, 0x2C, 0x00, 0x34, 0x02,
            0xCF, 4, 0x00, 0XC1, 0X30,
            0xE8, 4, 0x85, 0x00, 0x78,
            0xEA, 3, 0x00, 0x00,
            0xED, 5, 0x64, 0x03, 0X12, 0X81,
            0xF7, 2, 0x20,
            0xC0, 2, 0x23, // Power control  23 VrH[5:0]
            0xC1, 2, 0x10, // Power control  SAP[2:0];BT[3:0]
            0xC5, 3, 0x3E, 0x28, // VCM control
            0xC7, 2, 0x86, // VCM control2
            0x36, 2, 0xE8, // Memory Access Control  (28 -> mode paysage, 48 -> mode portrait, E8 -> paysage inverse (carte SD au dessus))
            0x3A, 2, 0x55, // 66 -> 3 bytes/pixel ; 55 -> 2 bytes/pixel
            0xB1, 3, 0x00, 0x10, // 0x00, 0x1F (frequence d'affichage, 0x10 est le plus rapide environ 120Hz)
            0xB6, 4, 0x08, 0x82, 0x27, // Display Function Control
            0xF2, 2, 0x00,
            0x26, 2, 0x01,
            0xE0, 16, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07 ,0x10 ,0x03 ,0x0E ,0x09 ,0x00,
            0xE1, 16, 0x00 ,0x0E ,0x14 ,0x03 ,0x11 ,0x07 ,0x31 ,0xC1 ,0x48 ,0x08 ,0x0F ,0x0C ,0x31 ,0x36 ,0x0F,
            0x11, 1 // Exit Sleep
    };
    const unsigned char tChar[1536] = {
// 000
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      // blank
            0x00, 0x01, 0x02, 0x01, 0x3F, 0x7F, 0x80, 0x7F, 0x80, 0x80, 0x7F, 0x00,      // char gauche
            0x00, 0xE0, 0x1E, 0xE0, 0xF8, 0xFC, 0x02, 0xFC, 0x02, 0x04, 0xF8, 0x00,      // char droite
            0x07, 0x08, 0x13, 0x04, 0x01, 0x01, 0x1F, 0x10, 0x14, 0x10, 0x1F, 0x00,      // telecommande gauche
            0xC0, 0x20, 0x90, 0x40, 0x00, 0x00, 0xF0, 0x10, 0x50, 0x10, 0xF0, 0x00,      // telecommande droite
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
// 001
            0x00, 0x00, 0x00, 0x30, 0x38, 0x3C, 0x38, 0x30, 0x00, 0x00, 0x00, 0x00,      // fleche droite
            0x00, 0x00, 0x00, 0x0C, 0x1C, 0x3C, 0x1C, 0x0C, 0x00, 0x00, 0x00, 0x00,      // fleche gauche
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,      //
// 002
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // space
            0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00,      // !
            0x36, 0x36, 0x36, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // "
            0x00, 0x6C, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, 0x00,      // #
            0x18, 0x18, 0x7C, 0xC6, 0xC0, 0x78, 0x3C, 0x06, 0xC6, 0x7C, 0x18, 0x18,      // $
            0x00, 0x00, 0x00, 0x62, 0x66, 0x0C, 0x18, 0x30, 0x66, 0xC6, 0x00, 0x00,      // %
            0x00, 0x38, 0x6C, 0x38, 0x38, 0x76, 0xF6, 0xCE, 0xCC, 0x76, 0x00, 0x00,      // &
            0x0C, 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // '
            0x00, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00, 0x00,      // (
            0x00, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00, 0x00,      // )
            0x00, 0x00, 0x00, 0x6C, 0x38, 0xFE, 0x38, 0x6C, 0x00, 0x00, 0x00, 0x00,      // *
            0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,      // +
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x18, 0x00,      // ,
            0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // -
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,      // .
            0x00, 0x00, 0x02, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00, 0x00,      // /
// 003
            0x00, 0x7C, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0xC6, 0x7C, 0x00, 0x00,      // 0
            0x00, 0x18, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00,      // 1
            0x00, 0x7C, 0xC6, 0xC6, 0x0C, 0x18, 0x30, 0x60, 0xC6, 0xFE, 0x00, 0x00,      // 2
            0x00, 0x7C, 0xC6, 0x06, 0x06, 0x3C, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00,      // 3
            0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x0C, 0x0C, 0x00, 0x00,      // 4
            0x00, 0xFE, 0xC0, 0xC0, 0xC0, 0xFC, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00,      // 5
            0x00, 0x7C, 0xC6, 0xC0, 0xC0, 0xFC, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00,      // 6
            0x00, 0xFE, 0xC6, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00,      // 7
            0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00,      // 8
            0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00,      // 9
            0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x00,      // :
            0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x18, 0x00,      // ;
            0x00, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x00, 0x00,      // <
            0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,      // =
            0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00,      // >
            0x00, 0x7C, 0xC6, 0xC6, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00,      // ?
// 004
            0x00, 0x7C, 0xC6, 0xC6, 0xDE, 0xDE, 0xDE, 0xDC, 0xC0, 0x7E, 0x00, 0x00,      // @
            0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0x00, 0x00,      // A
            0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00,      // B
            0x00, 0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00, 0x00,      // C
            0x00, 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 0x00,      // D
            0x00, 0xFE, 0x66, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x66, 0xFE, 0x00, 0x00,      // E
            0x00, 0xFE, 0x66, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00,      // F
            0x00, 0x7C, 0xC6, 0xC6, 0xC0, 0xC0, 0xCE, 0xC6, 0xC6, 0x7C, 0x00, 0x00,      // G
            0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00,      // H
            0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00,      // I
            0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70, 0x00, 0x00,      // J
            0x00, 0xC6, 0xCC, 0xD8, 0xF0, 0xF0, 0xD8, 0xCC, 0xC6, 0xC6, 0x00, 0x00,      // K
            0x00, 0xF0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00,      // L
            0x00, 0xC6, 0xC6, 0xEE, 0xFE, 0xD6, 0xD6, 0xD6, 0xC6, 0xC6, 0x00, 0x00,      // M
            0x00, 0xC6, 0xC6, 0xE6, 0xE6, 0xF6, 0xDE, 0xCE, 0xCE, 0xC6, 0x00, 0x00,      // N
            0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00,      // O
// 005
            0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00,      // P
            0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x06, 0x00,      // Q
            0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00,      // R
            0x00, 0x7C, 0xC6, 0xC0, 0x60, 0x38, 0x0C, 0x06, 0xC6, 0x7C, 0x00, 0x00,      // S
            0x00, 0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00,      // T
            0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00,      // U
            0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x10, 0x00, 0x00,      // V
            0x00, 0xC6, 0xC6, 0xD6, 0xD6, 0xD6, 0xFE, 0xEE, 0xC6, 0xC6, 0x00, 0x00,      // W
            0x00, 0xC6, 0xC6, 0x6C, 0x38, 0x38, 0x38, 0x6C, 0xC6, 0xC6, 0x00, 0x00,      // X
            0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00,      // Y
            0x00, 0xFE, 0xC6, 0x8C, 0x18, 0x30, 0x60, 0xC2, 0xC6, 0xFE, 0x00, 0x00,      // Z
            0x00, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7C, 0x00, 0x00,      // [
            0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00, 0x00,      // \       .
            0x00, 0x7C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x7C, 0x00, 0x00,      // ]
            0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // ^
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,      // _
// 006
            0x18, 0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // `
            0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xDC, 0x76, 0x00, 0x00,      // a
            0x00, 0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00,      // b
            0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00,      // c
            0x00, 0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0xCC, 0x7E, 0x00, 0x00,      // d
            0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC6, 0x7C, 0x00, 0x00,      // e
            0x00, 0x1C, 0x36, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00,      // f
            0x00, 0x00, 0x00, 0x00, 0x76, 0xCE, 0xC6, 0xC6, 0x7E, 0x06, 0xC6, 0x7C,      // g
            0x00, 0xE0, 0x60, 0x60, 0x6C, 0x76, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00,      // h
            0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00,      // i
            0x00, 0x0C, 0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78,      // j
            0x00, 0xE0, 0x60, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00,      // k
            0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00,      // l
            0x00, 0x00, 0x00, 0x00, 0x6C, 0xFE, 0xD6, 0xD6, 0xC6, 0xC6, 0x00, 0x00,      // m
            0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00,      // n
            0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00,      // o
// 007
            0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0,      // p
            0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0x0C, 0x1E,      // q
            0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00,      // r
            0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0x70, 0x1C, 0xC6, 0x7C, 0x00, 0x00,      // s
            0x00, 0x30, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00, 0x00,      // t
            0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00,      // u
            0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x10, 0x00, 0x00,      // v
            0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xD6, 0xFE, 0x6C, 0x00, 0x00,      // w
            0x00, 0x00, 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0x00,      // x
            0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xCE, 0x76, 0x06, 0xC6, 0x7C,      // y
            0x00, 0x00, 0x00, 0x00, 0xFE, 0x8C, 0x18, 0x30, 0x62, 0xFE, 0x00, 0x00,      // z
            0x00, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x00, 0x00,      // {
            0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00,      // |
            0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00,      // }
            0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // ~
            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81       //
    };
};

#endif /* INCLUDE_LIBILI9341_HPP_ */
