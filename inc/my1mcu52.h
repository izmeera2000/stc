/*----------------------------------------------------------------------------*/
#ifndef __MY1MCU52_H__
#define __MY1MCU52_H__
/*----------------------------------------------------------------------------*/
/**
 * my1mcu52.h
 * - by azman@my1matrix.org
 * - definitions for 8052 sfrs/sbits
 *   = note that some overlaps with stc51 (so these cannot be in mcu51.h)
**/
/*----------------------------------------------------------------------------*/
#include "my1mcu51.h"
/*----------------------------------------------------------------------------*/
/* available in 8052 */
my1sfr(T2CON ,0xC8);
my1sfr(RCAP2L,0xCA);
my1sfr(RCAP2H,0xCB);
my1sfr(TL2   ,0xCC);
my1sfr(TH2   ,0xCD);
/*----------------------------------------------------------------------------*/
my1sbit(CPRL2,0xC8);
my1sbit(CT2  ,0xC9);
my1sbit(TR2  ,0xCA);
my1sbit(EXEN2,0xCB);
my1sbit(TCLK ,0xCC);
my1sbit(RCLK ,0xCD);
my1sbit(EXF2 ,0xCE);
my1sbit(TF2  ,0xCF);
/*----------------------------------------------------------------------------*/
/* extends IE */
my1sbit(ET2  ,0xAD);
/* extends IP */
my1sbit(PT2  ,0xBD);
/*----------------------------------------------------------------------------*/
/* dual/alternate function @P1 */
my1sbit(T2   ,0x90); /* external clock for T2 */
my1sbit(T2EX ,0x91); /* reload/capture/direction control for T2 */
/*----------------------------------------------------------------------------*/
/* i-vector for TF2 & EXF2 @ 0x2b */
#define TF2_VECTOR 5
/*----------------------------------------------------------------------------*/
#endif /* __MY1MCU52_H__ */
/*----------------------------------------------------------------------------*/
