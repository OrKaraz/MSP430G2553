/*
 * libILI9341.cpp
 *
 *  Created on: 12 janv. 2025
 *      Author: david
 */

#include <libILI9341.hpp>

void ILI9341::init() {
    unsigned int pos;
    unsigned char *tableInit = (unsigned char *)tInitScreen;
    unsigned int j;

    WDT::init();
    WDT::flag = 0;
    do {
        LPM0;           // pause de 5ms mini
    } while (WDT::flag < 3);

    ILI9341::rstON();

    WDT::init();
    WDT::flag = 0;
    do {
        LPM0;           // pause de 15ms mini
    } while (WDT::flag < 8);

    ILI9341::rstOFF();

    WDT::init();
    WDT::flag = 0;
    do {
        LPM0;           // pause de 15ms mini
    } while (WDT::flag < 8);

    ILI9341::selectON();
    pos = *tableInit++;
    do {
        __delay_cycles(1);
        ILI9341::cmdON();
        ILI9341::send(*tableInit++);
        j = *tableInit++;
        while (--j > 0) {
            ILI9341::cmdOFF();
            ILI9341::send(*tableInit++);
        }
    } while (--pos != 0);

    WDT::init();
    WDT::flag = 0;
    do {
        LPM0;
    } while (WDT::flag < 62); // pause de 120ms

    ILI9341::cmdON();
    ILI9341::send(0x29);
    WDT::init();
    WDT::flag = 0;
    do {
        LPM0;           // pause de 15ms mini
    } while (WDT::flag < 8);

    ILI9341::selectOFF();
}

// sur l'écran complet : 77ms à 16MHz
void ILI9341::fillrect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int c) {
    unsigned int y;
    ILI9341::selectON();
    ILI9341::addr(x1, y1, x2, y2);
    if (x2 < x1) {
        unsigned int t = x2;
        x2 = x1;
        x1 = t;
    }
    if (y2 < y1) {
        unsigned int t = y2;
        y2 = y1;
        y1 = t;
    }
    x2 -= x1 - 1;
    y2 -= y1 - 1;
    y = y2;
    ILI9341::cmdOFF();
    do {
        do {
            ILI9341::send16(c);
            __delay_cycles(5);
        } while (--y != 0);
        y = y2;
    } while (--x2 > 0);
    ILI9341::selectOFF();
}

void ILI9341::drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int c) {
    ILI9341::selectON();
    if (y1 == y2) {
        ILI9341::addr(x1, y1, x2, y2);
        if (x2 < x1) {
            unsigned int t = x2;
            x2 = x1;
            x1 = t;
        }
        x2 -= x1;
        x2 += 2;
        do {
            ILI9341::send16(c);
        } while (--x2 != 0);
    } else if (x1 == x2) {
        ILI9341::addr(x1, y1, x2, y2);
        if (y2 < y1) {
            unsigned int t = y2;
            y2 = y1;
            y1 = t;
        }
        y2 -= y1;
        y2 += 2;
        do {
            ILI9341::send16(c);
        } while (--y2 != 0);
    } else {
        int xinc, yinc, dx, dy, cumul;
        dx = (int)x2 - (int)x1;
        xinc = ( dx > 0 ) ? 1 : -1;
        if (dx < 0) dx = -dx;
        dy = (int)y2 - (int)y1;
        yinc = ( dy > 0 ) ? 1 : -1;
        if (dy < 0) dy = -dy;
        ILI9341::addr(x1, y1, 319, 239);
        ILI9341::send16(c);
        if (dx > dy) {
            cumul = dx >> 2;
            do {
                x1 += xinc;
                ILI9341::sendX(0x2A, x1);
                cumul += dy;
                if (cumul > dx) {
                    cumul -= dx;
                    y1 += yinc;
                    ILI9341::sendX(0x2B, y1);
                }
                ILI9341::sendX(0x2C, c);
            } while (x1 != x2);
        } else {
            cumul = dy >> 2;
            do {
                y1 += yinc;
                ILI9341::sendX(0x2B, y1);
                cumul += dx;
                if (cumul > dy) {
                    cumul -= dy;
                    x1 += xinc;
                    ILI9341::sendX(0x2A, x1);
                }
                ILI9341::sendX(0x2C, c);
            } while (y1 != y2);
        }
    }
    ILI9341::selectOFF();
}

void ILI9341::drawInt(unsigned int p, unsigned int x, unsigned int y, unsigned int f, unsigned int b, unsigned char s) {
    // s = 0 : alignement à gauche
    // s = 1 : alignement à droite
    unsigned char iliste[5];
    unsigned int i = 5;
    mDIVrRET v;
    v.quotient = p;
    if (s) {
        do {
            v = mDivR(v.quotient, 10);
            ILI9341::drawChar(v.reste + 0x30, x, y, f, b);
            x -= 8;
        } while (v.quotient != 0);
    } else {
        i = 0;
        do {
            v = mDivR(v.quotient, 10);
            iliste[i] = v.reste + 0x30;
            i++;
        } while (v.quotient != 0);
        do {
            ILI9341::drawChar(iliste[--i], x, y, f, b);
            x += 8;
        } while (i != 0);
    }
}

