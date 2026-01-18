/*----------------------------------------------------------------------------*/
#ifndef __MY1STC51_H__
#define __MY1STC51_H__
/*----------------------------------------------------------------------------*/
/**
 * my1stc51.h
 * - by azman@my1matrix.org
 * - specific for stc devices:
 *   = stc12c5a60s2
 *   = stc15w204s
**/
/*----------------------------------------------------------------------------*/
#include "my1mcu51.h"
/*----------------------------------------------------------------------------*/
/* stc specific sfr */
my1sfr(P4  ,0xC0);
/** only P5.3-P5.0 actually! */
my1sfr(P5  ,0xC8);
/** i/o port modes */
my1sfr(P1M1,0x91);
my1sfr(P1M0,0x92);
my1sfr(P0M1,0x93);
my1sfr(P0M0,0x94);
my1sfr(P2M1,0x95);
my1sfr(P2M0,0x96);
my1sfr(P3M1,0xB1);
my1sfr(P3M0,0xB2);
my1sfr(P4M1,0xB3);
my1sfr(P4M0,0xB4);
my1sfr(P5M1,0xC9);
my1sfr(P5M0,0xCA);
/** second uart */
my1sfr(S2CON,0x9A);
my1sfr(S2BUF,0x9B);
my1sfr(BRT  ,0x9C);
my1sfr(AUXR ,0x8E);
my1sfr(IE2  ,0xAF);
my1sfr(IP2  ,0xB5);
my1sfr(IP2H ,0xB6);
my1sfr(AUXR1,0xA2);
/** pca/pwm related */
my1sfr(CCON,0xD8);
my1sfr(CMOD,0xD9);
my1sfr(CCAPM0,0xDA);
my1sfr(CCAPM1,0xDB);
my1sfr(CL,0xE9);
my1sfr(CCAP0L,0xEA);
my1sfr(CCAP1L,0xEB);
my1sfr(CH,0xF9);
my1sfr(CCAP0H,0xFA);
my1sfr(CCAP1H,0xFB);
my1sfr(PCAPWM0,0xF2);
my1sfr(PCAPWM1,0xF3);
/** timer2 on stc15w - also address for hidden RL_T2{H,L} */
my1sfr(TH2 ,0xD6);
my1sfr(TL2 ,0xD7);
/*----------------------------------------------------------------------------*/
/* stc specific sbit */
/* IE */
my1sbit(EADC,0xAD);
my1sbit(ELVD,0xAE);
/* IP */
my1sbit(PADC,0xBD);
my1sbit(PLVD,0xBE);
my1sbit(PPCA,0xBF);
/* P4 */
my1sbit(P4_0,0xC0);
my1sbit(P4_1,0xC1);
my1sbit(P4_2,0xC2);
my1sbit(P4_3,0xC3);
my1sbit(P4_4,0xC4);
my1sbit(P4_5,0xC5);
my1sbit(P4_6,0xC6);
my1sbit(P4_7,0xC7);
/* P5 */
my1sbit(P5_0,0xC8);
my1sbit(P5_1,0xC9);
my1sbit(P5_2,0xCA);
my1sbit(P5_3,0xCB);
/** stc15w has these */
my1sbit(P5_4,0xCC);
my1sbit(P5_5,0xCD);
/**
my1sbit(P5_6,0xCE);
my1sbit(P5_7,0xCF);
**/
/* CCON */
my1sbit(CCF0,0xD8);
my1sbit(CCF1,0xD9);
my1sbit(CR  ,0xDE);
my1sbit(CF  ,0xDF);
/*----------------------------------------------------------------------------*/
/** NON-{bit addressable} registers */
/* S2CON: 0x9A */
#define S2RI_BIT 0x01 /* S2CON.0 */
#define S2TI_BIT 0x02 /* S2CON.1 */
/* IE2: 0xAF */
#define ES2_BIT  0x01 /* IE2.0 */
#define ESP1_BIT 0x02 /* IE2.1 */
/* AUXR: 0x8E */
#define BRT_ENB_BIT 0x10
#define BRT_BRTR BRT_ENB_BIT /* AUXR.4 */
#define S2SMOD 0x08 /* AUXR.3 */
#define BRTx12 0x04 /* AUXR.2 */
#define S1BRS 0x01 /* AUXR.0 */
/* AUXR: 0x8E - stc15w T2 settings.. similar to BRT */
#define TR2 0x10 /* AUXR.4 */
#define T2_CTB 0x08 /* AUXR.3 */
#define T2x12 0x04 /* AUXR.2 */
#define T2_4S1 0x01 /* AUXR.0 */
/* AUXR1: 0x82 */
#define S2_ON_P4 0x10 /** AUXR1.4 */
#define PCA_ON_P4 0x40 /** AUXR1.6 */
/*----------------------------------------------------------------------------*/
/** extra interrupt vectors */
#define IPCA_VECTOR 7
#define S2I0_VECTOR 8
/*----------------------------------------------------------------------------*/
/* i/o mode 0 (M1=0,M0=0) => 20mA sink, 250uA pull (150-230uA) @default */
/* i/o mode 1 (M1=0,M0=1) => 20mA sink, 20mA pull */
/* i/o mode 2 (M1=1,M0=0) => input only (hi-Z) */
/* i/o mode 3 (M1=1,M0=1) => open drain! */
#define STC51_IOMODE_DEFAULT 0
#define STC51_IOMODE_PULLPUSH 1
#define STC51_IOMODE_HI_Z 2
#define STC51_IOMODE_OPENDRAIN 3
/*----------------------------------------------------------------------------*/
#define stc51_p0mode(md) { P0M1=(md&2)?0xff:0x00; P0M0=(md&1)?0xff:0x00; }
#define stc51_p1mode(md) { P1M1=(md&2)?0xff:0x00; P1M0=(md&1)?0xff:0x00; }
#define stc51_p2mode(md) { P2M1=(md&2)?0xff:0x00; P2M0=(md&1)?0xff:0x00; }
#define stc51_p3mode(md) { P3M1=(md&2)?0xff:0x00; P3M0=(md&1)?0xff:0x00; }
#define stc51_p4mode(md) { P4M1=(md&2)?0xff:0x00; P4M0=(md&1)?0xff:0x00; }
#define stc51_p5mode(md) { P5M1=(md&2)?0xff:0x00; P5M0=(md&1)?0xff:0x00; }
/*----------------------------------------------------------------------------*/
#define stc51_p1mode0() stc51_p1mode(STC51_IOMODE_DEFAULT)
#define stc51_p1mode1() stc51_p1mode(STC51_IOMODE_PULLPUSH)
#define stc51_p2mode0() stc51_p2mode(STC51_IOMODE_DEFAULT)
#define stc51_p2mode1() stc51_p2mode(STC51_IOMODE_PULLPUSH)
/*----------------------------------------------------------------------------*/
#endif /* __MY1STC51_H__ */
/*----------------------------------------------------------------------------*/
