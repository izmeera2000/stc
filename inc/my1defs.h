/*----------------------------------------------------------------------------*/
#ifndef __MY1DEFS_H__
#define __MY1DEFS_H__
/*----------------------------------------------------------------------------*/
/**
 * my1defs.h
 * - by azman@my1matrix.org
 * - simple definitions to allow the same code to be compiled on
 *   both Keil C51 compiler (commercial) and SDCC (open source)
**/
/*----------------------------------------------------------------------------*/
/**
 * this (__C51__) is defined for Keil
 * - from: https://sourceforge.net/p/predef/wiki/Compilers/
**/
#ifdef __C51__
#define __USE_KEIL__
#endif
/*----------------------------------------------------------------------------*/
/* Macro Definitions - works on sdcc-3.0.0 & keil uvision4 c51 9.02 */
/* - by default, we use SDCC :p */
#ifdef __USE_KEIL__
#define my1sfr(NAME,ADDR) sfr NAME = ADDR
#define my1sbit(NAME,ADDR) sbit NAME = ADDR
#define my1intr(inum) interrupt inum
#include <intrins.h>
#define NOP _nop_ ()
/* write these in sdcc syntax - convert to keil syntax when requested */
#define __interrupt(inum) interrupt inum
#define __bit bit
#define __xdata xdata
#define __code code
#else
#define my1sfr(NAME,ADDR) __sfr __at (ADDR) NAME
#define my1sbit(NAME,ADDR) __sbit __at (ADDR) NAME
#define my1intr(inum) __interrupt(inum)
#define NOP __asm__ ("nop")
#endif
/**
 * sdcc no longer accept __interrupt() definition without parenthesis...
# for a in $(find . -type f -name "*.c" -o -name "*.h") ; do \
b=$(sed -n 's/__interrupt(\([^) ]*\)/__interrupt(\1)/g p' $a) ; \
[ -z "$b" ] && continue ; echo -e "$a:\n => $b" ; done
# for a in $(find . -type f -name "*.c" -o -name "*.h") ; do \
sed -i 's/__interrupt \([^ ]*\)/__interrupt(\1)/g' $a ; done
**/
/*----------------------------------------------------------------------------*/
/* a more generic name */
#define _sfr(a,b) my1sfr(a,b)
#define _sbit(a,b) my1sbit(a,b)
#define _intr(a) my1intr(a)
/*----------------------------------------------------------------------------*/
/* useful addresses for user code */
#define PORT0 0x80
#define PORT1 0x90
#define PORT2 0xA0
#define PORT3 0xB0
#define PIN00 0x80
#define PIN01 0x81
#define PIN02 0x82
#define PIN03 0x83
#define PIN04 0x84
#define PIN05 0x85
#define PIN06 0x86
#define PIN07 0x87
#define PIN10 0x90
#define PIN11 0x91
#define PIN12 0x92
#define PIN13 0x93
#define PIN14 0x94
#define PIN15 0x95
#define PIN16 0x96
#define PIN17 0x97
#define PIN20 0xA0
#define PIN21 0xA1
#define PIN22 0xA2
#define PIN23 0xA3
#define PIN24 0xA4
#define PIN25 0xA5
#define PIN26 0xA6
#define PIN27 0xA7
#define PIN30 0xB0
#define PIN31 0xB1
#define PIN32 0xB2
#define PIN33 0xB3
#define PIN34 0xB4
#define PIN35 0xB5
#define PIN36 0xB6
#define PIN37 0xB7
/*----------------------------------------------------------------------------*/
#endif /** __MY1DEFS_H__ */
/*----------------------------------------------------------------------------*/
