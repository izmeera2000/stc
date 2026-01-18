#ifndef __UART_RX_H__
#define __UART_RX_H__

#include "uart_core.h"

unsigned char uart_read(void) {
	unsigned char rdat;
	while (!RI);
	rdat = SBUF;
	RI = 0;
	return rdat;
}

#endif
