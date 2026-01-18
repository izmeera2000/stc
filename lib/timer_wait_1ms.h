#ifndef __TIMER_WAIT_1MS_H__
#define __TIMER_WAIT_1MS_H__

#include "timer_core.h"

void timer_wait_1ms(unsigned int step) {
	do {
		// 1 ms delay
		TH0 = 0xfc; TL0 = 0x66;
		TR0 = 1; while (!TF0);
		TR0 = 0; TF0 = 0;
		step--;
	} while(step);
}

#endif