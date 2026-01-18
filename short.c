#include "inc/mcu51.h"
#include "lib/uart.h"

#define MAX_JADUAL 3

sbit SERVO = P1 ^ 1;
sbit RS = P2 ^ 0;
sbit E = P2 ^ 1;
sbit D4 = P2 ^ 2;
sbit D5 = P2 ^ 3;
sbit D6 = P2 ^ 4;
sbit D7 = P2 ^ 5;

// --- Schedule ---
struct Schedule
{
    unsigned char h, m, days, a;
};
struct Schedule jadwal[MAX_JADUAL] = {{8, 0, 0x7F, 1}, {13, 30, 0x15, 1}, {20, 0, 0x7F, 1}};
unsigned char total_jadual = 3;

// --- Time ---
unsigned char hour = 0, mmin = 0, ssec = 0, step = 0, day_of_week = 0;
bit update_lcd = 0, is_time_set = 0, fed_min = 0;

// --- UART ---
char rx_buffer[8];
unsigned char rx_index = 0;

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

void uart_write(char c)
{
    SBUF = c;
    while (!TI)
        ;
    TI = 0;
}
void uart_puts(char *s)
{
    while (*s)
        uart_write(*s++);
}

// --- Servo ---
void servo_buka(void)
{
    unsigned char i;
    ET0 = 0;
    for (i = 0; i < 15; i++)
    {
        SERVO = 1;
        delay_ms(2);
        SERVO = 0;
        delay_ms(18);
    }
    ET0 = 1;
}
void servo_tutup(void)
{
    unsigned char i;
    ET0 = 0;
    for (i = 0; i < 15; i++)
    {
        SERVO = 1;
        delay_ms(1);
        SERVO = 0;
        delay_ms(19);
    }
    ET0 = 1;
}

// --- LCD ---
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
void lcd_init(void)
{
    delay_ms(50);
    RS = 0;
    lcd_nibble(3);
    delay_ms(5);
    lcd_nibble(3);
    delay_ms(5);
    lcd_nibble(3);
    delay_ms(5);
    lcd_nibble(2);
    delay_ms(5);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    delay_ms(5);
}
void lcd_show_time(void)
{
    lcd_cmd(0x80);
    lcd_write((hour / 10) + '0');
    lcd_write((hour % 10) + '0');
    lcd_write((mmin / 10) + '0');
    lcd_write((mmin % 10) + '0');
    lcd_write((ssec / 10) + '0');
    lcd_write((ssec % 10) + '0');
    lcd_cmd(0xC0);
    lcd_write(day_of_week + '0');
}

// --- UART show ---
void uart_show_time(void)
{
    uart_write((hour / 10) + '0');
    uart_write((hour % 10) + '0');
    uart_write((mmin / 10) + '0');
    uart_write((mmin % 10) + '0');
    uart_write((ssec / 10) + '0');
    uart_write((ssec % 10) + '0');
    uart_write(day_of_week + '0');
    uart_write('\n');
}

// --- TIMER0 ---
void timer0_init(void)
{
    TMOD |= 0x01;
    TH0 = 0x4C;
    TL0 = 0;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}
void timer0_isr(void) interrupt 1
{
    TH0 = 0x4C;
    TL0 = 0;
    if (!is_time_set)
        return;
    step++;
    if (step >= 20)
    {
        step = 0;
        ssec++;
        update_lcd = 1;
        if (ssec >= 60)
        {
            ssec = 0;
            mmin++;
            if (mmin >= 60)
            {
                mmin = 0;
                hour++;
                if (hour >= 24)
                {
                    hour = 0;
                    day_of_week++;
                    if (day_of_week >= 7)
                        day_of_week = 0;
                }
            }
        }
    }
}

// --- Auto feed ---
void auto_feed_task(void)
{
    unsigned char i;
    if (!is_time_set || !update_lcd)
        return;
    update_lcd = 0;
    lcd_show_time();
    if (ssec != 0 || fed_min)
        return;
    fed_min = 1;
    for (i = 0; i < total_jadual; i++)
    {
        if (jadwal[i].a && hour == jadwal[i].h && mmin == jadwal[i].m && (jadwal[i].days & (1 << day_of_week)))
        {
            servo_buka();
            servo_tutup();
        }
    }
}

// --- Manual feed ---
void bt_manual_feed(void)
{
    servo_buka();
    servo_tutup();
}

// --- Set time via BT ---
void bt_set_time(char c)
{
    if (c == 't' || c == 'T')
    {
        rx_index = 0;
        rx_buffer[rx_index++] = c;
        return;
    }
    if (rx_index == 0)
        return;
    if (rx_index >= sizeof(rx_buffer))
    {
        rx_index = 0;
        return;
    }
    if (rx_index > 1)
    {
        if (c < '0' || c > '9')
        {
            rx_index = 0;
            return;
        }
    }
    rx_buffer[rx_index++] = c;
    if (rx_index == 8)
    {
        hour = (rx_buffer[1] - '0') * 10 + (rx_buffer[2] - '0');
        mmin = (rx_buffer[3] - '0') * 10 + (rx_buffer[4] - '0');
        ssec = (rx_buffer[5] - '0') * 10 + (rx_buffer[6] - '0');
        day_of_week = rx_buffer[7] - '0';
        if (hour > 23 || mmin > 59 || ssec > 59 || day_of_week > 6)
        {
            rx_index = 0;
            return;
        }
        is_time_set = 1;
        rx_index = 0;
    }
}

// --- BT Command ---
void bt_command_task(char c)
{
    if (c == 'm' || c == 'M')
    {
        bt_manual_feed();
        return;
    }
    if (c == 'l' || c == 'L')
    {
        uart_show_time();
        return;
    }
    if (c == 't' || c == 'T')
    {
        bt_set_time(c);
        return;
    }
}

// --- Startup ---
void system_startup(void)
{
    uart_init();
    timer0_init();
    lcd_init();
    servo_tutup();
}

void main(void)
{
    char rx;
    system_startup();
    while (1)
    {
        auto_feed_task();
        if (RI)
        {
            rx = SBUF;
            RI = 0;
            if (!is_time_set)
                bt_set_time(rx);
            else
                bt_command_task(rx);
        }
    }
}
