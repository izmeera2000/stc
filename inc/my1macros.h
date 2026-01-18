/*----------------------------------------------------------------------------*/
#ifndef __MY1MACROS_H__
#define __MY1MACROS_H__
/*----------------------------------------------------------------------------*/
/**
 * my1macros.h
 * - by azman@my1matrix.org
 * - useful macros
**/
/*----------------------------------------------------------------------------*/
/* nothing to do... */
#define hang() while (1)
/*----------------------------------------------------------------------------*/
/* convert a decimal value to ascii char - max=99 */
#define dec2char10(nval) (nval/10)+0x30
#define dec2char01(nval) (nval%10)+0x30
/*----------------------------------------------------------------------------*/
/* convert a 4-bit value to hexascii */
#define hex2ASCII(nval) (nval<10)?(nval+0x30):(nval+0x37)
#define hex2ascii(nval) (nval<10)?(nval+0x30):(nval+0x57)
/*----------------------------------------------------------------------------*/
#ifdef HEXASCII_ALLCAPS
#define nibb2ascii hex2ASCII
#else
#define nibb2ascii hex2ascii
#endif
/*----------------------------------------------------------------------------*/
#define nibb_hi(bval) ((bval>>4)&0x0f)
#define nibb_lo(bval) (bval&0x0f)
/*----------------------------------------------------------------------------*/
#define nibb_hi2ascii(bval) nibb2ascii(nibb_hi(bval))
#define nibb_lo2ascii(bval) nibb2ascii(nibb_lo(bval))
/*----------------------------------------------------------------------------*/
/* a simple count down loop - step variable can be declared 8-bit or 16-bit */
#define wait_loop(step,many) { step=many; while(--step); }
/*----------------------------------------------------------------------------*/
/* a wait_loop with inner job */
#define wait_that(step,many,that) { step=many; while(--step) that; }
/*----------------------------------------------------------------------------*/
/* stringify macro constants */
#define mstr(a) macrostr(a)
#define macrostr(a) #a
/*----------------------------------------------------------------------------*/
#endif /** __MY1MACROS_H__ */
/*----------------------------------------------------------------------------*/
