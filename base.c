#include "mcu51.h"
#include "uart.h"

#define BTNAME "nmk322bt"
#define BTPASS "0123"
#define MAX_JADUAL 5 

sbit SERVO = P1^1; 
sbit RS = P2^0; 
sbit E = P2^1;
sbit D4 = P2^2; 
sbit D5 = P2^3; 
sbit D6 = P2^4; 
sbit D7 = P2^5;

// --- STRUKTUR JADUAL ---
struct Schedule { unsigned char h; unsigned char m; };

struct Schedule jadwal[MAX_JADUAL] = {
    {8, 00},  
    {13, 30}, 
    {20, 00}, 
    {0, 0},   
    {0, 0}    
};

unsigned char total_jadual = 3; 
unsigned char hour = 0, mmin = 0, ssec = 0;
unsigned char step_counter;
bit update_lcd = 0;
bit is_time_set = 0; 

char rx_buffer[10];
unsigned char rx_index = 0;

void delay_ms(unsigned int ms) {
    unsigned int i,j;
    for(i=0;i<ms;i++) for(j=0;j<1200;j++);
}

void delay_us(unsigned int us) {
    unsigned int i; for(i=0;i<us;i++);
}

// --- FUNGSI UART WRITE (INI YANG TERTINGGAL TADI) ---
void uart_write(char c) {
    SBUF = c;
    while(TI==0); // Tunggu hantar selesai
    TI=0;         // Clear flag
}
// ----------------------------------------------------

void servo_buka(void) {
    unsigned char i;
    ET0 = 0; 
    for(i=0; i<15; i++) { 
        SERVO = 1; delay_ms(2); SERVO = 0; delay_ms(18); 
    }
    ET0 = 1; 
}

void servo_tutup(void) {
    unsigned char i;
    ET0 = 0; 
    for(i=0; i<15; i++) { 
        SERVO = 1; delay_ms(1); SERVO = 0; delay_ms(19); 
    }
    ET0 = 1; 
}

void lcd_pulse(void) { E=1; delay_us(50); E=0; delay_us(50); }
void lcd_nibble(unsigned char n) { D4=n&1; D5=(n>>1)&1; D6=(n>>2)&1; D7=(n>>3)&1; lcd_pulse(); }
void lcd_cmd(unsigned char c) { RS=0; lcd_nibble(c>>4); lcd_nibble(c&0x0F); delay_ms(2); }
void lcd_write(unsigned char c) { RS=1; lcd_nibble(c>>4); lcd_nibble(c&0x0F); delay_ms(1); }
void lcd_str(char *s) { while(*s) lcd_write(*s++); }

void lcd_init(void) {
    delay_ms(50); RS=0; lcd_nibble(3); delay_ms(5); lcd_nibble(3); delay_ms(5);
    lcd_nibble(3); delay_ms(5); lcd_nibble(2); delay_ms(5);
    lcd_cmd(0x28); lcd_cmd(0x0C); lcd_cmd(0x01); delay_ms(5);
}

void lcd_show_time(void) {
    lcd_cmd(0x80); lcd_str("TIME: ");
    lcd_write((hour/10)+'0'); lcd_write((hour%10)+'0'); lcd_write(':');
    lcd_write((mmin/10)+'0'); lcd_write((mmin%10)+'0'); lcd_write(':');
    lcd_write((ssec/10)+'0'); lcd_write((ssec%10)+'0'); lcd_str("  ");
}

void timer0_init(void) {
    TMOD |= 0x01; TH0 = 0x4C; TL0 = 0x00; ET0 = 1; EA = 1; TR0 = 1;
}

void timer0_isr(void) interrupt 1 {
    TH0 = 0x4C; TL0 = 0x00;
    if(is_time_set == 1) {
        step_counter++;
        if(step_counter >= 20) {
            step_counter = 0; ssec++; update_lcd = 1;
            if(ssec >= 60) { 
                ssec = 0; mmin++; 
                if(mmin >= 60) { mmin = 0; hour++; if(hour >= 24) hour = 0; }
            }
        }
    }
}

