/*----------------------------------------------------------------------------*/
#ifndef __OLED_SSD1306_H__
#define __OLED_SSD1306_H__
/*----------------------------------------------------------------------------*/
/**
 * oled_ssd1306.h
 *   - library for ssd1306 oled controller/driver
 *   - for 128x64 oled
 *   - does not require a buffer, mainly for text
**/
/*----------------------------------------------------------------------------*/
#include "my1types.h"
/*----------------------------------------------------------------------------*/
#ifndef SSD1306_ADDRESS
#define SSD1306_ADDRESS 0x3c
#endif
/*----------------------------------------------------------------------------*/
/* control byte mask - Co bit (*_MORE seems to have no use?) */
#define SSD1306_MASK_MORE 0x80
#define SSD1306_MASK_DATA 0x40
/* control byte options */
#define SSD1306_COMMAND 0x00
#define SSD1306_DATA (SSD1306_MASK_DATA)
/*----------------------------------------------------------------------------*/
#include "font_ssd1306.h"
#include "i2c_core.h"
/*----------------------------------------------------------------------------*/
/* FONT_* should be defined in font headers */
#define CHAR_HEIGHT FONT_HEIGHT
/* with 1 spacing column */
#define CHAR_WIDTH (FONT_WIDTH+1)
/*----------------------------------------------------------------------------*/
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_TEXT_MAXROWS (OLED_HEIGHT/FONT_HEIGHT)
/*----------------------------------------------------------------------------*/
#define oled1306_print(text) oled1306_puts(text)
#define oled1306_clear_row(rr) oled1306_clear_row_from(rr,0)
/*----------------------------------------------------------------------------*/
void oled1306_command(byte08_t acmd) {
	i2c_do_start();
	i2c_do_send_byte(SSD1306_ADDRESS<<1);
	i2c_do_send_byte(SSD1306_COMMAND);
	i2c_do_send_byte(acmd);
	i2c_do_stop();
}
/*----------------------------------------------------------------------------*/
void oled1306_clear(void) {
	int loop;
	/* reset position */
	oled1306_command(0x21);
	oled1306_command(0);
	oled1306_command(127);
	oled1306_command(0x22);
	oled1306_command(0);
	oled1306_command(7);
	/* blank display */
	i2c_do_start();
	i2c_do_send_byte(SSD1306_ADDRESS<<1);
	i2c_do_send_byte(SSD1306_DATA);
	for (loop=0; loop<1024; loop++)
		i2c_do_send_byte(0);
	i2c_do_stop();
	/* reset position - single transaction instead :) */
	i2c_do_start();
	i2c_do_send_byte(SSD1306_ADDRESS<<1);
	i2c_do_send_byte(SSD1306_COMMAND);
	i2c_do_send_byte(0x22); /* set page */
	i2c_do_send_byte(0); /* page 0 */
	i2c_do_send_byte(7); /* page 7 */
	i2c_do_send_byte(0x21); /* set cols */
	i2c_do_send_byte(0); /* column 0 */
	i2c_do_send_byte(127); /* column 127 */
	i2c_do_stop();
}
/*----------------------------------------------------------------------------*/
void oled1306_init(void) {
	oled1306_command(0xAE); /** display off */
	/* INIT: software initialization flow - from datasheet */
	oled1306_command(0xA8); /** set mux ratio? default is 63? 0x3f? */
	oled1306_command(0x3F); /* this is default??? */
	oled1306_command(0xD3); /** set display offset */
	oled1306_command(0x00); /* no offset? */
	oled1306_command(0x40 | 0x00); /** set start line (value mask:0x3f) */
	oled1306_command(0xA0 | 0x01); /** set segment remap (value:0|1) */
	oled1306_command(0xC0 | 0x08); /** set com scan dir (vmask:0x00) */
	oled1306_command(0xDA); /** set com pins hw config */
	oled1306_command(0x12); /* sample use 0x12? datasheet use 0x02 */
	oled1306_command(0x81); /** set contrast control */
	oled1306_command(0xCF); /* sample use 0xCF? datasheet use 0x7F */
	oled1306_command(0xA4); /** disable 'entire display on' - o/p ram */
	oled1306_command(0xA6); /** set normal display */
	oled1306_command(0xD5); /** set clock divider ratio & osc freq */
	oled1306_command(0x80); /* freq:0x80(0x00) | cdiv:0x00(0x01) */
	oled1306_command(0x8D); /** set charge pump */
	oled1306_command(0x14); /* internal vcc */
	/* ENDS: software initialization flow - from datasheet */
	oled1306_command(0x20); /** set memory addressing mode*/
	oled1306_command(0x00); /* horizontal: autoinc col, autoinc page */
	oled1306_command(0xD9); /** set precharge period */
	oled1306_command(0xF1);
	oled1306_command(0xDB); /** set vcom deselect */
	oled1306_command(0x40);
	oled1306_command(0x2E); /** STOP SCROLLING? DO I NEED THIS??? */
	oled1306_command(0xAF); /** display on */
	/* clear on init? */
	oled1306_clear();
}
/*----------------------------------------------------------------------------*/
void oled1306_putchar(char show) {
	byte08_t loop;
	if (show>=0x20&&show<0x80) {
		show -= 0x20;
		i2c_do_start();
		i2c_do_send_byte(SSD1306_ADDRESS<<1);
		i2c_do_send_byte(SSD1306_DATA);
		for (loop=0;loop<FONT_WIDTH;loop++)
			i2c_do_send_byte(oled1306_font5x8[show][loop]);
		i2c_do_send_byte(0x00); /* spacing column - actually 6x8 :P */
		i2c_do_stop();
	}
}
/*----------------------------------------------------------------------------*/
void oled1306_puts(char* mesg) {
	while (*mesg) {
		oled1306_putchar(*mesg);
		mesg++;
	}
}
/*----------------------------------------------------------------------------*/
void oled1306_set_cursor(byte08_t page,byte08_t cpos) {
	if (page<8&&cpos<128) {
		oled1306_command(0x21);
		oled1306_command(cpos);
		oled1306_command(127);
		oled1306_command(0x22);
		oled1306_command(page);
		oled1306_command(7);
	}
}
/*----------------------------------------------------------------------------*/
void oled1306_blit(byte08_t* bdat, int size) {
	int loop;
	i2c_do_start();
	i2c_do_send_byte(SSD1306_ADDRESS<<1);
	i2c_do_send_byte(SSD1306_DATA);
	for (loop=0; loop<size; loop++)
		i2c_do_send_byte(bdat[loop]);
	i2c_do_stop();
}
/*----------------------------------------------------------------------------*/
void oled1306_clear_row_from(byte08_t crow, byte08_t init) {
	i2c_do_start();
	i2c_do_send_byte(SSD1306_ADDRESS<<1);
	i2c_do_send_byte(SSD1306_COMMAND);
	i2c_do_send_byte(0x22); /* set page */
	i2c_do_send_byte(crow);
	i2c_do_send_byte(7);
	i2c_do_send_byte(0x21); /* set cols */
	i2c_do_send_byte(init);
	i2c_do_send_byte(127);
	i2c_do_stop();
	i2c_do_start();
	i2c_do_send_byte(SSD1306_ADDRESS<<1);
	i2c_do_send_byte(SSD1306_DATA);
	for (crow=0; crow<128; crow++)
		i2c_do_send_byte(0x00);
	i2c_do_stop();
}
/*----------------------------------------------------------------------------*/
#endif /** __OLED_SSD1306_H__ */
/*----------------------------------------------------------------------------*/
