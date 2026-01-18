/*----------------------------------------------------------------------------*/
#ifndef __TM1651_H__
#define __TM1651_H__
/*----------------------------------------------------------------------------*/
/**
 * tm1651.h
 * - library for tm1651 led driver/controller (e.g. battery level display)
**/
/*----------------------------------------------------------------------------*/
#include "my1mcu51.h"
/*----------------------------------------------------------------------------*/
#define TM1651_DISPLAY_MODE 0x00
#define TM1651_DATA_COMMAND 0x40
#define TM1651_DISPLAY_CTRL 0x80
#define TM1651_ADDR_COMMAND 0xC0
/*----------------------------------------------------------------------------*/
/** data command - bit0 is NEVER 1! */
#define TM1651_READ_KSCAN 0x02
#define TM1651_FIXED_ADDR 0x04
#define TM1651_ITEST_MODE 0x08
/*----------------------------------------------------------------------------*/
/** addr command */
#define TM1651_ADDR_0 0x00
#define TM1651_ADDR_1 0x01
#define TM1651_ADDR_2 0x02
#define TM1651_ADDR_3 0x03
/*----------------------------------------------------------------------------*/
/** display ctrl */
#define TM1651_DISPLAY_ON 0x08
#define TM1651_PULSE01_16 0x00
#define TM1651_PULSE02_16 0x01
#define TM1651_PULSE04_16 0x02
#define TM1651_PULSE10_16 0x03
#define TM1651_PULSE11_16 0x04
#define TM1651_PULSE12_16 0x05
#define TM1651_PULSE13_16 0x06
#define TM1651_PULSE14_16 0x07
/*----------------------------------------------------------------------------*/
#define TM1651_DO_DISPLAY (TM1651_DISPLAY_ON|TM1651_PULSE11_16)
/*----------------------------------------------------------------------------*/
/** allow code to override these pins! */
#ifndef TM1651_CLK
#define TM1651_CLK P1_6
#endif
#ifndef TM1651_DIO
#define TM1651_DIO P1_7
#endif
/*----------------------------------------------------------------------------*/
#define TM1651_WAIT(tlen) tm1651_wait(tlen)
/*----------------------------------------------------------------------------*/
void tm1651_init(void) {
	TM1651_CLK = 1;
	TM1651_DIO = 1;
}
/*----------------------------------------------------------------------------*/
void tm1651_wait(unsigned char ncnt) {
	do {
		NOP;
	} while(--ncnt);
}
/*----------------------------------------------------------------------------*/
void tm1651_do_start(void) {
	TM1651_CLK = 1;
	TM1651_DIO = 1;
	/*TM1651_WAIT(TM1651_WAIT_TLEN);*/
	TM1651_DIO = 0;
	/*TM1651_WAIT(TM1651_WAIT_TLEN);*/
	TM1651_CLK = 0;
}
/*----------------------------------------------------------------------------*/
void tm1651_do_stop(void) {
	TM1651_DIO = 0;
	TM1651_CLK = 1;
	/*TM1651_WAIT(TM1651_WAIT_TLEN);*/
	TM1651_DIO = 1;
	/*TM1651_WAIT(TM1651_WAIT_TLEN);*/
}
/*----------------------------------------------------------------------------*/
unsigned char tm1651_do_send_byte(unsigned char bdat) {
	unsigned char loop;
	for (loop=0;loop<8;loop++) {
		TM1651_DIO = bdat&0x01 ? 1 : 0;
		TM1651_CLK = 1;
		/*TM1651_WAIT(TM1651_WAIT_TLEN);*/
		TM1651_CLK = 0;
		/*TM1651_WAIT(TM1651_WAIT_HALF);*/
		bdat >>= 1;
	}
	TM1651_DIO = 1;
	TM1651_CLK = 1;
	/*TM1651_WAIT(TM1651_WAIT_HALF);*/
	loop = TM1651_DIO ? 1 : 0;
	TM1651_CLK = 0;
	return loop; /* returns zero if acknowledged */
}
/*----------------------------------------------------------------------------*/
void tm1651_write_data(unsigned char *bdat) {
	tm1651_do_start();
	tm1651_do_send_byte(TM1651_DATA_COMMAND);
	tm1651_do_stop();
	tm1651_do_start();
	tm1651_do_send_byte(TM1651_ADDR_COMMAND);
	tm1651_do_send_byte(bdat[0]);
	tm1651_do_send_byte(bdat[1]);
	tm1651_do_send_byte(bdat[2]);
	tm1651_do_send_byte(bdat[3]);
	tm1651_do_stop();
	tm1651_do_start();
	tm1651_do_send_byte(TM1651_DISPLAY_CTRL|TM1651_DO_DISPLAY);
	tm1651_do_stop();
}
/*----------------------------------------------------------------------------*/
void tm1651_level(unsigned char level, unsigned char frame) {
	unsigned char pick, outl, that;
	that = TM1651_DISPLAY_CTRL;
	switch (level) {
		default:
		case 0: pick = 0x00; break;
		case 1: pick = 0x01; break;
		case 2: pick = 0x03; break;
		case 3: pick = 0x07; break;
		case 4: pick = 0x0f; break;
		case 5: pick = 0x1f; break;
	}
	outl = frame ? 0x20 : 0x00;
	if (level||frame) that |= TM1651_DO_DISPLAY;
	tm1651_do_start();
	tm1651_do_send_byte(TM1651_DATA_COMMAND);
	tm1651_do_stop();
	tm1651_do_start();
	tm1651_do_send_byte(TM1651_ADDR_COMMAND);
	tm1651_do_send_byte(pick);
	tm1651_do_send_byte(outl);
	tm1651_do_stop();
	tm1651_do_start();
	tm1651_do_send_byte(that);
	tm1651_do_stop();
}
/*----------------------------------------------------------------------------*/
#endif /** __TM1651_H__ */
/*----------------------------------------------------------------------------*/
