/*----------------------------------------------------------------------------*/
#ifndef __TIMER2STC_H__
#define __TIMER2STC_H__
/*----------------------------------------------------------------------------*/
/**
 * timer2stc.h
 * - library for timer functions using timer2 on stc51
 * - PCA as software timer
**/
/*----------------------------------------------------------------------------*/
#include "pca_stc.h"
/*----------------------------------------------------------------------------*/
#ifndef PCAFOSC
#define PCAFOSC 11059200L
#endif
#define T2PCA1KHZ (PCAFOSC/12/1000)
#define T2PCA_NEXT T2PCA1KHZ
/*----------------------------------------------------------------------------*/
unsigned long __xdata pca1_mark, pca1_test;
unsigned long pca1_01ms;
unsigned int pca1_next;
/*----------------------------------------------------------------------------*/
#define pca1_make(tv) pca_mod1_make(tv)
#define pca1_exec() CCAPM1 |= CCAP_ECCF_BIT
#define pca1_stop() CCAPM1 &= ~CCAP_ECCF_BIT
#define pca1_conf pca_mod1_swtimer16b
/*----------------------------------------------------------------------------*/
void timer2_init(void) {
	/* config pca 1 as software timer */
	pca_init();
	pca_base(0,0);
	pca1_next = T2PCA_NEXT;
	pca1_make(pca1_next);
	pca1_next += T2PCA_NEXT;
	pca1_01ms = 0;
	pca1_conf();
	pca1_exec();
	EA = 1; /* enable global interrupt! */
	CR = 1; /* run pca timer! CCON.7 */
}
/*----------------------------------------------------------------------------*/
unsigned long timer2_read(void) {
	pca1_stop();
	pca1_test = pca1_01ms;
	pca1_exec();
	return pca1_test;
}
/*----------------------------------------------------------------------------*/
void timer2_wait(unsigned long tdel) {
	unsigned long mark = timer2_read();
	while (timer2_read()-mark<tdel);
}
/*----------------------------------------------------------------------------*/
/* pca interrupt handler */
void pca_handler(void) __interrupt(IPCA_VECTOR) {
	CCF1 = 0; /** CCON.1 */
	pca1_make(pca1_next);
	pca1_next += T2PCA_NEXT;
	pca1_01ms++;
}
/*----------------------------------------------------------------------------*/
#endif /** __TIMER2STC_H__ */
/*----------------------------------------------------------------------------*/
