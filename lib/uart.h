#ifndef __UART_H__
#define __UART_H__
// https://codeberg.org/azman/my1code51

void uart_init(void) {
	SCON = 0x50;
	TMOD = 0x21;
	TH1 = 253;
	TR1 = 1;
}

void uart_send(unsigned char tdat) {
	SBUF = tdat;
	while (!TI);
	TI = 0;
}

unsigned char uart_read(void) {
	unsigned char rdat;
	while (!RI);
	rdat = SBUF;
	RI = 0;
	return rdat;
}

void uart_puts(char* mesg) {
	while (*mesg) {
		uart_send(*mesg);
		mesg++;
	}
}

#endif
