/*----------------------------------------------------------------------------*/
#ifndef __PCA_STC_H__
#define __PCA_STC_H__
/*----------------------------------------------------------------------------*/
/**
 * pca_stc.h
 * - library for pca module on stc51
 * - 2 pca modules (module0 and module1)
 * - pins: module0 @ p1.3|p4.2 module1 @ p1.4|p4.3
**/
/*----------------------------------------------------------------------------*/
#include "my1stc51.h"
/*----------------------------------------------------------------------------*/
/* CIDL=0 counter runs during idle, CIDL=1 counter gated off */
#define CMOD_IDLE_MASK 0x80
/* ECF=1 enables  interrupt for CF bit in CCON */
#define CMOD_ENABLE_CF 0x01
/* PCA counter pulse source mask */
#define CMOD_CPS_MASK 0x0E
/* PCA CPS 0 - sysclk/12 */
#define CMOD_CPS_PICK0 0x00
/* PCA CPS 1 - sysclk/2 */
#define CMOD_CPS_PICK1 0x02
/* PCA CPS 2 - timer 0 overflow */
#define CMOD_CPS_PICK2 0x04
/* PCA CPS 3 - external clk @ECI/P1.2 (max sysclk/2) */
#define CMOD_CPS_PICK3 0x06
/* PCA CPS 4-7 : sysclk,sysclk/4,sysclk/6,sysclk/8 */
#define CMOD_CPS_T0 CMOD_CPS_PICK2
/*----------------------------------------------------------------------------*/
/** CCON is bit-addressable */
/* PCA counter overflow flag, cleared by sw */
#define CCON_CF_BIT 0x80
/* PCA counter control bit */
#define CCON_CR_BIT 0x40
/* PCA module 1 interrupt flag */
#define CCON_CCF1_BIT 0x02
/* PCA module 0 interrupt flag */
#define CCON_CCF0_BIT 0x01
/*----------------------------------------------------------------------------*/
/* ECOM* : enable comparator function */
#define CCAP_ECOM_BIT 0x40
/* MAT* : enable match function */
#define CCAP_MAT_BIT 0x08
/* TOG* : enable toggle function */
#define CCAP_TOG_BIT 0x04
/* PWM* : enables pwm output on CCP* pin  */
#define CCAP_PWM_BIT 0x02
/* ECCF* : enable ccf interupt */
#define CCAP_ECCF_BIT 0x01
/** PCA sw timer mode */
#define PCA_MODE_SWTIMER (CCAP_ECOM_BIT|CCAP_MAT_BIT)
/** PCA 8-bit pwm mode */
#define PCA_MODE_PWM (CCAP_ECOM_BIT|CCAP_PWM_BIT)
/*----------------------------------------------------------------------------*/
/* stop counter, clear flags, mode in reset state */
#define pca_init() { CCON=0; CMOD=0; }
/* set base timer values */
#define pca_base(hval,lval) { CL=0; CH=0; }
/* set pca pin to p4 */
#define pca_pins_p4() AUXR1 |= PCA_ON_P4
/* set pca clock pulse source */
#define pca_csrc(psrc) { CMOD &= ~CMOD_CPS_MASK; CMOD |= psrc&CMOD_CPS_MASK; }
/* set module 0/1 as 16-bit software timers */
#define pca_mod0_swtimer16b() CCAPM0=PCA_MODE_SWTIMER
#define pca_mod1_swtimer16b() CCAPM1=PCA_MODE_SWTIMER
/* set module 0/1 as 8-bit pwm generator */
#define pca_mod0_pwm8b() CCAPM0=PCA_MODE_PWM
#define pca_mod1_pwm8b() CCAPM1=PCA_MODE_PWM
/* set module 0/1 capture/compare registers */
#define pca_mod0_ccmp(hval,lval) { CCAP0L = lval; CCAP0H = hval; }
#define pca_mod1_ccmp(hval,lval) { CCAP1L = lval; CCAP1H = hval; }
#define pca_mod0_make(tval) { CCAP0L = tval&0xff; CCAP0H = (tval>>8)&0xff; }
#define pca_mod1_make(tval) { CCAP1L = tval&0xff; CCAP1H = (tval>>8)&0xff; }
/*----------------------------------------------------------------------------*/
#endif /** __PCA_STC_H__ */
/*----------------------------------------------------------------------------*/
