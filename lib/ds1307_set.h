/*----------------------------------------------------------------------------*/
#include "i2c.h"
#include "uart_hexascii.h"
#include "utils.h"
/*----------------------------------------------------------------------------*/
/** rtc module based on ds1307 */
#define RTC_I2C_ADDR 0x68
#define RTC_ENB_MASK 0x80
/*----------------------------------------------------------------------------*/
#define RTC_DATE_YY 0x23
#define RTC_DATE_mm 0x10
#define RTC_DATE_dd 0x31
/* day code */
#define RTC_DATE_XX 2
#define RTC_TIME_HH 0x08
#define RTC_TIME_MM 0x36
#define RTC_TIME_SS 0x00
/*----------------------------------------------------------------------------*/
unsigned char pdat[8], keep, test, show;
/*----------------------------------------------------------------------------*/
void main(void) {
	uart_init();
	i2c_init();
	pdat[0] = RTC_TIME_SS;
	pdat[1] = RTC_TIME_MM;
	pdat[2] = RTC_TIME_HH;
	pdat[3] = RTC_DATE_XX;
	pdat[4] = RTC_DATE_dd;
	pdat[5] = RTC_DATE_mm;
	pdat[6] = RTC_DATE_YY;
	i2c_putb(RTC_I2C_ADDR,0x00,pdat,7);
	keep = RTC_TIME_SS;
	show = 1;
	while (1) {
		i2c_getb(RTC_I2C_ADDR,0x00,pdat,8);
		test = pdat[0]&~RTC_ENB_MASK;
		if (test!=keep||show) {
			uart_send_hexbyte(pdat[6]); // YY
			uart_send('-');
			uart_send_hexbyte(pdat[5]); // mm
			uart_send('-');
			uart_send_hexbyte(pdat[4]); // dd
			uart_send('T');
			uart_send_hexbyte(pdat[2]); // hour
			uart_send(':');
			uart_send_hexbyte(pdat[1]); // minute
			uart_send(':');
			uart_send_hexbyte(test); // second
			uart_puts(" Day:");
			uart_send(nibb_lo2ascii(pdat[3])); // day (1=Mon,...,7=Sun)
			uart_puts(" Cfg:");
			uart_send_hexbyte(pdat[7]); // config
			uart_send('\n');
			keep = test;
			show = 0;
		}
	}
}
/*----------------------------------------------------------------------------*/
