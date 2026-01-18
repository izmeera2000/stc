/*----------------------------------------------------------------------------*/
#ifndef __MY1TIMER_H__
#define __MY1TIMER_H__
/*----------------------------------------------------------------------------*/
/**
 * timer.h
 * - library for basic timer functions
 *   = no functions here... only macros
 * - using timer in mode 1 (default: use timer0)
**/
/*----------------------------------------------------------------------------*/
#include "my1mcu51.h"
/*----------------------------------------------------------------------------*/
/* for FOSC = 11.0592MHz (12x) => TDEL = (1/FOSC)*12 = 1.08507us */
/* DELAY = STEP * TDEL => STEP = 65536 - REGVAL */
/* REGVAL = 65536 - (DELAY/TDEL) */
/** for DELAY=50ms, REGVAL = 65536 - (50/1.085)K = 19453 (0x4bfd) */
/** for DELAY=10ms, REGVAL = 65536 - (10/1.085)K = 56319 (0xdbff) */
/** for DELAY=5ms, REGVAL = 65536 - (5/1.085)K = 60928 (0xee00) */
/** for DELAY=1ms, REGVAL = 65536 - (1/1.085)K = 64614 (0xfc66) */
/**
 *  10us:65527@0xfff7 , 20us:65518@0xffee
 *  30us:65508@0xffe4 , 40us:65499@0xffdb
 *  50us:65490@0xffd2 , 100us:65443@0xffa4
 *  500us:65075@0xfe33
**/
/*----------------------------------------------------------------------------*/
#ifndef TIMER_VAL50MS
#define TIMER_VAL50MS 0x4bfd
#endif
#ifndef TIMER_VAL10MS
#define TIMER_VAL10MS 0xdbff
#endif
#ifndef TIMER_VAL01MS
#define TIMER_VAL01MS 0xfc66
#endif
#ifndef TIMER_IVAL_1S
#define TIMER_IVAL_1S TIMER_VAL50MS
#endif
#ifndef TIMER_LOOP_1S
#define TIMER_LOOP_1S 20
#endif
/*----------------------------------------------------------------------------*/
#define timer0_run0() TR0 = 0
#define timer0_run1() TR0 = 1
#define timer0_rst0() TF0 = 0
#define timer0_init() TMOD &= 0xF0; TMOD |= 0x01
#define timer0_tval(hi,lo) TH0 = hi; TL0 = lo
#define timer0_null() timer0_tval(0,0)
#define timer0_prep(tdel) timer0_tval((tdel>>8)&0xff,tdel&0xff)
#define timer0_stop() timer0_run0(); timer0_rst0()
#define timer0_exec() timer0_run1()
#define timer0_flag() timer0_exec(); while (!TF0)
#define timer0_wait(tdel) timer0_prep(tdel); timer0_flag(); timer0_stop()
#define timer0_evdo() ET0 = 1; EA = 1
#define timer0_make(tdel) timer0_stop(); timer0_prep(tdel); timer0_evdo()
#define timer0_read() ((unsigned int)TH0<<8)|TL0
#define timer0_done() (TF0==1)
#define timer0_running() (TR0==1)
/*----------------------------------------------------------------------------*/
/* 1st param must be a defined var that can hold secs*TIMER_LOOP_1S */
#define timer0_delay1s(step,secs) { step = secs*TIMER_LOOP_1S; \
	do { timer0_wait(TIMER_IVAL_1S); } while (--step); }
/*----------------------------------------------------------------------------*/
#define timer_run1 timer0_run1
#define timer_run0 timer0_run0
#define timer_rst0 timer0_rst0
#define timer_init timer0_init
#define timer_tval timer0_tval
#define timer_null timer0_null
#define timer_prep timer0_prep
#define timer_stop timer0_stop
#define timer_exec timer0_exec
#define timer_flag timer0_flag
#define timer_wait timer0_wait
#define timer_evdo timer0_evdo
#define timer_make timer0_make
#define timer_read timer0_read
#define timer_done timer0_done
#define timer_running timer0_running
#define timer_delay1s timer0_delay1s
/*----------------------------------------------------------------------------*/
#define timer_wait_50ms() timer_wait(TIMER_VAL50MS)
#define timer_wait_1ms() timer_wait(TIMER_VAL01MS)
#define timer_init_dual() TMOD = 0x11
/*----------------------------------------------------------------------------*/
#define timer1_run0() TR1 = 0
#define timer1_run1() TR1 = 1
#define timer1_rst0() TF1 = 0
#define timer1_done() (TF1==1)
#define timer1_init() TMOD &= 0x0F; TMOD |= 0x10
#define timer1_tval(hi,lo) TH1 = hi; TL1 = lo
#define timer1_null() timer1_tval(0,0)
#define timer1_prep(tdel) timer1_tval((tdel>>8)&0xff,tdel&0xff)
#define timer1_stop() timer1_run0(); timer1_rst0()
#define timer1_exec() timer1_run1()
#define timer1_flag() timer1_exec(); while (!TF0)
#define timer1_read() ((unsigned int)TH1<<8)|TL1
#define timer1_wait(tdel) timer1_prep(tdel); timer1_flag(); timer1_stop()
#define timer1_evdo() ET1 = 1; EA = 1
#define timer1_make(tdel) timer1_stop(); timer1_prep(tdel); timer1_evdo()
#define timer1_running() (TR1==1)
/*----------------------------------------------------------------------------*/
#define timer1_delay1s(step,secs) { step = secs*TIMER_LOOP_1S; \
	do { timer1_wait(TIMER_IVAL_1S); } while (--step); }
/*----------------------------------------------------------------------------*/
#endif /** __MY1TIMER_H__ */
/*----------------------------------------------------------------------------*/
