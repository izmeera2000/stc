/*----------------------------------------------------------------------------*/
#ifndef __TM1637_H__
#define __TM1637_H__
/*----------------------------------------------------------------------------*/
/**
 * tm1637.h
 * - library for tm1637 7-segment driver/controller
 * - uses tm1651 as 'core'
 * - requires led7seg
**/
/*----------------------------------------------------------------------------*/
#include "tm1651.h"
#include "led7seg.h"
/*----------------------------------------------------------------------------*/
#define TM1637_DISPLAY_MODE TM1651_DISPLAY_MODE
#define TM1637_DATA_COMMAND TM1651_DATA_COMMAND
#define TM1637_DISPLAY_CTRL TM1651_DISPLAY_CTRL
#define TM1637_ADDR_COMMAND TM1651_ADDR_COMMAND
/*----------------------------------------------------------------------------*/
/** data command - bit0 is NEVER 1! */
#define TM1637_READ_KSCAN TM1651_READ_KSCAN
#define TM1637_FIXED_ADDR TM1651_FIXED_ADDR
#define TM1637_ITEST_MODE TM1651_ITEST_MODE
/*----------------------------------------------------------------------------*/
/** addr command */
#define TM1637_ADDR_0 TM1651_ADDR_0
#define TM1637_ADDR_1 TM1651_ADDR_1
#define TM1637_ADDR_2 TM1651_ADDR_2
#define TM1637_ADDR_3 TM1651_ADDR_3
/*----------------------------------------------------------------------------*/
/** display ctrl */
#define TM1637_DISPLAY_ON TM1651_DISPLAY_ON
#define TM1637_PULSE01_16 TM1651_PULSE01_16
#define TM1637_PULSE02_16 TM1651_PULSE02_16
#define TM1637_PULSE04_16 TM1651_PULSE04_16
#define TM1637_PULSE10_16 TM1651_PULSE10_16
#define TM1637_PULSE11_16 TM1651_PULSE11_16
#define TM1637_PULSE12_16 TM1651_PULSE12_16
#define TM1637_PULSE13_16 TM1651_PULSE13_16
#define TM1637_PULSE14_16 TM1651_PULSE14_16
/*----------------------------------------------------------------------------*/
#define TM1637_DO_DISPLAY (TM1637_DISPLAY_ON|TM1637_PULSE11_16)
/*----------------------------------------------------------------------------*/
/** allow code to override these pins! */
#ifndef TM1637_CLK
#define TM1637_CLK P1_6
#endif
#ifndef TM1637_DIO
#define TM1637_DIO P1_7
#endif
/*----------------------------------------------------------------------------*/
#define TM1637_WAIT(tlen) tm1637_wait(tlen)
#define tm1637_init tm1651_init
#define tm1637_wait tm1651_wait
#define tm1637_do_start tm1651_do_start
#define tm1637_do_stop tm1651_do_stop
#define tm1637_do_send_byte tm1651_do_send_byte
#define tm1637_write_data tm1651_write_data
/*----------------------------------------------------------------------------*/
unsigned char __xdata seg7hex[] = { _7SEGHEX_CC_ };
/*----------------------------------------------------------------------------*/
void tm1637_display(unsigned char *bdat) {
	unsigned char loop;
	tm1637_do_start();
	tm1637_do_send_byte(TM1637_DATA_COMMAND);
	tm1637_do_stop();
	tm1637_do_start();
	tm1637_do_send_byte(TM1637_ADDR_COMMAND);
	for (loop=0;loop<4;loop++)
		tm1637_do_send_byte(bdat[loop]);
	tm1637_do_stop();
	tm1637_do_start();
	tm1637_do_send_byte(TM1637_DISPLAY_CTRL|TM1637_DO_DISPLAY);
	tm1637_do_stop();
}
/*----------------------------------------------------------------------------*/
void tm1637_display_num(int num, unsigned char pad) {
	unsigned char bdat[4], pick;
	if (num>=10000||num<0) num = 0;
	for (pick=4;pick>0;) {
		bdat[--pick] = num%10;
		num /= 10;
	}
	for (pick=0;pick<4;pick++) {
		if (!pad&&pick<3&&!bdat[pick])
			bdat[pick] = 0;
		else {
			bdat[pick] = seg7hex[bdat[pick]];
			pad++;
		}
	}
	tm1637_display(bdat);
}
/*----------------------------------------------------------------------------*/
void tm1637_display_hex(unsigned int hex, unsigned char pad) {
	unsigned char bdat[4], pick;
	for (pick=4;pick>0;) {
		bdat[--pick] = hex&0xf;
		hex >>= 4;
	}
	for (pick=0;pick<4;pick++) {
		if (!pad&&pick<3&&!bdat[pick])
			bdat[pick] = 0;
		else {
			bdat[pick] = seg7hex[bdat[pick]];
			pad++;
		}
	}
	tm1637_display(bdat);
}
/*----------------------------------------------------------------------------*/
#endif /** __TM1637_H__ */
/*----------------------------------------------------------------------------*/
