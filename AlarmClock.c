#include "mcu51.h"
#include "timer.h"
#include "uart.h"
#define STEP_COUNT_1S 20
#define ALARM_ON_TIME 10
unsigned char step, hour, ssec, mmin;
struct _alarm_t
{
	unsigned char mmin, hour;
	unsigned char time; // on time
};
struct _alarm_t alarm1;
struct _alarm_t alarm2;
struct _alarm_t alarm3;
unsigned char alarm_time(struct _alarm_t *pt)
{
	if (ssec == 0 && hour == pt->hour && mmin == pt->mmin)
	{
		pt->time = ALARM_ON_TIME;
	}
	return pt->time;
}

_sbit(ALARM, P1 ^ 0);
void show_time(void)
{
	uart_puts("TIME>");
	uart_send((hour / 10) + 0x30);
	uart_send((hour % 10) + 0x30);
	uart_send(':');
	uart_send((mmin / 10) + 0x30);
	uart_send((mmin % 10) + 0x30);
	uart_send(':');
	uart_send((ssec / 10) + 0x30);
	uart_send((ssec % 10) + 0x30);
	uart_puts("\r\n");
}
void t0_handler(void) _intr(TF0_VECTOR)
{
	TH0 = 0x4b;
	TL0 = 0xfd; // 50ms timer
	if (step)
		step--;
}

void main(void)
{
	unsigned char buff[8];
	timer_init();
	uart_init();
	uart_puts("-- I am alive!\r\n");
	TH0 = 0x4b;
	TL0 = 0xfd; // 50ms timer
	step = STEP_COUNT_1S;
	EA = 1;
	ET0 = 1;
	EX0 = 1;
	IT0 = 1; // INT0 edge triggered
	TR0 = 1;
	ssec = 0;
	mmin = 59;
	hour = 7;
	alarm1.hour = 8;
	alarm1.mmin = 0;
	alarm1.time = 0;
	ALARM = 0;
	alarm2.hour = 8;
	alarm2.mmin = 1;
	alarm2.time = 0;
	ALARM = 0;
	alarm3.hour = 8;
	alarm3.mmin = 2;
	alarm3.time = 0;
	ALARM = 0;

	show_time();
	while (1)
	{
		if (!step)
		{
			step = STEP_COUNT_1S;
			ssec++;
			if (ssec == 60)
			{
				ssec = 0;
				mmin++;
				if (mmin == 60)
				{
					mmin = 0;
					hour++;
					if (hour == 24)
						hour = 0;
				}
			}
			show_time();
			// check alarm
			ALARM = 0;

			if (alarm_time(&alarm1))
			{
				alarm1.time--;
				ALARM = 1;
			}

			if (alarm_time(&alarm2))
			{
				alarm2.time--;
				ALARM = 1;
			}

			if (alarm_time(&alarm3))
			{
				alarm3.time--;
				ALARM = 1;
			}
		}
	}
}