void main(void) {
    unsigned int loop;
    unsigned char i; 
    char temp_char;
    char str_time[6]; 
    
    uart_init();
    timer0_init(); 
    
    uart_puts("AT+NAME" BTNAME "\r\n"); for(loop=0;loop<60000;loop++);
    uart_puts("AT+PIN" BTPASS "\r\n");  for(loop=0;loop<60000;loop++);

    lcd_init();
    servo_tutup(); 
    
    lcd_cmd(0x80); lcd_str("PET FEEDER      ");
    lcd_cmd(0xC0); lcd_str("START...        ");
    delay_ms(2500); 

    lcd_cmd(0x80); lcd_str("PLEASE CONNECT  ");
    lcd_cmd(0xC0); lcd_str("BLUETOOTH...    ");
    delay_ms(2500); 

    lcd_cmd(0x01); 
    lcd_cmd(0x80); lcd_str("  SET TIME NOW:   ");
    lcd_cmd(0xC0); lcd_str("      24Hrs       "); 

    while(1) {
        
        // --- LOGIK AUTO FEED ---
        if(is_time_set == 1 && update_lcd) {
            update_lcd = 0;
            lcd_show_time();
            
            if(ssec == 0) { 
                for(i=0; i<total_jadual; i++) {
                    if (hour == jadwal[i].h && mmin == jadwal[i].m) {
                        lcd_cmd(0xC0); lcd_str(" AUTO: MAKAN!    ");
                        servo_buka(); 
                        servo_tutup();
                        uart_puts("Feed Done\r\n");
                        lcd_cmd(0xC0); lcd_str(" AUTO: SELESAI   ");
                    }
                }
            }
        }

        if (RI) {
            temp_char = uart_read(); 
            
            if (is_time_set == 0) {
                if (temp_char == 't' || temp_char == 'T') {
                    rx_index = 0; rx_buffer[rx_index++] = temp_char; 
                }
                else if (rx_index > 0) {
                    rx_buffer[rx_index++] = temp_char;
                    if (rx_index == 7) { 
                        hour = (rx_buffer[1]-'0')*10 + (rx_buffer[2]-'0');
                        mmin = (rx_buffer[3]-'0')*10 + (rx_buffer[4]-'0');
                        ssec = (rx_buffer[5]-'0')*10 + (rx_buffer[6]-'0');
                        is_time_set = 1; 
                        lcd_cmd(0x01); lcd_cmd(0x80); lcd_str("TIME UPDATED!");
                        delay_ms(1000); lcd_cmd(0x01);
                        rx_index = 0; 
                    }
                }
            }
            else {
                if (temp_char == 'm' || temp_char == 'M') {
                    lcd_cmd(0xC0); lcd_str("  MANUAL FEED...  ");
                    servo_buka(); servo_tutup();
                    uart_puts("Feed Done\r\n");
                    lcd_cmd(0xC0); lcd_str("  STATUS: READY   ");
                }
                
                // --- STATUS PAPARAN FLEXIBLE ---
                else if (temp_char == 's' || temp_char == 'S') {
                    uart_puts("Active Schedule: ");
                    uart_write(total_jadual + '0'); // Error fixed here
                    uart_puts("\r\n");
                    
                    for(i=0; i<total_jadual; i++) {
                        uart_write(i + 1 + '0');    // Error fixed here
                        uart_puts(" [");
                        
                        str_time[0] = (jadwal[i].h / 10) + '0';
                        str_time[1] = (jadwal[i].h % 10) + '0';
                        str_time[2] = ':';
                        str_time[3] = (jadwal[i].m / 10) + '0';
                        str_time[4] = (jadwal[i].m % 10) + '0';
                        str_time[5] = '\0';
                        uart_puts(str_time);
                        
                        uart_puts("] - ");
                        
                        if(hour > jadwal[i].h || (hour == jadwal[i].h && mmin >= jadwal[i].m)) 
                            uart_puts("Done\r\n");
                        else 
                            uart_puts("Not Yet\r\n");
                    }
                }

                // --- DETECT COMMAND ---
                else if (temp_char == 't' || temp_char == 'T' || 
                         temp_char == 'f' || temp_char == 'F' || 
                         temp_char == 'c' || temp_char == 'C') {
                     rx_index = 0; rx_buffer[rx_index++] = temp_char; 
                }
                
                else if (rx_index > 0) {
                     rx_buffer[rx_index++] = temp_char;
                     
                     // --- SET TIME ---
                     if ( (rx_buffer[0]=='t' || rx_buffer[0]=='T') && rx_index == 7) {
                        hour = (rx_buffer[1]-'0')*10 + (rx_buffer[2]-'0');
                        mmin = (rx_buffer[3]-'0')*10 + (rx_buffer[4]-'0');
                        ssec = (rx_buffer[5]-'0')*10 + (rx_buffer[6]-'0');
                        uart_puts("TIME OK\r\n");
                        rx_index = 0;
                     }
                     
                     // --- SET JADUAL ---
                     else if ( (rx_buffer[0]=='f' || rx_buffer[0]=='F') && rx_index == 6) {
                        unsigned char alarm_num = rx_buffer[1] - '0';
                        
                        if(alarm_num >= 1 && alarm_num <= MAX_JADUAL) {
                            unsigned char h = (rx_buffer[2]-'0')*10 + (rx_buffer[3]-'0');
                            unsigned char m = (rx_buffer[4]-'0')*10 + (rx_buffer[5]-'0');
                            
                            jadwal[alarm_num-1].h = h;
                            jadwal[alarm_num-1].m = m;
                            
                            lcd_cmd(0xC0); lcd_str("  SCHEDULE SET!   ");
                            delay_ms(1000); 
                            lcd_cmd(0xC0); lcd_str("                  "); 
                        }
                        rx_index = 0;
                     }

                     // --- UBAH JUMLAH JADUAL (CN) ---
                     else if ( (rx_buffer[0]=='c' || rx_buffer[0]=='C') && rx_index == 2) {
                        unsigned char new_count = rx_buffer[1] - '0';
                        
                        if(new_count >= 1 && new_count <= MAX_JADUAL) {
                            total_jadual = new_count;
                            uart_puts("Total Schedule: ");
                            uart_write(total_jadual + '0'); // Error fixed here
                            uart_puts("\r\n");
                            
                            lcd_cmd(0xC0); lcd_str("  SCHD COUNT:   ");
                            lcd_write(total_jadual + '0');
                            delay_ms(1000);
                            lcd_cmd(0xC0); lcd_str("                  ");
                        }
                        rx_index = 0;
                     }
                }
            }
        }
    }
}