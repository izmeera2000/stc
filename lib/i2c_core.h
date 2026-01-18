/*----------------------------------------------------------------------------*/
#ifndef __MY1I2C_CORE_H__
#define __MY1I2C_CORE_H__
/*----------------------------------------------------------------------------*/
/**
 * i2c_core.h
 * - core library for i2c (bit-bang)
 * - enable customizable read/write
**/
/*----------------------------------------------------------------------------*/
#include "my1mcu51.h"
/*----------------------------------------------------------------------------*/
/** allow code to override these pins! */
#ifndef I2C_SCL
#define I2C_SCL P3_6
#endif
#ifndef I2C_SDA
#define I2C_SDA P3_7
#endif
/*----------------------------------------------------------------------------*/
/** allow code to override this as well */
#ifndef I2C_WAIT
#define I2C_WAIT(tlen) i2c_wait(tlen)
#endif
#ifndef I2C_WAIT_TLEN
#define I2C_WAIT_TLEN 5
#endif
#define I2C_WAIT_HALF (I2C_WAIT_TLEN>>1)
/*----------------------------------------------------------------------------*/
void i2c_init(void) {
	I2C_SCL = 1; I2C_SDA = 1;
}
/*----------------------------------------------------------------------------*/
void i2c_wait(unsigned char ncnt) {
	do { NOP; } while(--ncnt);
}
/*----------------------------------------------------------------------------*/
/** write out start marker */
void i2c_do_start(void) {
	I2C_SCL = 1; I2C_SDA = 1;
	/* start condition setup time >4.7 us */
	I2C_WAIT(I2C_WAIT_TLEN);
	I2C_SDA = 0;
	/* start condition hold time > 4.0 us */
	I2C_WAIT(I2C_WAIT_TLEN);
	I2C_SCL = 0;
}
/*----------------------------------------------------------------------------*/
/** write out stop marker */
void i2c_do_stop(void) {
	I2C_SDA = 0; I2C_SCL = 1;
	/* stop condition setup time > 4.0 us */
	I2C_WAIT(I2C_WAIT_TLEN);
	I2C_SDA = 1;
	/* no hold time, but we let bus idle for some time */
	I2C_WAIT(I2C_WAIT_TLEN);
}
/*----------------------------------------------------------------------------*/
/** write 1 byte out - scl already low */
unsigned char i2c_do_send_byte(unsigned char bdat) {
	unsigned char mask;
	mask = 0x80;
	while (mask) {
		/* data transitions on scl low! */
		I2C_SDA = bdat&mask ? 1 : 0;
		/* data setup time is 0.25 us */
		I2C_SCL = 1;
		/* scl high time > 4.0 us (low time > 4.7 us) */
		I2C_WAIT(I2C_WAIT_TLEN);
		I2C_SCL = 0;
		I2C_WAIT(I2C_WAIT_HALF);
		//I2C_WAIT(I2C_WAIT_TLEN); /* can skip this? 'covered' by shift? */
		mask >>= 1;
	}
	I2C_SDA = 1; I2C_SCL = 1;
	I2C_WAIT(I2C_WAIT_HALF);
	/* check acknowledge bit */
	mask = I2C_SDA ? 1 : 0;
	I2C_SCL = 0;
	return mask; /* returns zero if acknowledged */
}
/*----------------------------------------------------------------------------*/
/** read 1 byte in */
unsigned char i2c_do_read_byte(unsigned char last) {
	unsigned char bdat, loop;
	I2C_SDA = 1; /* easier for external device to pull down */
	for (loop=0,bdat=0;loop<8;loop++) {
		I2C_SCL = 0;
		/* scl low time > 4.7 us (high time > 4.0 us) */
		I2C_WAIT(I2C_WAIT_TLEN);
		I2C_SCL = 1;
		I2C_WAIT(I2C_WAIT_HALF); /* do we need this? */
		bdat <<= 1;
		if (I2C_SDA) bdat |= 0x01;
	}
	I2C_SCL = 0;
	/* scl low to data valid time < 3.4us?? */
	if (!last) I2C_SDA = 0; /* send ack bit */
	I2C_SCL = 1;
	I2C_WAIT(I2C_WAIT_HALF);
	I2C_SCL = 0;
	return bdat;
}
/*----------------------------------------------------------------------------*/
#endif /** __MY1I2C_CORE_H__ */
/*----------------------------------------------------------------------------*/
