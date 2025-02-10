/*
 * libMaths.cpp
 *
 *  Created on: 12 janv. 2025
 *      Author: david
 */

#include <libMaths.hpp>

// division de 2 entiers non signés
// si division par zéro : reste 0 et quotient = Dividende
// --opt_level = 3
// nombre de cycles maximum : 8 + 2 + 1 + 16 * ( 2 + 3 + (2) + 3 ) + 11 = 182 cycles
// à 16MHz => 11,375µs
// minimum : 182 - 16 * 2 = 150
// à 16MHz => 9,375µs
mDIVrRET mDivR(unsigned int Dividende, unsigned int Diviseur) { // appel + retour : 5 + 3
    typedef union {
        mDIVrRET Op;
        unsigned long iOp;
    } mDIVrVAR;

    unsigned int nb = 16;                   // Initialisation du registre : 2 cycles
    mDIVrVAR Op;
    Op.Op.quotient = Dividende;             // Aucun impact en ASM
    Op.Op.reste = 0;                        // Initialisation du registre : 1 cycles

    do {
        Op.iOp <<= 1;                       // Décalage à gauche sur les 2 registres : 2 cycles
        if (Op.Op.reste >= Diviseur) {      // Comparaison de deux registres et saut : 3 cycles
            Op.Op.quotient++;               // Incrément du registre de 1 : 1 cycle
            Op.Op.reste -= Diviseur;        // Soustraction de deux registres : 1 cycle
        }
    } while (--nb > 0);                     // Soustraction et saut : 3 cycles

    return Op.Op;                           // Test de l'adresse de la variable + push en sortie du resultat : 11 cycles
}

// multiplication de deux entiers positifs
unsigned long mMult(unsigned int a, unsigned int b) {
    unsigned long r = 0;
    unsigned long c = (unsigned long)a;

    // si une des valeurs vaut 0, alors résultat nul
    if ((b == 0) || (a == 0)) return 0;

    do {
        if (b & 1) {
            r += c;
        }
        c <<= 1;
        b >>= 1;
    } while (b != 0);

    return r;
}
