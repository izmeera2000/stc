#ifndef __UART_CORE_H__
#define __UART_CORE_H__

#include "uart_core.h"

void uart_send(unsigned char tdat) {
	SBUF = tdat;
	while (!TI);
	TI = 0;
}

void uart_puts(char* mesg) {
	while (*mesg) {
		uart_send(*mesg);
		mesg++;
	}
}

#endif
