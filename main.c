#include "inc/mcu51.h"
#include "lib/uart.h"

#define BTNAME "nmk322bt"
#define BTPASS "0123"
#define MAX_JADUAL 5

sbit SERVO = P1 ^ 1;
sbit RS = P2 ^ 0;
sbit E = P2 ^ 1;
sbit D4 = P2 ^ 2;
sbit D5 = P2 ^ 3;
sbit D6 = P2 ^ 4;
sbit D7 = P2 ^ 5;

#define DAY_MON (1 << 0)
#define DAY_TUE (1 << 1)
#define DAY_WED (1 << 2)
#define DAY_THU (1 << 3)
#define DAY_FRI (1 << 4)
#define DAY_SAT (1 << 5)
#define DAY_SUN (1 << 6)
#define DAY_ALL 0x7F

struct Schedule
{
  unsigned char h;
  unsigned char m;
  unsigned char days; // Day mask
  unsigned char a;    // active flag
};

struct Schedule jadwal[MAX_JADUAL] = {{8, 0, DAY_ALL, 1},
                                      {13, 30, DAY_MON | DAY_WED | DAY_FRI, 1},
                                      {20, 0, DAY_ALL, 1},
                                      {0, 0, 0, 0},
                                      {0, 0, 0, 0}};

unsigned char total_jadual = 3;
unsigned char hour = 0, mmin = 0, ssec = 0;
unsigned char step_counter;
bit update_lcd = 0;
bit is_time_set = 0;
unsigned char day_of_week = 0;
bit fed_this_minute = 0;

char rx_buffer[12];
unsigned char rx_index = 0;

// --- DELAYS ---
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

// --- UART ---
void uart_write(char c)
{
  SBUF = c;
  while (TI == 0)
    ;
  TI = 0;
}

// --- SERVO ---
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

