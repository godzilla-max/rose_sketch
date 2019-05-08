/**
 * @file    SPIEEPROM.h
 * @brief  SPI EEPROM library(STMicro M95M02)
 * @author Yuuichi Akagawa
 * @date   2018/12/24
 */
#ifndef LIB_SPIEEPROM_SPIEEPROM_H_
#define LIB_SPIEEPROM_SPIEEPROM_H_
#include <Arduino.h>
#include <SPI.h>

#define SPI_CLK 3000000

class SPIEEPROM {
public:
	SPIEEPROM(uint8_t cs = SS);
	~SPIEEPROM() {};

	void    begin(uint32_t flashChipSize = 0);
	uint8_t readStatus();
	uint8_t readByte(uint32_t _addr);
	void    readArray(uint32_t _addr, uint8_t *data, uint32_t length);
	void    writeByte(uint32_t _addr, uint8_t data);
	void    writeArray(uint32_t _addr, uint8_t *data, uint32_t length);

	//SPI EEPROM Instructions
	static const uint8_t WREN  = 0b00000110;
	static const uint8_t WRDI  = 0b00000100;
	static const uint8_t RDSR  = 0b00000101;
	static const uint8_t WRSR  = 0b00000001;
	static const uint8_t READ  = 0b00000011;
	static const uint8_t WRITE = 0b00000010;
	static const uint8_t RDID  = 0b10000011;
	static const uint8_t WRID  = 0b10000010;
	static const uint8_t LID   = 0b10000010;

	//SRWD bit
	static const uint8_t SRWD_SRWD  = 0b10000000;
	static const uint8_t SRWD_BP1   = 0b00001000;
	static const uint8_t SRWD_BP0   = 0b00000100;
	static const uint8_t SRWD_WEL   = 0b00000010;
	static const uint8_t SRWD_WIP   = 0b00000001;

private:
	void enableWrite();
	void sendAddress(uint32_t _addr);
	bool WIP();

	uint8_t     csPin;
#ifdef SPI_HAS_TRANSACTION
	SPISettings _settings;
#endif
};

#endif /* LIB_SPIEEPROM_SPIEEPROM_H_ */
