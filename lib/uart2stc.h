/*----------------------------------------------------------------------------*/
#ifndef __UART2STC_H__
#define __UART2STC_H__
/*----------------------------------------------------------------------------*/
/**
 * uart2stc.h
 * - library for uart2 on stc51
 * - second uart at P1.2 (RXD2) and P1.3 (TXD2)
 * - OR, can be switched to P4.2 (RXD2) and P4.3 (TXD2)
 * - use #define _UART2_ON_P4_ (or in makefile)
**/
/*----------------------------------------------------------------------------*/
#include "my1stc51.h"
#include "stc51brt.h"
/*----------------------------------------------------------------------------*/
#define uart2_init() uart2_init_baud(BRTVAL)
#define uart2_peek() (S2CON&S2RI_BIT)
#define uart2_purge() while(uart2_peek())uart2_read()
#define uart2_use_p4() AUXR1 |= S2_ON_P4
/*----------------------------------------------------------------------------*/
void uart2_init_baud(unsigned char baud) {
	S2CON = 0x50; /* uart2 mode 1, enable receiver */
	brt_load(baud); /* baudgen reload value */
	brt_smod0();
	brt_mode1t();
	brt_enable();
	/* will deprecate this macro definition */
#ifdef _UART2_ON_P4_
	uart2_use_p4();
#endif
}
/*----------------------------------------------------------------------------*/
void uart2_send(unsigned char c) {
	S2BUF = c;
	while (!(S2CON&S2TI_BIT));
	S2CON &= ~S2TI_BIT;
}
/*----------------------------------------------------------------------------*/
void uart2_puts(char *mesg) {
	while(*mesg) {
		uart2_send((unsigned char)*mesg);
		mesg++;
	}
}
/*----------------------------------------------------------------------------*/
unsigned char uart2_read(void) {
	while (!uart2_peek());
	S2CON &= ~S2RI_BIT;
	return S2BUF;
}
/*----------------------------------------------------------------------------*/
unsigned char uart2_gets(char *mesg, unsigned char size) {
	unsigned char temp, loop = 0;
	do {
		temp = uart2_read();
		uart2_send(temp);
		mesg[loop++] = temp;
		if (temp=='\n') {
			loop--;
			if (mesg[loop-1]=='\r')
				loop--;
			break;
		}
		if (loop>=size-1) {
			loop = size-1;
			break;
		}
	} while (1);
	mesg[loop] = 0x0;
	return loop;
}
/*----------------------------------------------------------------------------*/
#endif /** __UART2STC_H__ */
/*----------------------------------------------------------------------------*/
