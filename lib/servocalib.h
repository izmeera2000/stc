#include "my1mcu51.h"
#include "uart.h"
#include "timer.h"

sbit SRV = P3^0;  // Servo control pin connected to P2.0

void delay_100us(unsigned char step) {
    do {
        timer_wait(0xffa4); // Delay of 0.1ms
    } while (--step);
}

void servo_rotate(unsigned char pulse_width, unsigned int duration_ms) {
    unsigned int loop_count = duration_ms / 10; // Calculate loop count for 10ms cycles
    unsigned int i;

    for (i = 0; i < loop_count; i++) { // Generate PWM for the specified duration
        SRV = 1;                         // Generate HIGH signal
        delay_100us(pulse_width);        // High duration
        SRV = 0;                         // Generate LOW signal
        delay_100us(100 - pulse_width);  // Low duration (to complete 10ms)
    }
}

void main(void) {
    unsigned char toggle = 0; // Toggle variable to alternate direction
    timer0_init();            // Initialize Timer 0
    uart_init();              // Initialize UART
    uart_puts("-- Servo Control with Continuous 180-Degree Alternating Turns Initialized!\r\n");
	
	// Initial 5-second delay before starting the servo motor
    uart_puts("## Waiting for 5 seconds before starting servo control\r\n");
    delay_100us(50); // Wait for 5 seconds (since each delay_100us step is 100us)

	while (1) {
        		    if (toggle == 0) {
				
                uart_puts("## Rotate 180 Degrees in One Direction\r\n");
                servo_rotate(25,1000); // Rotate in one direction for ~180 degrees
                toggle = 1;            // Toggle the direction
            } else {
                uart_puts("## Rotate 180 Degrees in the Opposite Direction\r\n");
                servo_rotate(5,20000); // Rotate in the opposite direction for ~180 degrees
                toggle = 0;            // Toggle back to the original direction
				uart_puts("## Rotate 180 Degrees in One Direction\r\n");
                servo_rotate(25,1000); // Rotate in one direction for ~180 degrees
                toggle = 1;
            }
    }
}
