/*----------------------------------------------------------------------------*/
#ifndef __UART_HEXASCII_H__
#define __UART_HEXASCII_H__
/*----------------------------------------------------------------------------*/
/**
 * uart_hexascii.h
 * - extension library for uart_send.h to transmit byte(s) in ASCII
 * - define HEXASCII_ALLCAPS to use capital letters A-F
**/
/*----------------------------------------------------------------------------*/
#include "mcu51.h"
#include "uart_tx.h"
/*----------------------------------------------------------------------------*/
void uart_send_hexbyte(unsigned char byte) {
	uart_send(nibb_hi2ascii(byte));
	uart_send(nibb_lo2ascii(byte));
}
/*----------------------------------------------------------------------------*/
void uart_send_hexuint(unsigned int word) {
	uart_send_hexbyte((unsigned char)(word>>8)&0xff);
	uart_send_hexbyte((unsigned char)word&0xff);
}
/*----------------------------------------------------------------------------*/
void uart_send_hexlong(unsigned long word) {
	uart_send_hexuint((unsigned int)(word>>16)&0xffff);
	uart_send_hexuint((unsigned int)word&0xffff);
}
/*----------------------------------------------------------------------------*/
#endif /** __UART_HEXASCII_H__ */
/*----------------------------------------------------------------------------*/
