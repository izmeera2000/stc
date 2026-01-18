/*----------------------------------------------------------------------------*/
#ifndef __MY1SPI_H__
#define __MY1SPI_H__
/*----------------------------------------------------------------------------*/
/**
 * spi.h
 * - library for spi (bit-bang)
**/
/*----------------------------------------------------------------------------*/
#include "my1mcu51.h"
/*----------------------------------------------------------------------------*/
/** allow code to override these pins! */
#ifndef SPI_SCLK
#define SPI_SCLK P2_4
#endif
#ifndef SPI_MOSI
#define SPI_MOSI P2_5
#endif
#ifndef SPI_MISO
#define SPI_MISO P2_6
#endif
#ifndef SPI_CENB
#define SPI_CENB P2_7
#endif
/*----------------------------------------------------------------------------*/
#define spi_enable() SPI_CENB = 0
#define spi_disable() SPI_CENB = 1
/*----------------------------------------------------------------------------*/
unsigned char spi_transfer(unsigned char sdat) {
	unsigned char loop, read;
	for (loop=0,read=0;loop<8;loop++) {
		read <<= 1;
		/* msb first! */
		if (sdat&0x80) SPI_MOSI = 1;
		else SPI_MOSI = 0;
		read |= SPI_MISO;
		SPI_SCLK = 1;
		SPI_SCLK = 0;
		sdat <<= 1;
	}
	return read;
}
/*----------------------------------------------------------------------------*/
void spi_init(void) {
	SPI_SCLK = 0;
	SPI_MOSI = 0;
	SPI_MISO = 1;
	SPI_CENB = 1;
}
/*----------------------------------------------------------------------------*/
#endif /** __MY1SPI_H__ */
/*----------------------------------------------------------------------------*/
