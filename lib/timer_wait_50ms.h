#ifndef __TIMER_WAIT_50MS_H__
#define __TIMER_WAIT_50MS_H__

#include "timer_core.h"

void timer_wait_50ms(unsigned char step) {
	do {
		// 50 ms delay
		TH0 = 0x4b; TL0 = 0xfd;
		TR0 = 1; while (!TF0);
		TR0 = 0; TF0 = 0;
		step--;
	} while(step);
}

#endif