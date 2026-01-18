#ifndef __TIMER_CORE_H__
#define __TIMER_CORE_H__

//void timer_init(void) {
//	// using Timer0 mode 1
//	TMOD &= 0xF0;
//	TMOD |= 0x01;
//}
#define timer_init() TMOD &= 0xF0; TMOD |= 0x01

#define TVAL_DELAY_1MS 0xfc66
#define TVAL_DELAY_50MS 0x4bfd

#define timer_tval(hi,lo) TH0 = hi; TL0 = lo
#define timer_run0() TR0 = 0
#define timer_run1() TR0 = 1
#define timer_rst0() TF0 = 0
#define timer_null() timer_tval(0,0)
#define timer_prep(tdel) timer_tval((tdel>>8)&0xff,tdel&0xff)
#define timer_stop() timer_run0(); timer_rst0()
#define timer_exec() timer_run1()
#define timer_flag() timer_exec(); while (!TF0)
#define timer_wait(tdel) timer_prep(tdel); timer_flag(); timer_stop()

#endif