void ILI9341::drawChar(unsigned char p, unsigned int x, unsigned int y, unsigned int f, unsigned int b) {
    unsigned char i, j, *pos = (unsigned char *)ILI9341::tChar;
    unsigned char val;

    ILI9341::selectON();
    ILI9341::addr(x, y, x+7, y+11);

    j = 12;
    pos += (unsigned int)p << 2;  //   p * 4
    pos += (unsigned int)p << 3;  //   p * 8
    do {
        val = *pos++;
        i = 8;
        do {
            if (val & 0x80) {
                ILI9341::send16(f);
            } else {
                ILI9341::send16(b);
            }
            val <<= 1;
        } while (--i != 0);
    } while (--j != 0);

    ILI9341::selectOFF();
}

void ILI9341::drawBigChar(unsigned char p, unsigned int x, unsigned int y, unsigned int f, unsigned int b) {
    unsigned char i, j, *pos = (unsigned char *)ILI9341::tChar;
    unsigned char val;

    ILI9341::selectON();
    ILI9341::addr(x, y, x+15, y+23);

    j = 12;
    pos += (unsigned int)p << 2;  //   p * 4
    pos += (unsigned int)p << 3;  //   p * 8
    do {
        int bis = 2;
        unsigned char valbis;
        val = *pos++;
        do {
            valbis = val;
            i = 8;
            do {
                if (valbis & 0x80) {
                    ILI9341::send16(f);
                    __delay_cycles(8);
                    ILI9341::send16(f);
                } else {
                    ILI9341::send16(b);
                    __delay_cycles(8);
                    ILI9341::send16(b);
                }
                valbis <<= 1;
            } while (--i != 0);
        } while (--bis != 0);
    } while (--j != 0);

    ILI9341::selectOFF();
}

void ILI9341::drawTxt(unsigned char *p, unsigned int x, unsigned int y, unsigned int f, unsigned int b, unsigned char size) {
    unsigned int i = 0;

    while (p[i] != 0) {
        if (size) {
            ILI9341::drawBigChar(p[i++], x, y, f, b);
            x += 8;
        }
        else ILI9341::drawChar(p[i++], x, y, f, b);
        x += 8;
    }
}

unsigned int ILI9341::rvb(unsigned char r, unsigned char v, unsigned char b) {
    unsigned int rr = r, vv = v, bb = b;
    rr <<= 8;
    rr = rr & 0xF800;
    bb >>= 3;
    vv <<= 3;
    vv = vv & 0x07E0;
    return rr | vv | bb;
}

void ILI9341::sendX(unsigned char val, unsigned int x) {
    ILI9341::cmdON();
    ILI9341::send(val);
    __delay_cycles(7);
    ILI9341::cmdOFF();
    ILI9341::send16(x);
}

void ILI9341::addr(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    ILI9341::sendX(0x2A, x1);
    __delay_cycles(5);
    ILI9341::send16(x2);
    __delay_cycles(5);
    ILI9341::sendX(0x2B, y1);
    __delay_cycles(5);
    ILI9341::send16(y2);
    __delay_cycles(10);
    ILI9341::cmdON();
    ILI9341::send(0x2C);
    __delay_cycles(7);
    ILI9341::cmdOFF();
}

inline void ILI9341::send(unsigned char s) {
#ifdef ili9341SPIA0
    UCA0TXBUF = s;
#endif
#ifdef ili9341SPIB0
    UCB0TXBUF = s;
#endif
}

inline void ILI9341::send16(unsigned int s) {
    ILI9341::send(s>>8);
    ILI9341::send(s);
}

inline void ILI9341::rstON() {
#ifdef ili9341rstP1
    P1OUT &= ~ili9341RESET;
#endif
#ifdef ili9341rstP2
    P2OUT &= ~ili9341RESET;
#endif
#ifdef ili9341rstP3
    P3OUT &= ~ili9341RESET;
#endif
}

inline void ILI9341::rstOFF() {
#ifdef ili9341rstP1
    P1OUT |= ili9341RESET;
#endif
#ifdef ili9341rstP2
    P2OUT |= ili9341RESET;
#endif
#ifdef ili9341rstP3
    P3OUT |= ili9341RESET;
#endif
}

inline void ILI9341::cmdON() {
#ifdef ili9341comP1
    P1OUT &= ~ili9341COMP;
#endif
#ifdef ili9341comP2
    P2OUT &= ~ili9341COMP;
#endif
#ifdef ili9341comP3
    P3OUT &= ~ili9341COMP;
#endif
}

inline void ILI9341::cmdOFF() {
#ifdef ili9341comP1
    P1OUT |= ili9341COMP;
#endif
#ifdef ili9341comP2
    P2OUT |= ili9341COMP;
#endif
#ifdef ili9341comP3
    P3OUT |= ili9341COMP;
#endif
}

inline void ILI9341::selectON() {
#ifdef ili9341selP1
    P1OUT &= ~ili9341SELECT;
#endif
#ifdef ili9341selP2
    P2OUT &= ~ili9341SELECT;
#endif
#ifdef ili9341selP3
    P3OUT &= ~ili9341SELECT;
#endif
}
inline void ILI9341::selectOFF() {
#ifdef ili9341selP1
    P1OUT |= ili9341SELECT;
#endif
#ifdef ili9341selP2
    P2OUT |= ili9341SELECT;
#endif
#ifdef ili9341selP3
    P3OUT |= ili9341SELECT;
#endif
}
