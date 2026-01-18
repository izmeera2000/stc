#ifndef __UART_TX_H__
#define __UART_TX_H__

void uart_init(void) {
	SCON = 0x50;
	TMOD &= 0x0F;
	TMOD |= 0x20;
	TH1 = 253;
	TR1 = 1;
}

#endif
