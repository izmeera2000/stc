/*----------------------------------------------------------------------------*/
#ifndef __MY1I2C_H__
#define __MY1I2C_H__
/*----------------------------------------------------------------------------*/
/**
 * i2c.h
 * - library for i2c (bit-bang)
 * - extends i2c_core
 * - provides i2c_putb and i2c_getb
 * - by default, assume actual address (needs <<1)
 *   = define I2C_CORE_ADDRESS_SHIFTED to skip
**/
/*----------------------------------------------------------------------------*/
#ifndef I2C_CORE_ADDRESS_SHIFTED
#ifndef I2C_CORE_ADDRESS_ACTUAL
#define I2C_CORE_ADDRESS_ACTUAL
#endif
#else
#ifdef I2C_CORE_ADDRESS_ACTUAL
#undef I2C_CORE_ADDRESS_ACTUAL
#endif
#endif
/*----------------------------------------------------------------------------*/
#include "i2c_core.h"
/*----------------------------------------------------------------------------*/
/** send multiple bytes */
unsigned char i2c_putb(unsigned char addr, unsigned char regs,
	unsigned char* pdat, unsigned char size) {
	unsigned char loop, test;
	test = 0;
#ifdef I2C_CORE_ADDRESS_ACTUAL
	addr <<= 1;
#endif
	i2c_do_start();
	test |= i2c_do_send_byte(addr);
	test |= i2c_do_send_byte(regs);
	for (loop=0;loop<size;loop++)
		test |= i2c_do_send_byte(pdat[loop]);
	i2c_do_stop();
	return test;
}
/*----------------------------------------------------------------------------*/
/** read multiple bytes */
unsigned char i2c_getb(unsigned char addr, unsigned char regs,
	unsigned char* pdat, unsigned char size) {
	unsigned char loop, test;
	test = 0;
#ifdef I2C_CORE_ADDRESS_ACTUAL
	addr <<= 1;
#endif
	i2c_do_start();
	test |= i2c_do_send_byte(addr);
	test |= i2c_do_send_byte(regs);
	i2c_do_start(); /* send a restart for read */
	test |= i2c_do_send_byte(addr|0x01); /* activate read bit */
	for (loop=0;loop<size-1;loop++)
		pdat[loop] = (unsigned char)i2c_do_read_byte(0);
	pdat[loop] = i2c_do_read_byte(1);
	i2c_do_stop();
	return test;
}
/*----------------------------------------------------------------------------*/
#endif /** __MY1I2C_H__ */
/*----------------------------------------------------------------------------*/
