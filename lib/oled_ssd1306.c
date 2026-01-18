/*----------------------------------------------------------------------------*/
#include "my1mcu51.h"
#include "timer.h"
#include "utils_lite.h"
#include "oled_ssd1306.h"
/*----------------------------------------------------------------------------*/
#define HEAD_TXT "OLED (SSD1306)"
#define MESG_TXT "THIS WORKS!"
/*----------------------------------------------------------------------------*/
char buff[] = MESG_TXT;
unsigned char tlen, step;
unsigned int loop;
/*----------------------------------------------------------------------------*/
#define MESG_ROW 2
#define LOOP_ROW 5
/*----------------------------------------------------------------------------*/
void main(void) {
	/* get buff strlen */
	tlen = 0; while (buff[tlen++]);
	i2c_init();
	oled1306_init();
	oled1306_puts(HEAD_TXT);
	oled1306_set_cursor(MESG_ROW,(128-(tlen*CHAR_WIDTH))>>1);
	oled1306_puts(buff);
	timer_init();
	oled1306_set_cursor(LOOP_ROW,0);
	oled1306_puts("LOOP:");
	tlen = 5*CHAR_WIDTH; loop = 0;
	while (1) {
		uint16_2cstr(buff,loop);
		oled1306_puts(buff);
		timer_delay1s(step,1);
		oled1306_clear_row_from(LOOP_ROW,tlen);
		oled1306_set_cursor(LOOP_ROW,tlen);
	}
}
/*----------------------------------------------------------------------------*/
