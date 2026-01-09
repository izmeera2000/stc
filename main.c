#include "mcu51.h"

#include "uart.h"
#define LED1 P2_0
#define LED2 P2_1
#define BTNAME "nmk322bt"
#define BTPASS "0123"


sbit SERVO = P1^1;
sbit RS = P2^0;
sbit E  = P2^1;
sbit D4 = P2^2;
sbit D5 = P2^3;
sbit D6 = P2^4;
sbit D7 = P2^5;

void delay_ms(unsigned int ms) {
    unsigned int i,j;
    for(i=0;i<ms;i++)
        for(j=0;j<1200;j++);
}

void delay_us(unsigned int us) {
    unsigned int i;
    for(i=0;i<us;i++);
}

void lcd_pulse(void) {
    E = 1; delay_us(50);
    E = 0; delay_us(50);
}

void lcd_nibble(unsigned char n) {
    D4 = n & 1;
    D5 = (n>>1)&1;
    D6 = (n>>2)&1;
    D7 = (n>>3)&1;
    lcd_pulse();
}

void lcd_cmd(unsigned char c) {
    RS = 0;
    lcd_nibble(c>>4);
    lcd_nibble(c&0x0F);
    delay_ms(2);
}

void lcd_write(unsigned char c) {
    RS = 1;
    lcd_nibble(c>>4);
    lcd_nibble(c&0x0F);
    delay_ms(1);
}

void lcd_str(char *s) {
    while(*s) lcd_write(*s++);
}

void lcd_init(void) {
    delay_ms(50);
    RS = 0;
    lcd_nibble(0x03); delay_ms(5);
    lcd_nibble(0x03); delay_ms(5);
    lcd_nibble(0x03); delay_ms(5);
    lcd_nibble(0x02); delay_ms(5);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    delay_ms(5);
}

void servo_0deg(void) {
    SERVO = 1;
    delay_ms(1);
    SERVO = 0;
    delay_ms(19);
}

void servo_180deg(void) {
    SERVO = 1;
    delay_ms(2);
    SERVO = 0;
    delay_ms(18);
}

void main(void) {
    unsigned int t;

    	unsigned char temp;
	unsigned int loop;
	uart_init();
	// cuba ubah nama bt module
	uart_puts("AT+NAME" BTNAME "\r\n");
	for (loop=0;loop<60000;loop++);
	uart_puts("AT+PIN" BTPASS "\r\n");
	for (loop=0;loop<60000;loop++);

    lcd_init();
    lcd_cmd(0x80);
    lcd_str("PET FEEDER");
    lcd_cmd(0xC0);
    lcd_str("MASA MAKAN");

    while(1) {

        uart_puts("RX = ");
uart_putchar(temp);
uart_puts("\r\n");
       
        // for(t=0; t<250; t++) {
        //     servo_0deg();

            
        //     lcd_cmd(0xC0);
        //     lcd_str("Servo Buka      ");
        // }

        
        // for(t=0; t<250; t++) {
        //     servo_180deg();

           
        //     lcd_cmd(0xC0);
        //     lcd_str("Servo Tutup     ");
        // }
    }
}