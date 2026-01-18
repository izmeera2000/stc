/*----------------------------------------------------------------------------*/
#ifndef __MY1MCU51_H__
#define __MY1MCU51_H__
/*----------------------------------------------------------------------------*/
/**
 * my1mcu51.h
 * - by azman@my1matrix.org
 * - alternative header (e.g. instead of using reg51.h with Keil compiler)
**/
/*----------------------------------------------------------------------------*/
#include "my1defs.h"
#include "my1macros.h"
#include "my1types.h"
/*----------------------------------------------------------------------------*/
/* commonly used oscillator frequency */
#ifndef MCU51_FOSC
#define MCU51_FOSC 11059200L
#endif
#define MCU51_TCLK ((float)1/(MCU51_FOSC/12))
/* 1ms/1.085us? */
#ifndef MCU51_COUNT_1MS
#define MCU51_COUNT_1MS (long)(((float)1/1000)/MCU51_TCLK)
#endif
/*----------------------------------------------------------------------------*/
/** SPECIAL FUNCTION REGISTER DECLARATIONS */
my1sfr(P0  ,0x80);
my1sfr(SP  ,0x81);
my1sfr(DPL ,0x82);
my1sfr(DPH ,0x83);
my1sfr(PCON,0x87);
my1sfr(TCON,0x88);
my1sfr(TMOD,0x89);
my1sfr(TL0 ,0x8A);
my1sfr(TL1 ,0x8B);
my1sfr(TH0 ,0x8C);
my1sfr(TH1 ,0x8D);
my1sfr(P1  ,0x90);
my1sfr(SCON,0x98);
my1sfr(SBUF,0x99);
my1sfr(P2  ,0xA0);
my1sfr(IE  ,0xA8);
my1sfr(P3  ,0xB0);
my1sfr(IP  ,0xB8);
my1sfr(PSW ,0xD0);
my1sfr(ACC ,0xE0);
my1sfr(B   ,0xF0);
/*----------------------------------------------------------------------------*/
/** SFR BIT DECLARATIONS (BIT-ADDRESSABLE SFR) */
/* P0 */
my1sbit(P0_0,0x80);
my1sbit(P0_1,0x81);
my1sbit(P0_2,0x82);
my1sbit(P0_3,0x83);
my1sbit(P0_4,0x84);
my1sbit(P0_5,0x85);
my1sbit(P0_6,0x86);
my1sbit(P0_7,0x87);
/* TCON */
my1sbit(IT0 ,0x88);
my1sbit(IE0 ,0x89);
my1sbit(IT1 ,0x8A);
my1sbit(IE1 ,0x8B);
my1sbit(TR0 ,0x8C);
my1sbit(TF0 ,0x8D);
my1sbit(TR1 ,0x8E);
my1sbit(TF1 ,0x8F);
/* P1 */
my1sbit(P1_0,0x90);
my1sbit(P1_1,0x91);
my1sbit(P1_2,0x92);
my1sbit(P1_3,0x93);
my1sbit(P1_4,0x94);
my1sbit(P1_5,0x95);
my1sbit(P1_6,0x96);
my1sbit(P1_7,0x97);
/* SCON */
my1sbit(RI  ,0x98);
my1sbit(TI  ,0x99);
my1sbit(RB8 ,0x9A);
my1sbit(TB8 ,0x9B);
my1sbit(REN ,0x9C);
my1sbit(SM2 ,0x9D);
my1sbit(SM1 ,0x9E);
my1sbit(SM0 ,0x9F);
/* P2 */
my1sbit(P2_0,0xA0);
my1sbit(P2_1,0xA1);
my1sbit(P2_2,0xA2);
my1sbit(P2_3,0xA3);
my1sbit(P2_4,0xA4);
my1sbit(P2_5,0xA5);
my1sbit(P2_6,0xA6);
my1sbit(P2_7,0xA7);
/* IE */
my1sbit(EX0 ,0xA8);
my1sbit(ET0 ,0xA9);
my1sbit(EX1 ,0xAA);
my1sbit(ET1 ,0xAB);
my1sbit(ES  ,0xAC);
my1sbit(EA  ,0xAF);
/* P3 */
my1sbit(P3_0,0xB0);
my1sbit(P3_1,0xB1);
my1sbit(P3_2,0xB2);
my1sbit(P3_3,0xB3);
my1sbit(P3_4,0xB4);
my1sbit(P3_5,0xB5);
my1sbit(P3_6,0xB6);
my1sbit(P3_7,0xB7);
/* P3 - DUAL PURPOSE */
my1sbit(RXD ,0xB0);
my1sbit(TXD ,0xB1);
my1sbit(INT0,0xB2);
my1sbit(INT1,0xB3);
my1sbit(T0  ,0xB4);
my1sbit(T1  ,0xB5);
my1sbit(WR  ,0xB6);
my1sbit(RD  ,0xB7);
/* IP */
my1sbit(PX0 ,0xB8);
my1sbit(PT0 ,0xB9);
my1sbit(PX1 ,0xBA);
my1sbit(PT1 ,0xBB);
my1sbit(PS  ,0xBC);
/* PSW */
my1sbit(P   ,0xD0);
my1sbit(UFLG,0xD1); /* not used? labeled as 'user definable' in user's manual */
my1sbit(OV  ,0xD2);
my1sbit(RS0 ,0xD3);
my1sbit(RS1 ,0xD4);
my1sbit(F0  ,0xD5);
my1sbit(AC  ,0xD6);
my1sbit(CY  ,0xD7);
/*----------------------------------------------------------------------------*/
/** NON-{bit addressable} registers */
/* PCON: 0x87 */
#define PCON_IDLE 0x01
#define PCON_PDWN 0x02
#define PCON_GF0  0x04
#define PCON_GF1  0x08
#define PCON_SMOD 0x80
/*----------------------------------------------------------------------------*/
/** useful macro for power management */
#define mcu51_idle() PCON = PCON | PCON_IDLE
#define mcu51_sleep() PCON = PCON | PCON_PDWN
/*----------------------------------------------------------------------------*/
/** INTERRUPT VECTORS - 0x03, 0x0b, 0x13, 0x1b, 0x23 */
#define IE0_VECTOR 0
#define TF0_VECTOR 1
#define IE1_VECTOR 2
#define TF1_VECTOR 3
#define SI0_VECTOR 4
/*----------------------------------------------------------------------------*/
#endif /* __MY1MCU51_H__ */
/*----------------------------------------------------------------------------*/
