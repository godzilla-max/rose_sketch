/**
 * @file    SPIEEPROM.cpp
 * @brief  SPI EEPROM library(STMicro M95M02)
 * @author Yuuichi Akagawa
 * @date   2018/12/24
 */

#include "SPIEEPROM.h"

SPIEEPROM::SPIEEPROM(uint8_t cs) {
	  csPin = cs;
	  pinMode(csPin, OUTPUT);
}

/**
 * @brief ライブラリの初期化を行う関数
 * @param none
 * @return none
 * @detail SPIの初期化を行う
 */
void SPIEEPROM::begin(uint32_t flashChipSize)
{
    SPI.begin();
#ifdef SPI_HAS_TRANSACTION
  //Define the settings to be used by the SPI bus
  _settings = SPISettings(SPI_CLK, MSBFIRST, SPI_MODE0);
#else
  SPI.setBitOrder(MSBFIRST);              // the chip works MSB first
  SPI.setDataMode(SPI_MODE0);             // clock MODE0 : rising edge, low default state
#endif
}

/**
 * @brief EEPROMのステータスを取得する関数
 * @param none
 * @return uint8_t RDSRの値を返す
 * @detail EEPROMのRDSRの値を取得し、返却する
 */
uint8_t SPIEEPROM::readStatus()
{
#ifdef SPI_HAS_TRANSACTION
	SPI.beginTransaction(_settings);
#endif
	digitalWrite(csPin, LOW);
	SPI.transfer(SPIEEPROM::RDSR);
	uint8_t d = SPI.transfer(0);
	digitalWrite(csPin, HIGH);
#ifdef SPI_HAS_TRANSACTION
	SPI.endTransaction();
#endif
	return d;
}

/**
 * @brief EEPROMの書き込み許可を設定する関数
 * @param none
 * @return none
 * @detail EEPROMにWRENコマンドを送信し、書き込み可能にする。WRITEの前に必ず実行する必要がある。
 */
void SPIEEPROM::enableWrite()
{
#ifdef SPI_HAS_TRANSACTION
	SPI.beginTransaction(_settings);
#endif
	digitalWrite(csPin, LOW);
	SPI.transfer(SPIEEPROM::WREN);
	digitalWrite(csPin, HIGH);
#ifdef SPI_HAS_TRANSACTION
	SPI.endTransaction();
#endif
}

/**
 * @brief EEPROMにアドレスを送信する関数
 * @param[in] _addr アドレス(24bit)
 * @return none
 * @detail EEPROMにアドレスを送信する(MSB first)。CS制御等は呼び出し元で行うこと。
 */
void SPIEEPROM::sendAddress(uint32_t _addr){
	SPI.transfer((byte)((_addr>>16) & 0xff));
	SPI.transfer((byte)((_addr>>8) & 0xff));
	SPI.transfer((byte)(_addr & 0xff));
}

/**
 * @brief EEPROMの書き込み状態を返す関数
 * @param none
 * @return bool true:書き込み処理中
 * @detail EEPROMのRDSRの値を取得し、WIPの状態を返す。
 */
bool SPIEEPROM::WIP()
{
	uint8_t d = readStatus();
	return (d & SPIEEPROM::SRWD_WIP);
}

/**
 * @brief EEPROMから1バイト読み込む関数
 * @param[in] _addr アドレス
 * @return uint8_t データ
 * @detail EEPROMから1バイトだけ読み込む。
 */
uint8_t SPIEEPROM::readByte(uint32_t _addr)
{
#ifdef SPI_HAS_TRANSACTION
	SPI.beginTransaction(_settings);
#endif
	digitalWrite(csPin, LOW);
	SPI.transfer(SPIEEPROM::READ);
	sendAddress(_addr);
	uint8_t d = SPI.transfer(0);
	digitalWrite(csPin, HIGH);
#ifdef SPI_HAS_TRANSACTION
	SPI.endTransaction();
#endif
	return d;
}

/**
 * @brief EEPROMから複数バイト読み込む関数
 * @param[in] _addr アドレス
 * @param[out] *data 出力先バッファの先頭アドレス
 * @param[in] length 読み込むサイズ
 * @return none
 * @detail EEPROMから連続した複数バイト数読み込む。
 */
void SPIEEPROM::readArray(uint32_t _addr, uint8_t *data, uint32_t length)
{
#ifdef SPI_HAS_TRANSACTION
	SPI.beginTransaction(_settings);
#endif
	digitalWrite(csPin, LOW);
	SPI.transfer(SPIEEPROM::READ);
	sendAddress(_addr);
	for(uint32_t i=0; i<length; i++){
		data[i] = SPI.transfer(0);
	}
	digitalWrite(csPin, HIGH);
#ifdef SPI_HAS_TRANSACTION
	SPI.endTransaction();
#endif
}

/**
 * @brief EEPROMに1バイト書き込む関数
 * @param[in] _addr アドレス
 * @param[in] data データ
 * @return none
 * @detail EEPROMに1バイトだけ書き込む。
 */
void SPIEEPROM::writeByte(uint32_t _addr, uint8_t data)
{
	enableWrite();
#ifdef SPI_HAS_TRANSACTION
	SPI.beginTransaction(_settings);
#endif
	digitalWrite(csPin, LOW);
	SPI.transfer(SPIEEPROM::WRITE);
	sendAddress(_addr);
	SPI.transfer(data);
	digitalWrite(csPin, HIGH);
#ifdef SPI_HAS_TRANSACTION
	SPI.endTransaction();
#endif
	// wait for writing finish
	while (WIP()) {};
}

/**
 * @brief EEPROMに複数バイト書き込む関数
 * @param[in] _addr アドレス
 * @param[in] *data 書き込みデータバッファの先頭アドレス
 * @param[in] length 書き込みサイズ
 * @return none
 * @detail EEPROMに連続した複数バイト書き込む。
 */
void SPIEEPROM::writeArray(uint32_t _addr, uint8_t *data, uint32_t length)
{
	enableWrite();
#ifdef SPI_HAS_TRANSACTION
	SPI.beginTransaction(_settings);
#endif
	digitalWrite(csPin, LOW);
	SPI.transfer(SPIEEPROM::WRITE);
	sendAddress(_addr);
	for(uint32_t i=0; i<length; i++){
		SPI.transfer(data[i]);
	}
	digitalWrite(csPin, HIGH);
#ifdef SPI_HAS_TRANSACTION
	SPI.endTransaction();
#endif
	// wait for writing finish
	while (WIP()) {};
}