void lcd_str(char *s)
{
  while (*s)
    lcd_write(*s++);
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

// --- SHOW TIME AND DAY ---
void lcd_show_time(void)
{
    /* Line 1: Time */
    lcd_cmd(0x80);
    lcd_str("TIME: ");

    lcd_write((hour / 10) + '0');
    lcd_write((hour % 10) + '0');
    lcd_write(':');

    lcd_write((mmin / 10) + '0');
    lcd_write((mmin % 10) + '0');
    lcd_write(':');

    lcd_write((ssec / 10) + '0');
    lcd_write((ssec % 10) + '0');

    lcd_str("  ");   // clear leftover chars

    /* Line 2: Day */
    lcd_cmd(0xC0);
    lcd_str("DAY: ");

    lcd_write(day_of_week + '0');
    lcd_str("   ");  // clear leftovers
}


void uart_show_time(void)
{
  uart_puts("Time: ");

  uart_write((hour / 10) + '0');
  uart_write((hour % 10) + '0');
  uart_write(':');

  uart_write((mmin / 10) + '0');
  uart_write((mmin % 10) + '0');
  uart_write(':');

  uart_write((ssec / 10) + '0');
  uart_write((ssec % 10) + '0');

  uart_puts("  Day: ");
  uart_write(day_of_week + '0');
  uart_puts("\r\n");
}

// --- TIMER0 ---
void timer0_init(void)
{
  TMOD |= 0x01;
  TH0 = 0x4C;
  TL0 = 0x00;
  ET0 = 1;
  EA = 1;
  TR0 = 1;
}

void timer0_isr(void) interrupt 1
{
  TH0 = 0x4C;
  TL0 = 0x00;

  if (!is_time_set)
    return;

  step_counter++;
  if (step_counter >= 20)
  {
    step_counter = 0;
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

// --- BLUETOOTH ---
void bluetooth_init(void)
{
  unsigned int loop;
  uart_puts("AT+NAME" BTNAME "\r\n");
  for (loop = 0; loop < 60000; loop++)
    ;
  uart_puts("AT+PIN" BTPASS "\r\n");
  for (loop = 0; loop < 60000; loop++)
    ;
}

// --- STARTUP ---
void lcd_startup_screen(void)
{
  lcd_cmd(0x80);
  lcd_str("PET FEEDER      ");
  lcd_cmd(0xC0);
  lcd_str("START...        ");
  delay_ms(2500);

  lcd_cmd(0x80);
  lcd_str("PLEASE CONNECT  ");
  lcd_cmd(0xC0);
  lcd_str("BLUETOOTH...    ");
  delay_ms(2500);

  lcd_cmd(0x01);
  lcd_cmd(0x80);
  lcd_str("  SET TIME NOW:   ");
  lcd_cmd(0xC0);
  lcd_str("      24Hrs       ");
}

void system_startup(void)
{
  uart_init();
  timer0_init();
  lcd_init();
  servo_tutup();
  bluetooth_init();
  lcd_startup_screen();
}

// --- AUTO FEED ---
void auto_feed_task(void)
{
  unsigned char i;
  if (!is_time_set || !update_lcd)
    return;

  update_lcd = 0;
  lcd_show_time();
  fed_this_minute = 0;

  if (ssec == 1)
    fed_this_minute = 0;

  if (ssec != 0 || fed_this_minute)
    return;

  fed_this_minute = 1;

  for (i = 0; i < total_jadual; i++)
  {
    if (jadwal[i].a && hour == jadwal[i].h && mmin == jadwal[i].m &&
        (jadwal[i].days & (1 << day_of_week)))
    {
      lcd_cmd(0xC0);
      lcd_str(" AUTO: MAKAN!    ");
      servo_buka();
      servo_tutup();
      uart_puts("Feed Done\r\n");
      lcd_cmd(0xC0);
      lcd_str(" AUTO: SELESAI   ");
    }
  }
}

// --- MANUAL FEED ---
void bt_manual_feed(void)
{
  lcd_cmd(0xC0);
  lcd_str("  MANUAL FEED...  ");
  servo_buka();
  servo_tutup();
  uart_puts("Feed Done\r\n");
  lcd_cmd(0xC0);
  lcd_str("  STATUS: READY   ");
}

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

  // Accept digits only after 't'
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
      uart_puts("TIME ERR\r\n");
      rx_index = 0;
      return;
    }

    is_time_set = 1;

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("TIME UPDATED!");
    uart_puts("TIME OK\r\n");

    delay_ms(800);
    lcd_cmd(0x01);

    rx_index = 0;
  }
}
// --- BT COMMAND HANDLER ---
void bt_command_task(char c)
{
  unsigned char i,d;
  char str_time[6];

  if (c == 'm' || c == 'M')
  {
    bt_manual_feed();
    return;
  }

  if (c == 'l' || c == 'L')
  {
    // lcd_show_time();
    uart_show_time();
    return;
  }

  if (c == 't' || c == 'T')
  {
    bt_set_time(c);
  }

  if (c == 's' || c == 'S')
  {

    uart_puts("Active Schedule: ");
    uart_write(total_jadual + '0');
    uart_puts("\r\n");

    for (i = 0; i < total_jadual; i++)
    {
      uart_write(i + 1 + '0');
      uart_puts(" [");

      str_time[0] = (jadwal[i].h / 10) + '0';
      str_time[1] = (jadwal[i].h % 10) + '0';
      str_time[2] = ':';
      str_time[3] = (jadwal[i].m / 10) + '0';
      str_time[4] = (jadwal[i].m % 10) + '0';
      str_time[5] = '\0';

      uart_puts(str_time);
      uart_puts("] ");

      // 🔹 SHOW DAYS HERE
      uart_puts("(");
       d = jadwal[i].days;
      uart_write((d / 100) + '0');        
      uart_write(((d / 10) % 10) + '0');  
      uart_write((d % 10) + '0');         

      uart_puts(") - ");

      if (hour > jadwal[i].h || (hour == jadwal[i].h && mmin >= jadwal[i].m))
        uart_puts("Done\r\n");
      else
        uart_puts("Not Yet\r\n");
    }
    return;
  }

  if (c == 'f' || c == 'F' || c == 'c' || c == 'C')
  {
    rx_index = 0;
    rx_buffer[rx_index++] = c;
    return;
  }

  if (rx_index == 0)
    return;

  rx_buffer[rx_index++] = c;

  if ((rx_buffer[0] == 'f' || rx_buffer[0] == 'F') && rx_index == 9)
  {
    unsigned char n = rx_buffer[1] - '0';

    if (n >= 1 && n <= MAX_JADUAL)
    {
      jadwal[n - 1].h = (rx_buffer[2] - '0') * 10 + (rx_buffer[3] - '0');

      jadwal[n - 1].m = (rx_buffer[4] - '0') * 10 + (rx_buffer[5] - '0');

      jadwal[n - 1].days = (rx_buffer[6] - '0') * 100 +
                           (rx_buffer[7] - '0') * 10 + (rx_buffer[8] - '0');
      if (jadwal[n - 1].days > DAY_ALL)
        jadwal[n - 1].days = DAY_ALL;

      jadwal[n - 1].a = 1;

      lcd_cmd(0xC0);
      lcd_str("  SCHEDULE SET!   ");
      uart_puts("Schedule Set OK\r\n");

      delay_ms(1000);
    }
    rx_index = 0;
  }

  // SET TOTAL COUNT
  else if ((rx_buffer[0] == 'c' || rx_buffer[0] == 'C') && rx_index == 2)
  {
    unsigned char new_count = rx_buffer[1] - '0';
    if (new_count >= 1 && new_count <= MAX_JADUAL)
    {
      total_jadual = new_count;
      uart_puts("Total Schedule: ");
      uart_write(total_jadual + '0');
      uart_puts("\r\n");
    }
    rx_index = 0;
  }
}





// --- MAIN ---
void main(void)
{
  char rx;
  system_startup();

  while (1)
  {
    auto_feed_task();

    if (RI)
    {
      rx = uart_read();

      if (!is_time_set)
        bt_set_time(rx);
      else
        bt_command_task(rx);
    }
  }
}
