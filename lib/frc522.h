/*----------------------------------------------------------------------------*/
#ifndef __FRC522_H__
#define __FRC522_H__
/*----------------------------------------------------------------------------*/
/**
 * frc522.h
 * - library for mfrc522 contactless reader ic
 * - requires spi, loop_delay
 * - note: use 3.3v supply, io is 5v tolerant
RFID-RC522 pins:
- SDA (SPI_CENB) -> 2.7 - kuning
- SCK (SPI_SCLK) -> 2.4 - coklat
- MOSI (SPI_MOSI) -> 2.5 -hijau
- MISO (SPI_MISO) -> 2.6 - putih
- IRQ
- GND (GND) -> GND - ungu
- RST
- 3.3V (3.3V) -> 3V3 - putih
**/
/*----------------------------------------------------------------------------*/
#include "spi.h"
#include "loop_delay.h"
/*----------------------------------------------------------------------------*/
#define FRC522_MAX_RXSIZE 16
/*----------------------------------------------------------------------------*/
#define FRC522_OK 0
#define FRC522_ERROR_GENERAL 1
#define FRC522_ERROR_NO_TAG 2
#define FRC522_ERROR_TIMEOUT 3
#define FRC522_ERROR_TXRX 4
#define FRC522_ERROR_CHECKSUM 5
#define FRC522_ERROR_REQ_A 10
/*----------------------------------------------------------------------------*/
/** frc522 commands */
#define FRC522_IDLE         0x00
#define FRC522_MEM25BUFF    0x01
#define FRC522_GENERATE_ID  0x02
#define FRC522_CALC_CRC     0x03
#define FRC522_TRANSMIT     0x04
#define FRC522_NO_CMD_CHG   0x07
#define FRC522_RECEIVE      0x08
#define FRC522_TRANSCEIVE   0x0C
#define FRC522_RESERVED     0x0D
#define FRC522_MF_AUTHENT   0x0E
#define FRC522_SOFT_RESET   0x0F
/*----------------------------------------------------------------------------*/
/** mifare 1 tag commands */
#define MF1_REQIDL    0x26 /* find the antenna area - REQuest A */
#define MF1_REQALL    0x52 /* find all the tags antenna area- WakeUP A */
#define MF1_ANTICOLL  0x93 /* anti-collision/select, cascade level 1 */
#define MF1_AUTHENT1A 0x60 /* authentication key A */
#define MF1_AUTHENT1B 0x61 /* authentication key B */
#define MF1_READ      0x30 /* read Block */
#define MF1_WRITE     0xA0 /* write block */
#define MF1_DECREMENT 0xC0 /* debit */
#define MF1_INCREMENT 0xC1 /* recharge */
#define MF1_RESTORE   0xC2 /* transfer block data to the buffer */
#define MF1_TRANSFER  0xB0 /* save the data in the buffer */
#define MF1_HALT      0x50 /* sleep */
/*----------------------------------------------------------------------------*/
/** page 0 regs: command & status */
#define FRC522_P0_RESERVED0 0x00
#define FRC522_P0_COMMAND_REG 0x01
#define FRC522_P0_COMIEN_REG 0x02
#define FRC522_P0_DIVIEN_REG 0x03
#define FRC522_P0_COMIRQ_REG 0x04
#define FRC522_P0_DIVIRQ_REG 0x05
#define FRC522_P0_ERROR_REG 0x06
#define FRC522_P0_STATUS1_REG 0x07
#define FRC522_P0_STATUS2_REG 0x08
#define FRC522_P0_FIFODATA_REG 0x09
#define FRC522_P0_FIFOLVL_REG 0x0A
#define FRC522_P0_FLOWLVL_REG 0x0B
#define FRC522_P0_CONTROL_REG 0x0C
#define FRC522_P0_BITFRAME_REG 0x0D
#define FRC522_P0_COLL_REG 0x0E
#define FRC522_P0_RESERVED1 0x0F
/** page 1 regs: command */
#define FRC522_P1_RESERVED0 0x10
#define FRC522_P1_MODE_REG 0x11
#define FRC522_P1_TX_MODE_REG 0x12
#define FRC522_P1_RX_MODE_REG 0x13
#define FRC522_P1_TX_CNTL_REG 0x14
#define FRC522_P1_TX_ASK_REG 0x15
#define FRC522_P1_TX_SEL_REG 0x16
#define FRC522_P1_RX_SEL_REG 0x17
#define FRC522_P1_RX_THRESH_REG 0x18
#define FRC522_P1_DEMOD_REG 0x19
#define FRC522_P1_RESERVED1 0x1A
#define FRC522_P1_RESERVED2 0x1B
#define FRC522_P1_MF_TX_REG 0x1C
#define FRC522_P1_MF_RX_REG 0x1D
#define FRC522_P1_RESERVED3 0x1E
#define FRC522_P1_UARTSPEED_REG 0x1F
/** page 2 regs: config */
#define FRC522_P2_RESERVED0 0x20
#define FRC522_P2_CRCRESH_REG 0x21
#define FRC522_P2_CRCRESL_REG 0x22
#define FRC522_P2_RESERVED1 0x23
#define FRC522_P2_MODWIDTH_REG 0x24
#define FRC522_P2_RESERVED2 0x25
#define FRC522_P2_RFCFG_REG 0x26
#define FRC522_P2_GSN_REG 0x27
#define FRC522_P2_CWGS_REG 0x28
#define FRC522_P2_MODGS_REG 0x29
#define FRC522_P2_TMODE_REG 0x2A
#define FRC522_P2_TPRESCALER_REG 0x2B
#define FRC522_P2_TRELOADH_REG 0x2C
#define FRC522_P2_TRELOADL_REG 0x2D
#define FRC522_P2_TCOUNTH_REG 0x2E
#define FRC522_P2_TCOUNTL_REG 0x2F
/** page 3 regs: test regs */
#define FRC522_P3_RESERVED0 0x30
#define FRC522_P3_TESTSEL1_REG 0x31
#define FRC522_P3_TESTSEL2_REG 0x32
#define FRC522_P3_TESTPIN_EN_REG 0x33
#define FRC522_P3_TESTPIN_VAL_REG 0x34
#define FRC522_P3_TESTBUS_REG 0x35
#define FRC522_P3_AUTOTEST_REG 0x36
#define FRC522_P3_VERSION_REG 0x37
#define FRC522_P3_ANALOGTEST_REG 0x38
#define FRC522_P3_TESTDAC1_REG 0x39
#define FRC522_P3_TESTDAC2_REG 0x3A
#define FRC522_P3_TESTADC_REG 0x3B
#define FRC522_P3_RESERVED1 0x3C
#define FRC522_P3_RESERVED2 0x3D
#define FRC522_P3_RESERVED3 0x3E
#define FRC522_P3_RESERVED4 0x3F
/*----------------------------------------------------------------------------*/
#define FRC522_ADDRESS_MASK 0x7E
#define FRC522_READ_MASK 0x80
/*----------------------------------------------------------------------------*/
void frc522_reg_write(unsigned char addr, unsigned char dval)  {
	spi_enable();
	spi_transfer((addr<<1)&FRC522_ADDRESS_MASK);
	spi_transfer(dval);
	spi_disable();
}
/*----------------------------------------------------------------------------*/
unsigned char frc522_reg_read(unsigned char addr) {
	unsigned char temp;
	spi_enable();
	spi_transfer(((addr<<1)&FRC522_ADDRESS_MASK)|FRC522_READ_MASK);
	temp = spi_transfer(0x00);
	spi_disable();
	return temp;
}
/*----------------------------------------------------------------------------*/
#define TIMER_AUTO 0x80
#define TIMER_MASK_PRESCALERH 0x0F
#define TIMER_6780KHZ_PRESCALEH 0x0D
#define TIMER_6780KHZ_PRESCALEL 0x3E
#define MODULATE_FORCE100ASK 0x40
#define MODE_TX_WAITRF 0x20
#define MODE_RESERVED0 0x10
#define MODE_POLMFIN 0x08
#define MODE_RESERVED1 0x04
#define MODE_CRC_PRESET_FFFF 0x03
#define MODE_CRC_PRESET_A671 0x02
#define MODE_CRC_PRESET_6363 0x01
#define MODE_CRC_PRESET_0000 0x00
#define CNTL_TX2RF_ENABLE 0x02
#define CNTL_TX1RF_ENABLE 0x01
/*----------------------------------------------------------------------------*/
#define INIT_P1_TXCTRL CNTL_TX2RF_ENABLE|CNTL_TX1RF_ENABLE
/* for f=6.78MHz? - prescale(0xd3e=3390):2kHz -> t=0.5ms? */
#define INIT_PRESCALE_H (TIMER_6780KHZ_PRESCALEH&TIMER_MASK_PRESCALERH)
#define INIT_PRESCALE_L TIMER_6780KHZ_PRESCALEL
/* reload timer at 30x0.5ms=15ms? */
#define INIT_TRELOAD_H 0x00
#define INIT_TRELOAD_L 0x1e
/*----------------------------------------------------------------------------*/
#define TXRX_ERROR_FLAG0 ERROR_BUFFER_OVERFLOW|ERROR_BIT_COLLISION
#define TXRX_ERROR_FLAGS TXRX_ERROR_FLAG0|ERROR_PARITY_FAILED|ERROR_PROTOCOL
/*----------------------------------------------------------------------------*/
unsigned char frc522_init(void) {
	unsigned char temp;
	frc522_reg_write(FRC522_P0_COMMAND_REG,FRC522_SOFT_RESET);
	frc522_reg_write(FRC522_P2_TMODE_REG,TIMER_AUTO|INIT_PRESCALE_H);
	frc522_reg_write(FRC522_P2_TPRESCALER_REG,INIT_PRESCALE_L);
	frc522_reg_write(FRC522_P2_TRELOADL_REG,INIT_TRELOAD_L);
	frc522_reg_write(FRC522_P2_TRELOADH_REG,INIT_TRELOAD_H);
	frc522_reg_write(FRC522_P1_TX_ASK_REG,MODULATE_FORCE100ASK);
	temp = frc522_reg_read(FRC522_P1_TX_CNTL_REG);
	frc522_reg_write(FRC522_P1_TX_CNTL_REG,temp|
		CNTL_TX2RF_ENABLE|CNTL_TX1RF_ENABLE); /* switch on antenna */
	/* returns firmware version */
	return frc522_reg_read(FRC522_P3_VERSION_REG);
}
/*----------------------------------------------------------------------------*/
#define COMIEN_IRQINV 0x80
#define COMIRQ_SET1 0x80
#define COMIRQ_RXIRQ 0x20
#define COMIRQ_IDLEIRQ 0x10
#define COMIRQ_TIMERIRQ 0x01
#define COMIRQ_WAITIRQ (COMIRQ_RXIRQ|COMIRQ_IDLEIRQ)
#define FIFO_FLUSHBUFF 0x80
#define BITFRAME_STARTSEND 0x80
#define BITFRAME_TXLASTBITS 0x07
#define ERROR_BUFFER_OVERFLOW 0x10
#define ERROR_BIT_COLLISION 0x08
#define ERROR_CRC_FAILED 0x04
#define ERROR_PARITY_FAILED 0x02
#define ERROR_PROTOCOL 0x01
/*----------------------------------------------------------------------------*/
void frc522_prep_tx(void) {
	unsigned char temp;
	temp = frc522_reg_read(FRC522_P0_COMIRQ_REG); /* clear all irq */
	frc522_reg_write(FRC522_P0_COMIRQ_REG,temp&~COMIRQ_SET1);
	temp = frc522_reg_read(FRC522_P0_FIFOLVL_REG); /* flush fifo buffers */
	frc522_reg_write(FRC522_P0_FIFOLVL_REG,temp|FIFO_FLUSHBUFF);
	frc522_reg_write(FRC522_P0_COMMAND_REG,FRC522_IDLE); /* force idle mode */
}
/*----------------------------------------------------------------------------*/
unsigned char frc522_push_tx(void) {
	unsigned char temp, loop;
	frc522_reg_write(FRC522_P0_COMMAND_REG,FRC522_TRANSCEIVE);
	temp = frc522_reg_read(FRC522_P0_BITFRAME_REG);
	frc522_reg_write(FRC522_P0_BITFRAME_REG,temp|BITFRAME_STARTSEND);
	/* wait... <25ms? */
	loop = 25;
	do {
		loop_delay(1);
		temp = frc522_reg_read(FRC522_P0_COMIRQ_REG);
		loop--;
	} while ((loop>0)&&!(temp&COMIRQ_TIMERIRQ)&&!(temp&COMIRQ_WAITIRQ));
	if (!loop) return FRC522_ERROR_TIMEOUT;
	if (frc522_reg_read(FRC522_P0_ERROR_REG)&(TXRX_ERROR_FLAGS))
		return FRC522_ERROR_TXRX;
	if (temp&COMIRQ_TIMERIRQ)
		return FRC522_ERROR_NO_TAG;
	return FRC522_OK;
}
/*----------------------------------------------------------------------------*/
unsigned char frc522_pull_rx(unsigned char *pdat) {
	unsigned char temp, test, loop;
	temp = frc522_reg_read(FRC522_P0_FIFOLVL_REG);
	test = frc522_reg_read(FRC522_P0_CONTROL_REG)&0x07;
	if (temp==0) temp = 1; // minimum single read
	else if (temp > FRC522_MAX_RXSIZE) // buffer limit
		temp = FRC522_MAX_RXSIZE;
	for (loop=0;loop<temp;loop++)
		pdat[loop] = frc522_reg_read(FRC522_P0_FIFODATA_REG);
	return temp;
}
/*----------------------------------------------------------------------------*/
unsigned char *atqa = 0x0;
/**
CardType | ATQA (16-bits) | SAK | UID Size
Mifare Classic 1K | 00 04 | 08 | 4 Bytes
Mifare Classic 4K | 00 02 | 18 | 4 Bytes
Mifare Ultralight | 00 44 | 00 | 7 Bytes
- note: SAK obtained in SELECT command?
**/
/*----------------------------------------------------------------------------*/
unsigned char frc522_scan(unsigned char *buff, unsigned char *blen) {
	unsigned char temp, test, loop;
	/* prepare for tx */
	frc522_reg_write(FRC522_P0_BITFRAME_REG,BITFRAME_TXLASTBITS);
	frc522_prep_tx();
	/* mifare cmd ReqA */
	frc522_reg_write(FRC522_P0_FIFODATA_REG,MF1_REQIDL);
	temp = frc522_push_tx();
	if (temp!=FRC522_OK) return temp;
	temp = frc522_pull_rx(buff);
	if (temp!=2)
		return FRC522_ERROR_REQ_A;
	if (atqa) {
		atqa[0] = buff[0];
		atqa[1] = buff[1];
	}
	/* prepare for tx */
	frc522_reg_write(FRC522_P0_BITFRAME_REG,0x00);
	frc522_prep_tx();
	/* mifare cmd CascadeLevel1? */
	frc522_reg_write(FRC522_P0_FIFODATA_REG,MF1_ANTICOLL);
	frc522_reg_write(FRC522_P0_FIFODATA_REG,0x20);
	temp = frc522_push_tx();
	if (temp!=FRC522_OK) return temp;
	temp = frc522_pull_rx(buff);
	/* checksum */
	test = 0x00;
	for (loop=0;loop<temp-1;loop++)
		test ^= buff[loop];
	if (test != buff[loop])
		return FRC522_ERROR_CHECKSUM;
	if (blen) *blen = temp;
	return FRC522_OK;
}
/*----------------------------------------------------------------------------*/
#endif /** __FRC522_H__ */
/*----------------------------------------------------------------------------*/
