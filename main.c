#include "mcu51.h"
#include "uart.h"
#include "timer2stc.h"
#include "timer.h"

#define BTNAME "nmk322bt"
#define BTPASS "0123"



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



sbit SERVO = P1 ^ 1;
sbit RS = P2 ^ 0;
sbit E = P2 ^ 1;
sbit D4 = P2 ^ 2;
sbit D5 = P2 ^ 3;
sbit D6 = P2 ^ 4;
sbit D7 = P2 ^ 5;


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



void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1200; j++)
            ;
}

void delay_us(unsigned int us)
{
    unsigned int i;
    for (i = 0; i < us; i++)
        ;
}

void lcd_pulse(void)
{
    E = 1;
    delay_us(50);
    E = 0;
    delay_us(50);
}

void lcd_nibble(unsigned char n)
{
    D4 = n & 1;
    D5 = (n >> 1) & 1;
    D6 = (n >> 2) & 1;
    D7 = (n >> 3) & 1;
    lcd_pulse();
}

void lcd_cmd(unsigned char c)
{
    RS = 0;
    lcd_nibble(c >> 4);
    lcd_nibble(c & 0x0F);
    delay_ms(2);
}

void lcd_write(unsigned char c)
{
    RS = 1;
    lcd_nibble(c >> 4);
    lcd_nibble(c & 0x0F);
    delay_ms(1);
}

void lcd_str(char *s)
{
    while (*s)
        lcd_write(*s++);
}

void lcd_init(void)
{
    delay_ms(50);
    RS = 0;
    lcd_nibble(0x03);
    delay_ms(5);
    lcd_nibble(0x03);
    delay_ms(5);
    lcd_nibble(0x03);
    delay_ms(5);
    lcd_nibble(0x02);
    delay_ms(5);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    delay_ms(5);
}

void servo_0deg(void)
{
    SERVO = 1;
    delay_ms(1);
    SERVO = 0;
    delay_ms(19);
}

void servo_180deg(void)
{
    SERVO = 1;
    delay_ms(2);
    SERVO = 0;
    delay_ms(18);
}

void main(void)
{

    unsigned char temp;
    unsigned int loop;
    P2 = 0x00;
    lcd_init();
    uart_init();
	timer_init();

    timer2_init();

    // cuba ubah nama bt module
    uart_puts("AT+NAME" BTNAME "\r\n");
    for (loop = 0; loop < 60000; loop++)
        ;
    uart_puts("AT+PIN" BTPASS "\r\n");
    for (loop = 0; loop < 60000; loop++)
        ;

    lcd_cmd(0x80);
    lcd_str("PET FEEDER");
    lcd_cmd(0xC0);
    lcd_str("MASA MAKAN");

    while (1)
    {
        temp = uart_read();

        uart_puts("RX = ");
        uart_putchar(temp);
        uart_puts("\r\n");

        lcd_cmd(0x01);
        delay_ms(2);
        lcd_cmd(0x80); // first line

        lcd_str("RX = ");
        lcd_write(temp);

        // switch (temp)
        // {
        // case 'A':
        //     lcd_cmd(0x80);
        //     lcd_str("TEST 1");
        //     uart_puts("TEST 1");

        //     break;
        // case 'B':
        //     lcd_cmd(0x80);
        //     lcd_str("TEST 2");
        //     uart_puts("TEST 1");

        //     break;
        // }

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