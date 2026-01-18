#ifndef __TIMER_H__
#define __TIMER_H__

#include "timer_core.h"
/* 1st param must be a defined var that can hold secs*TIMER_LOOP_1S */
#define timer_delay1s(step,secs) { step = secs*20; \
	do { timer_wait(TVAL_DELAY_50MS); } while (--step); }

#endif