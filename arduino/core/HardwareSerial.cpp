/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
  Modified 13 July 2014 by Nozomu Fujita for GR-SAKURA
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

#ifdef GRROSE
#include "utilities.h"
#include "usb_hal.h"
#include "usb_cdc.h"
#include "util.h"
extern "C"{
typedef struct SET_CONTROL_LINE_STATE_DATA
{
    uint32_t dwDTERate;
    uint8_t bCharFormat;
    uint8_t bParityType;
    uint8_t bDataBits;
}SET_CONTROL_LINE_STATE_DATA;
extern SET_CONTROL_LINE_STATE_DATA g_SetControlLineData;
extern uint8_t g_SET_CONTROL_LINE_STATE_DATA_Buffer[];
extern uint16_t setupPacketwValue;
}

/// For USB receive /////////////////////////////
/// The definitions are copied from usb_hal.c ///
#include "iodefine.h"
/*NOTE USB0 is defined in iodefine.h file*/
#define USBIO USB0
#define PID_BUF     1
/////////////////////////////////////////////

struct SciInterruptRegistersTableStruct {
  uint8_t _txier;
  uint8_t _txien;
  uint8_t _rxier;
  uint8_t _rxien;
  uint8_t _txir;
  uint8_t _rxir;
  uint8_t _txipr;
  uint8_t _rxipr;
  uint8_t _eri_group;
  uint8_t _eri_bit;
};
static const SciInterruptRegistersTableStruct SciInterruptRegistersTable[] = {
        {IER_SCI0_TXI0, 3, IER_SCI0_RXI0, 2, IR_SCI0_TXI0, IR_SCI0_RXI0, IPR_SCI0_TXI0, IPR_SCI0_RXI0, 0, 1},
        {IER_SCI2_TXI2, 7, IER_SCI2_RXI2, 6, IR_SCI2_TXI2, IR_SCI2_RXI2, IPR_SCI2_TXI2, IPR_SCI2_RXI2, 0, 5},
        {IER_SCI5_TXI5, 5, IER_SCI5_RXI5, 4, IR_SCI5_TXI5, IR_SCI5_RXI5, IPR_SCI5_TXI5, IPR_SCI5_RXI5, 0, 11},
        {IER_SCI6_TXI6, 7, IER_SCI6_RXI6, 6, IR_SCI6_TXI6, IR_SCI6_RXI6, IPR_SCI6_TXI6, IPR_SCI6_RXI6, 0, 13},
        {IER_SCI1_TXI1, 5, IER_SCI1_RXI1, 4, IR_SCI1_TXI1, IR_SCI1_RXI1, IPR_SCI1_TXI1, IPR_SCI1_RXI1, 0, 3},
        {IER_SCI3_TXI3, 1, IER_SCI3_RXI3, 0, IR_SCI3_TXI3, IR_SCI3_RXI3, IPR_SCI3_TXI3, IPR_SCI3_RXI3, 0, 7},
        {IER_SCI8_TXI8, 5, IER_SCI8_RXI8, 4, IR_SCI8_TXI8, IR_SCI8_RXI8, IPR_SCI8_TXI8, IPR_SCI8_RXI8, 1, 25},
};

// TODO
// this typedef and array is copyed from FIT modules.
// Should be considered to implement Serial class using FIT modules.
#define NUM_DIVISORS_ASYNC (9)
typedef struct st_baud_divisor
{
    int16_t     divisor;    // clock divisor
    uint8_t     abcs;       // abcs value to get divisor
    uint8_t     bgdm;       // bdgm value to get divisor
    uint8_t     cks;        // cks  value to get divisor (cks = n)
} baud_divisor_t;

const baud_divisor_t async_baud[NUM_DIVISORS_ASYNC]=
{
    /* divisor result, abcs, bgdm, n */
    {8,    1, 1, 0},
    {16,   0, 1, 0},
    {32,   0, 0, 0},
    {64,   0, 1, 1},
    {128,  0, 0, 1},
    {256,  0, 1, 2},
    {512,  0, 0, 2},
    {1024, 0, 1, 3},
    {2048, 0, 0, 3}
};
///////////////////////////////////

#endif // GRROSE

// this next line disables the entire HardwareSerial.cpp, 
// this is so I can support Attiny series and any other chip without a uart
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3) || defined(HAVE_HWSERIAL4) || defined(HAVE_HWSERIAL5) || defined(HAVE_HWSERIAL6)  || defined(HAVE_HWSERIAL7)

HardwareSerial Serial(0, NULL, MstpIdINVALID, INVALID_IO, INVALID_IO, INVALID_IO);
HardwareSerial Serial1(1, &SCI0, MstpIdSCI0, 1, 0, OUTPUT_OPENDRAIN);
HardwareSerial Serial2(2, &SCI2, MstpIdSCI2, 3, 2, OUTPUT_OPENDRAIN);
HardwareSerial Serial3(3, &SCI5, MstpIdSCI5, 5, 4, OUTPUT_OPENDRAIN);
HardwareSerial Serial4(4, &SCI6, MstpIdSCI6, 7, 6, OUTPUT_OPENDRAIN);
HardwareSerial Serial5(5, &SCI1, MstpIdSCI1, 9, 8, OUTPUT);
HardwareSerial Serial6(6, &SCI3, MstpIdSCI3, PIN_ESP_TX, PIN_ESP_RX, OUTPUT);
HardwareSerial Serial7(7, &SCI8, MstpIdSCI8, PIN_RS485_TX, PIN_RS485_RX, OUTPUT_OPENDRAIN);

// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
// The Serialx_available is just a wrapper around Serialx.available(),
// but we can refer to it weakly so we don't pull in the entire
// HardwareSerial instance if the user doesn't also refer to it.
#if defined(HAVE_HWSERIAL0)
  void serialEvent() __attribute__((weak));
  bool Serial0_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL1)
  void serialEvent1() __attribute__((weak));
  bool Serial1_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL2)
  void serialEvent2() __attribute__((weak));
  bool Serial2_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL3)
  void serialEvent3() __attribute__((weak));
  bool Serial3_available() __attribute__((weak));
#endif

#ifdef __RX600__
#if defined(HAVE_HWSERIAL4)
  void serialEvent4() __attribute__((weak));
  bool Serial4_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL5)
  void serialEvent5() __attribute__((weak));
  bool Serial5_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL6)
  void serialEvent6() __attribute__((weak));
  bool Serial6_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL7)
  void serialEvent7() __attribute__((weak));
  bool Serial7_available() __attribute__((weak));
#endif

#endif

void serialEventRun(void)
{
#if defined(HAVE_HWSERIAL0)
  if (Serial0_available && serialEvent && Serial0_available()) serialEvent();
#endif
#if defined(HAVE_HWSERIAL1)
  if (Serial1_available && serialEvent1 && Serial1_available()) serialEvent1();
#endif
#if defined(HAVE_HWSERIAL2)
  if (Serial2_available && serialEvent2 && Serial2_available()) serialEvent2();
#endif
#if defined(HAVE_HWSERIAL3)
  if (Serial3_available && serialEvent3 && Serial3_available()) serialEvent3();
#endif
#ifdef __RX600__
#if defined(HAVE_HWSERIAL4)
  if (Serial4_available && serialEvent4 && Serial4_available()) serialEvent4();
#endif
#if defined(HAVE_HWSERIAL5)
  if (Serial5_available && serialEvent5 && Serial5_available()) serialEvent5();
#endif
#if defined(HAVE_HWSERIAL6)
  if (Serial6_available && serialEvent6 && Serial6_available()) serialEvent6();
#endif
#if defined(HAVE_HWSERIAL7)
  if (Serial7_available && serialEvent7 && Serial7_available()) serialEvent7();
#endif
#endif/*__RX600__*/
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial::_tx_udr_empty_irq(void)
{
#ifndef __RX600__
  // If interrupts are enabled, there must be more data in the output
  // buffer. Send the next byte
  unsigned char c = _tx_buffer[_tx_buffer_tail];
  _tx_buffer_tail = (_tx_buffer_tail + 1) % SERIAL_BUFFER_SIZE;

  *_udr = c;

  // clear the TXC bit -- "can be cleared by writing a one to its bit
  // location". This makes sure flush() won't return until the bytes
  // actually got written
  sbi(*_ucsra, TXC0);

  if (_tx_buffer_head == _tx_buffer_tail) {
    // Buffer empty, so disable interrupts
    cbi(*_ucsrb, UDRIE0);
  }
#else /*__RX600__*/
  if (_tx_buffer_head != _tx_buffer_tail) {
    _sci->TDR = _tx_buffer[_tx_buffer_tail];
    _tx_buffer_tail = (_tx_buffer_tail + 1) % SERIAL_BUFFER_SIZE;
  } else {
    _sending = false;
  }
#endif/*__RX600__*/
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned long baud, byte config)
{
#ifndef __RX600__
  // Try u2x mode first
  uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
  *_ucsra = 1 << U2X0;

  // hardcoded exception for 57600 for compatibility with the bootloader
  // shipped with the Duemilanove and previous boards and the firmware
  // on the 8U2 on the Uno and Mega 2560. Also, The baud_setting cannot
  // be > 4095, so switch back to non-u2x mode if the baud rate is too
  // low.
  if (((F_CPU == 16000000UL) && (baud == 57600)) || (baud_setting >4095))
  {
    *_ucsra = 0;
    baud_setting = (F_CPU / 8 / baud - 1) / 2;
  }

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  *_ubrrh = baud_setting >> 8;
  *_ubrrl = baud_setting;

  _written = false;

  //set the data bits, parity, and stop bits
#if defined(__AVR_ATmega8__)
  config |= 0x80; // select UCSRC register (shared with UBRRH)
#endif
  *_ucsrc = config;
  
  sbi(*_ucsrb, RXEN0);
  sbi(*_ucsrb, TXEN0);
  sbi(*_ucsrb, RXCIE0);
  cbi(*_ucsrb, UDRIE0);
#else /*__RX600__*/
  switch (_serial_channel) {
#if defined(HAVE_HWSERIAL0)
  case 0:
    {
        USB_ERR err = USBCDC_Init();
        if (err == USB_ERR_OK) {
            const unsigned long TimeOut = 3000;
            unsigned long start = millis();
            while ((millis() - start) < TimeOut) {
                if (USBCDC_IsConnected()) {
                    _begin = true;
                    break;
                }
            }
        }
    }
    break;

#endif
#if defined(HAVE_HWSERIAL1)
  case 1:
#endif
#if defined(HAVE_HWSERIAL2)
  case 2:
#endif
#if defined(HAVE_HWSERIAL3)
  case 3:
#endif
#if defined(HAVE_HWSERIAL4)
  case 4:
#endif
#if defined(HAVE_HWSERIAL5)
  case 5:
#endif
#if defined(HAVE_HWSERIAL6)
  case 6:
#endif
#if defined(HAVE_HWSERIAL7)
  case 7:
#endif
#if defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3) || defined(HAVE_HWSERIAL4) || defined(HAVE_HWSERIAL5) || defined(HAVE_HWSERIAL6) || defined(HAVE_HWSERIAL7)
    {
      digitalWrite(_txpin, HIGH);
      pinMode(_txpin, _output_mode);
      pinMode(_rxpin, INPUT_PULLUP);

      startModule(_module);

      _sci->SCR.BIT.CKE = 0;
      _sci->SCR.BIT.TEIE = 0;
      _sci->SCR.BIT.MPIE = 0;
      _sci->SCR.BIT.RE = 0;
      _sci->SCR.BIT.TE = 0;
      _sci->SCR.BIT.RIE = 0;
      _sci->SCR.BIT.TIE = 0;
      _sci->SCMR.BIT.SMIF = 0;
      _sci->SCMR.BIT.SINV = 0;
      _sci->SCMR.BIT.SDIR = 0;
      _sci->SMR.BIT.MP = 0;
      _sci->SMR.BIT.CM = 0;

      {
        const uint8_t SERIAL_DM = 0b00000111;
        const uint8_t SERIAL_D8 = 0b00000110;
        const uint8_t SERIAL_D7 = 0b00000100;
        const uint8_t SERIAL_PM = 0b00110000;
        const uint8_t SERIAL_PN = 0b00000000;
        const uint8_t SERIAL_PE = 0b00100000;
        const uint8_t SERIAL_PO = 0b00110000;
        const uint8_t SERIAL_SM = 0b00001000;
        const uint8_t SERIAL_S1 = 0b00000000;
        const uint8_t SERIAL_S2 = 0b00001000;

        _sci->SMR.BIT.CHR = 0;
        _sci->SMR.BIT.PE = 0;
        _sci->SMR.BIT.PM = 0;
        _sci->SMR.BIT.STOP = 0;
        switch (config & SERIAL_DM) {
        case SERIAL_D8:
          _sci->SMR.BIT.CHR = 0;
          break;
        case SERIAL_D7:
          _sci->SMR.BIT.CHR = 1;
          break;
        default:
          break;
        }
        switch (config & SERIAL_PM) {
        case SERIAL_PN:
          _sci->SMR.BIT.PE = 0;
          break;
        case SERIAL_PE:
          _sci->SMR.BIT.PE = 1;
          _sci->SMR.BIT.PM = 0;
          break;
        case SERIAL_PO:
          _sci->SMR.BIT.PE = 1;
          _sci->SMR.BIT.PM = 1;
          break;
        default:
          break;
        }
        switch (config & SERIAL_SM) {
        case SERIAL_S1:
          _sci->SMR.BIT.STOP = 0;
          break;
        case SERIAL_S2:
          _sci->SMR.BIT.STOP = 1;
          break;
        default:
          break;
        }
      }

      /* FIND DIVISOR; table has associated ABCS, BGDM and CKS values */
      /* BRR must be 255 or less */
      /* the "- 1" is ignored in some steps for approximations */
      /* BRR = (PCLK/(divisor * baud)) - 1 */
      /* BRR = (ratio / divisor) - 1 */
      uint32_t ratio = PCLK/baud;
      int i;
      for(i = 0; i < NUM_DIVISORS_ASYNC; i++)
      {
          if (ratio < (uint32_t)(async_baud[i].divisor * 256))
          {
              break;
          }
      }

      /* RETURN IF BRR WILL BE >255 OR LESS THAN 0 */
      if (i == NUM_DIVISORS_ASYNC)
      {
          i = NUM_DIVISORS_ASYNC - 1;
      }

      uint32_t divisor = (uint32_t)async_baud[i].divisor;
      uint32_t tmp = ratio/(divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1

      /* SET BRR, ABCS, BDGM, and CKS */
      tmp = ratio / (divisor/2);  // divide by half the divisor

      /* if odd, "round up" by ignoring -1; divide by 2 again for rest of divisor */
      _sci->BRR = (uint8_t)((tmp & 0x01) ? (tmp/2) : ((tmp/2)-1));
      _sci->SEMR.BIT.ABCS = async_baud[i].abcs;
      _sci->SEMR.BIT.BGDM = async_baud[i].bgdm;
      _sci->SMR.BIT.CKS = async_baud[i].cks;

      /* CALCULATE BIT RATE ERROR.
       * RETURN IF ERROR LESS THAN 1% OR IF IN SYNCHRONOUS/SSPI MODE.
       */
      tmp = ratio/(divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1
      float error = ( ((float)PCLK / ((baud * divisor) * tmp)) - 1) * 100;
      float abs_error  = (error < 0) ? (-error) : error;

      if (abs_error <= 1.0)
      {
    	  _sci->SEMR.BIT.BRME = 0;          // disable MDDR
      } else {
          /* CALCULATE M ASSUMING A 0% ERROR then WRITE REGISTER */
          _sci->BRR = (uint8_t)(tmp-1);
          float float_M = ((float)((baud * divisor) * 256) * tmp) / PCLK;
          float_M *= 2;
          uint32_t int_M = (uint32_t)float_M;
          int_M = (int_M & 0x01) ? ((int_M/2) + 1) : (int_M/2);

          _sci->MDDR = (uint8_t)int_M;      // write M
          _sci->SEMR.BIT.BRME = 1;          // enable MDDR
      }

      {
      	_sci->SSR.BIT.ORER = 0;
    	_sci->SSR.BIT.FER = 0;
    	_sci->SSR.BIT.PER = 0;
    	_sci->SSR.BIT.RDRF = 1;
    	_sci->SSR.BIT.TDRE = 1;
      }

      {
        const SciInterruptRegistersTableStruct* t = &SciInterruptRegistersTable[_serial_channel - 1];
        ICU.IPR[t->_txipr].BIT.IPR = 2;
        ICU.IPR[t->_rxipr].BIT.IPR = 2;

        BSET(&ICU.IER[t->_txier].BYTE, t->_txien);
        BSET(&ICU.IER[t->_rxier].BYTE, t->_rxien);

        ICU.IR[t->_txir].BIT.IR = 0x0;
        ICU.IR[t->_rxir].BIT.IR = 0x0;

        _sci->SCR.BIT.TIE = 1;
        _sci->SCR.BIT.RIE = 1;
        _sci->SCR.BIT.TE = 1;
        _sci->SCR.BIT.RE = 1;

        ICU.IER[0xD].BIT.IEN6 = 1; // GROUP BL0 interrupt for error communication
        ICU.IPR[110].BIT.IPR = 2;
        ICU.IR[110].BIT.IR = 0;
        ICU.IER[0xD].BIT.IEN7 = 1; // GROUP BL1 interrupt for error communication
        ICU.IPR[111].BIT.IPR = 2;
        ICU.IR[110].BIT.IR = 0;
        switch(t->_eri_group){
        case 0 :
            ICU.GENBL0.LONG |= (1 << t->_eri_bit);
        	break;
        case 1 :
            ICU.GENBL1.LONG |= (1 << t->_eri_bit);
        	break;
        default :
        	break;
        }
      }

      {
        int txPort = digitalPinToPort(_txpin);
        int rxPort = digitalPinToPort(_rxpin);
        int txBit = digitalPinToBit(_txpin);
        int rxBit = digitalPinToBit(_rxpin);

        assignPinFunction(_txpin, 0b01010, 0, 0);
        assignPinFunction(_rxpin, 0b01010, 0, 0);

        BSET(portModeRegister(txPort), txBit);
        BSET(portModeRegister(rxPort), rxBit);
      }
      _begin = true;

	  // for initialization of ESP8266 AT commands
      if(_serial_channel == 6){
    		pinMode(PIN_ESP_EN, OUTPUT);
    		pinMode(PIN_ESP_IO0, OUTPUT);
    		pinMode(PIN_ESP_IO15, OUTPUT);

    		digitalWrite(PIN_ESP_IO0, HIGH);
    		digitalWrite(PIN_ESP_IO15, LOW);
    		digitalWrite(PIN_ESP_EN, LOW);
    		delay(100);
    		digitalWrite(PIN_ESP_EN, HIGH);
      }
	  // for initialization for RS-485
      if(_serial_channel == 7){
          ICU.GENBL1.LONG |= (1 << 24); // TEND interrupt for switching input.
          digitalWrite(PIN_RS485_DIR, LOW);
          pinMode(PIN_RS485_DIR, OUTPUT);
      }
    }
    break;
#endif
  default:
    break;
  }
#endif/*__RX600__*/
}

void HardwareSerial::end()
{
#ifndef __RX600__
  // wait for transmission of outgoing data
  while (_tx_buffer_head != _tx_buffer_tail)
    ;

  cbi(*_ucsrb, RXEN0);
  cbi(*_ucsrb, TXEN0);
  cbi(*_ucsrb, RXCIE0);
  cbi(*_ucsrb, UDRIE0);
  
  // clear any received data
  _rx_buffer_head = _rx_buffer_tail;
#else /*__RX600__*/
  switch (_serial_channel) {
#if defined(HAVE_HWSERIAL0)
  case 0:
    {
      USBCDC_Cancel();
    }
    break;
#endif
#if defined(HAVE_HWSERIAL1)
  case 1:
#endif
#if defined(HAVE_HWSERIAL2)
  case 2:
#endif
#if defined(HAVE_HWSERIAL3)
  case 3:
#endif
#if defined(HAVE_HWSERIAL4)
  case 4:
#endif
#if defined(HAVE_HWSERIAL5)
  case 5:
#endif
#if defined(HAVE_HWSERIAL6)
  case 6:
#endif
#if defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3) || defined(HAVE_HWSERIAL4) || defined(HAVE_HWSERIAL5) || defined(HAVE_HWSERIAL6)
    {
      flush();
      {
        const SciInterruptRegistersTableStruct* t = &SciInterruptRegistersTable[_serial_channel - 1];
        ICU.IR[t->_txir].BIT.IR = 0x0;
        ICU.IR[t->_rxir].BIT.IR = 0x0;

        BCLR(&ICU.IER[t->_txier].BYTE, t->_txien);
        BCLR(&ICU.IER[t->_rxier].BYTE, t->_rxien);

        ICU.IPR[t->_txipr].BIT.IPR = 0;
        ICU.IPR[t->_rxipr].BIT.IPR = 0;

        switch(t->_eri_group){
        case 0 :
            ICU.GENBL0.LONG &= ~(1 << t->_eri_bit);
        	break;
        case 1 :
            ICU.GENBL1.LONG &= ~(1 << t->_eri_bit);
        	break;
        default :
        	break;
        }
      }

      stopModule(_module);
      _begin = false;
    }
    break;
#endif
  default:
    break;
  }
#endif/*__RX600__*/
}

void HardwareSerial::direction(uint8_t dir){
	// TODO : Fix to remove magic number
	switch(_serial_channel) {
	case 1 :
		if(dir == DUPLEX){
			while(!_sci->SSR.BIT.TEND);
			pinMode(PIN_S1_SEL, INPUT);
//			digitalWrite(PIN_S1_SEL, LOW);  // pull-down by hardware
		} else {
			pinMode(PIN_S1_SEL, OUTPUT);
			digitalWrite(PIN_S1_SEL, HIGH);
		}
		break;
	case 2 :
		if(dir == DUPLEX){
			while(!_sci->SSR.BIT.TEND);
			pinMode(PIN_S2_SEL, INPUT);
//			digitalWrite(PIN_S2_SEL, LOW);  // pull-down by hardware
		} else {
			pinMode(PIN_S2_SEL, OUTPUT);
			digitalWrite(PIN_S2_SEL, HIGH);
		}
		break;
	case 3 :
		if(dir == DUPLEX){
			while(!_sci->SSR.BIT.TEND);
			pinMode(PIN_S3_SEL, INPUT);
//			digitalWrite(PIN_S3_SEL, LOW);  // pull-down by hardware
		} else {
			pinMode(PIN_S3_SEL, OUTPUT);
			digitalWrite(PIN_S3_SEL, HIGH);
		}
		break;
	case 4 :
		if(dir == DUPLEX){
			while(!_sci->SSR.BIT.TEND);
			pinMode(PIN_S4_SEL, INPUT);
//			digitalWrite(PIN_S4_SEL, LOW);  // pull-down by hardware
		} else {
			pinMode(PIN_S4_SEL, OUTPUT);
			digitalWrite(PIN_S4_SEL, HIGH);
		}
		break;
	case 7 :
		if(dir == OUTPUT){
          	PORTC.PODR.BIT.B5 = HIGH; // Switch direction of RS-485 to OUTPUT
		} else {
			while(!_sci->SSR.BIT.TEND);
          	PORTC.PODR.BIT.B5 = LOW; // Switch direction of RS-485 to INPUT
		}
		break;
	default :
		break;
	}
}

int HardwareSerial::available(void)
{
  return (unsigned int)(SERIAL_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail) % SERIAL_BUFFER_SIZE;
}

int HardwareSerial::peek(void)
{
  if (_rx_buffer_head == _rx_buffer_tail) {
    return -1;
  } else {
    return _rx_buffer[_rx_buffer_tail];
  }
}

int HardwareSerial::read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (_rx_buffer_head == _rx_buffer_tail) {
    return -1;
  } else {
    unsigned char c = _rx_buffer[_rx_buffer_tail];
#if SERIAL_BUFFER_SIZE < 256
	_rx_buffer_tail = (uint8_t)(_rx_buffer_tail + 1) % SERIAL_BUFFER_SIZE;
#else
	_rx_buffer_tail = (uint32_t)(_rx_buffer_tail + 1) % SERIAL_BUFFER_SIZE;
#endif
    return c;
  }
}

void HardwareSerial::flush()
{
#ifndef __RX600__
  // If we have never written a byte, no need to flush. This special
  // case is needed since there is no way to force the TXC (transmit
  // complete) bit to 1 during initialization
  if (!_written)
    return;

  while (bit_is_set(*_ucsrb, UDRIE0) || bit_is_clear(*_ucsra, TXC0)) {
    if (bit_is_clear(SREG, SREG_I) && bit_is_set(*_ucsrb, UDRIE0))
	// Interrupts are globally disabled, but the DR empty
	// interrupt should be enabled, so poll the DR empty flag to
	// prevent deadlock
	if (bit_is_set(*_ucsra, UDRE0))
	  _tx_udr_empty_irq();
  }
  // If we get here, nothing is queued anymore (DRIE is disabled) and
  // the hardware finished tranmission (TXC is set).
#else /*__RX600__*/
  while (_tx_buffer_head != _tx_buffer_tail) {
    ;
  }
  while (_sending) {
    ;
  }
  if(_serial_channel != 0){
	  while(!_sci->SSR.BIT.TEND);
  }

#endif/*__RX600__*/
}

size_t HardwareSerial::write(uint8_t c)
{
#ifndef __RX600__
  // If the buffer and the data register is empty, just write the byte
  // to the data register and be done. This shortcut helps
  // significantly improve the effective datarate at high (>
  // 500kbit/s) bitrates, where interrupt overhead becomes a slowdown.
  if (_tx_buffer_head == _tx_buffer_tail && bit_is_set(*_ucsra, UDRE0)) {
    *_udr = c;
    sbi(*_ucsra, TXC0);
    return 1;
  }
  uint8_t i = (_tx_buffer_head + 1) % SERIAL_BUFFER_SIZE;
	
  // If the output buffer is full, there's nothing for it other than to 
  // wait for the interrupt handler to empty it a bit
  while (i == _tx_buffer_tail) {
    if (bit_is_clear(SREG, SREG_I)) {
      // Interrupts are disabled, so we'll have to poll the data
      // register empty flag ourselves. If it is set, pretend an
      // interrupt has happened and call the handler to free up
      // space for us.
      if(bit_is_set(*_ucsra, UDRE0))
	_tx_udr_empty_irq();
    } else {
      // nop, the interrupt handler will free up space for us
    }
  }

  _tx_buffer[_tx_buffer_head] = c;
  _tx_buffer_head = i;
	
  sbi(*_ucsrb, UDRIE0);
  _written = true;
  
  return 1;
#else /*__RX600__*/
  switch (_serial_channel) {
#if defined(HAVE_HWSERIAL0)
  case 0:
    {
        unsigned int i = (_tx_buffer_head + 1) % SERIAL_BUFFER_SIZE;

        if (_begin) {
            if (i != _tx_buffer_tail) {
                USB0.INTENB0.BIT.BRDYE = 0;
                _tx_buffer[_tx_buffer_head] = c;
                _tx_buffer_head = i;
                USB0.INTENB0.BIT.BRDYE = 1;
                USB0.BRDYENB.BIT.PIPE2BRDYE = 1;
            }
        } else {
            if (USBCDC_IsConnected()) {
                _begin = true;
            }
            _tx_buffer[_tx_buffer_head] = c;
            _tx_buffer_head = i;
        }
        return 1;

    }
#endif
#if defined(HAVE_HWSERIAL1)
  case 1:
#endif
#if defined(HAVE_HWSERIAL2)
  case 2:
#endif
#if defined(HAVE_HWSERIAL3)
  case 3:
#endif
#if defined(HAVE_HWSERIAL4)
  case 4:
#endif
#if defined(HAVE_HWSERIAL5)
  case 5:
#endif
#if defined(HAVE_HWSERIAL6)
  case 6:
#endif
#if defined(HAVE_HWSERIAL7)
  case 7:
#endif
#if defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3) || defined(HAVE_HWSERIAL4) || defined(HAVE_HWSERIAL5) || defined(HAVE_HWSERIAL6) || defined(HAVE_HWSERIAL7)
    {
      unsigned int i = (_tx_buffer_head + 1) % SERIAL_BUFFER_SIZE;
      // If the output buffer is full, there's nothing for it other than to
      // wait for the interrupt handler to empty it a bit
      // ???: return 0 here instead?
      if (_begin) {
        while (i == _tx_buffer_tail) {
          ;
        }
        _tx_buffer[_tx_buffer_head] = c;
        _tx_buffer_head = i;

        if (!_sending) {
          if(_serial_channel == 7){ // for switching direction
          	PORTC.PODR.BIT.B5 = HIGH; // Switch direction of RS-485 to OUTPUT
          }
          bool di = isNoInterrupts();
          noInterrupts();
          _sending = true;
          _tx_udr_empty_irq();
          if (!di) {
            interrupts();
          }
        }
      } else {
        if (i != _tx_buffer_tail) {
          _tx_buffer[_tx_buffer_head] = c;
          _tx_buffer_head = i;
        }
      }
      return 1;
    }
#endif
  default:
    return 0;
  }
#endif/*__RX600__*/
}

#ifdef GRROSE
int HardwareSerial::getDteRate(void){
	return (int)g_SetControlLineData.dwDTERate;
}
int HardwareSerial::getDteDataBits(void){
	return (int)g_SetControlLineData.bDataBits;
}
int HardwareSerial::getDteParityType(void){
	return (int)g_SetControlLineData.bParityType;
}
int HardwareSerial::getDteCharFormat(void){
	return (int)g_SetControlLineData.bCharFormat;
}
bool HardwareSerial::getDteState(void){
	return ((setupPacketwValue & 0x0001U) != 0);
}
uint8_t HardwareSerial::getDteConfig(void){
	uint8_t res = 0;

	if(getDteDataBits() == 8){
		res |= 0b00000110;
	} else if(getDteDataBits() == 7){
		res |= 0b00000100;
	}

	if(getDteCharFormat() == 2){
		res |= 0b00001000;
	}

	if(getDteParityType() == 1){ // odd
		res |= 0b00110000;
	} else if(getDteParityType() == 2){ // even
		res |= 0b00100000;
	}
	return res;
}
bool HardwareSerial::isConnected(void){
	return (getDteState() && USB0.INTSTS0.BIT.DVSQ);
}

#endif
#endif // whole file

#ifdef HAVE_HWSERIAL0
extern "C"{
void ReadBulkOUTPacket(void)
{
    uint16_t DataLength = 0;

    /*Read data using D1FIFO*/
    /*NOTE: This probably will have already been selected if using BRDY interrupt.*/
    do{
    USBIO.D1FIFOSEL.BIT.CURPIPE = PIPE_BULK_OUT;
    }while(USBIO.D1FIFOSEL.BIT.CURPIPE != PIPE_BULK_OUT);

    /*Set PID to BUF*/
    USBIO.PIPE1CTR.BIT.PID = PID_BUF;

    /*Wait for buffer to be ready*/
    while(USBIO.D1FIFOCTR.BIT.FRDY == 0){;}

    /*Set Read Count Mode - so DTLN count will decrement as data read from buffer*/
    USBIO.D1FIFOSEL.BIT.RCNT = 1;

    /*Read length of data */
    DataLength = USBIO.D1FIFOCTR.BIT.DTLN;

    if( DataLength == 0 ) {
        USBIO.D1FIFOCTR.BIT.BCLR = 1;
        return;
    }

    while(DataLength != 0){
        /*Read from the FIFO*/
        uint16_t Data = USBIO.D1FIFO.WORD;
        if(DataLength >= 2){
            /*Save first byte*/
            Serial._store_char((uint8_t)Data);
            /*Save second byte*/
            Serial._store_char((uint8_t)(Data>>8));
            DataLength-=2;
        } else {
            Serial._store_char((uint8_t)Data);
            DataLength--;
        }
    }

}
void WriteBulkINPacket(void)
{
    uint32_t Count = 0;

    /*Write data to Bulk IN pipe using D0FIFO*/
    /*Select pipe (Check this happens before continuing)*/
    /*Set 8 bit access*/
    USBIO.D0FIFOSEL.BIT.MBW = 0;
    do{
        USBIO.D0FIFOSEL.BIT.CURPIPE = PIPE_BULK_IN;
    }while(USBIO.D0FIFOSEL.BIT.CURPIPE != PIPE_BULK_IN);


    /*Wait for buffer to be ready*/
    while(USBIO.D0FIFOCTR.BIT.FRDY == 0){;}

    /* Write data to the IN Fifo until have written a full packet
     or we have no more data to write */
    while((Count < BULK_IN_PACKET_SIZE) && Serial._buffer_available())
    {
        USBIO.D0FIFO.WORD = Serial._extract_char();
        Count++;
    }

    /*Send the packet */
    /*Set PID to BUF*/
    USBIO.PIPE2CTR.BIT.PID = PID_BUF;

    /*If we have not written a full packets worth to the buffer then need to
    signal that the buffer is now ready to be sent, set the buffer valid flag (BVAL).*/
    if(Count != BULK_IN_PACKET_SIZE)
    {
        USBIO.D0FIFOCTR.BIT.BVAL = 1;
    }

    if(!Serial._buffer_available())
    {
        USBIO.BRDYENB.BIT.PIPE2BRDYE = 0;
    }
}

} // extern C

#endif/*HAVE_HWSERIAL0*/

#ifdef HAVE_HWSERIAL1
extern "C"
void isr_serial1_receive(void) __attribute__((interrupt(".rvectors", VECT(SCI0, RXI0)), used));
void isr_serial1_receive(void)
{
  Serial1._rx_complete_irq();
}

extern "C"
void isr_serial1_transmit(void) __attribute__((interrupt(".rvectors", VECT(SCI0, TXI0)), used));
void isr_serial1_transmit()
{
  Serial1._tx_udr_empty_irq();
}

bool Serial1_available() {
  return Serial1.available();
}
#endif/*HAVE_HWSERIAL1*/

#ifdef HAVE_HWSERIAL2
extern "C"
void isr_serial2_receive() __attribute__((interrupt(".rvectors", VECT(SCI2, RXI2)), used));
void isr_serial2_receive()
{
  Serial2._rx_complete_irq();
}

extern "C"
void isr_serial2_transmit() __attribute__((interrupt(".rvectors", VECT(SCI2, TXI2)), used));
void isr_serial2_transmit()
{
  Serial2._tx_udr_empty_irq();
}

bool Serial2_available() {
  return Serial2.available();
}

#endif/*HAVE_HWSERIAL2*/

#ifdef HAVE_HWSERIAL3
extern "C"
void isr_serial3_receive() __attribute__((interrupt(".rvectors", VECT(SCI5, RXI5)), used));
void isr_serial3_receive()
{
  Serial3._rx_complete_irq();
}

extern "C"
void isr_serial3_transmit() __attribute__((interrupt(".rvectors", VECT(SCI5, TXI5)), used));
void isr_serial3_transmit()
{
  Serial3._tx_udr_empty_irq();
}
bool Serial3_available() {
  return Serial3.available();
}

#endif/*HAVE_HWSERIAL3*/

#ifdef HAVE_HWSERIAL4
extern "C"
void isr_serial4_receive() __attribute__((interrupt(".rvectors", VECT(SCI6, RXI6)), used));
void isr_serial4_receive()
{
  Serial4._rx_complete_irq();
}

extern "C"
void isr_serial4_transmit() __attribute__((interrupt(".rvectors", VECT(SCI6, TXI6)), used));
void isr_serial4_transmit()
{
  Serial4._tx_udr_empty_irq();
}

bool Serial4_available() {
  return Serial4.available();
}

#endif/*HAVE_HWSERIAL4*/

#ifdef HAVE_HWSERIAL5
extern "C"
void isr_serial5_receive() __attribute__((interrupt(".rvectors", VECT(SCI1, RXI1)), used));
void isr_serial5_receive()
{
  Serial5._rx_complete_irq();
}

extern "C"
void isr_serial5_transmit() __attribute__((interrupt(".rvectors", VECT(SCI1, TXI1)), used));
void isr_serial5_transmit()
{
  Serial5._tx_udr_empty_irq();
}

bool Serial5_available() {
  return Serial5.available();
}

#endif/*HAVE_HWSERIAL5*/

#ifdef HAVE_HWSERIAL6
extern "C"
void isr_serial6_receive() __attribute__((interrupt(".rvectors", VECT(SCI3, RXI3)), used));
void isr_serial6_receive()
{
  Serial6._rx_complete_irq();
}

extern "C"
void isr_serial6_transmit() __attribute__((interrupt(".rvectors", VECT(SCI3, TXI3)), used));
void isr_serial6_transmit()
{
  Serial6._tx_udr_empty_irq();
}

bool Serial6_available() {
  return Serial6.available();
}

#endif/*HAVE_HWSERIAL6*/

#ifdef HAVE_HWSERIAL7
extern "C"
void isr_serial7_receive() __attribute__((interrupt(".rvectors", VECT(SCI8, RXI8)), used));
void isr_serial7_receive()
{
  Serial7._rx_complete_irq();
}

extern "C"
void isr_serial7_transmit() __attribute__((interrupt(".rvectors", VECT(SCI8, TXI8)), used));
void isr_serial7_transmit()
{
  SCI8.SCR.BIT.TEIE = 1; // for TEND interrupt to switch the direction of RS-485 from OUTPUT to INPUT
  Serial7._tx_udr_empty_irq();
}

bool Serial7_available() {
  return Serial7.available();
}

#endif/*HAVE_HWSERIAL7*/

void (*hook_bl0_handler)();
void (*hook_bl1_handler)();
extern "C"
void group_bl0_handler_isr(void) __attribute__((interrupt(".rvectors", VECT(ICU, GROUPBL0)), used));
void group_bl0_handler_isr(void){
	if(hook_bl0_handler != NULL){
		(*hook_bl0_handler)();
	}
    if(ICU.GRPBL0.BIT.IS1 == 1){
        SCI0.SSR.BYTE &= 0b11000111;
    }
    if(ICU.GRPBL0.BIT.IS3 == 1){
        SCI1.SSR.BYTE &= 0b11000111;
    }
    if(ICU.GRPBL0.BIT.IS5 == 1){
        SCI2.SSR.BYTE &= 0b11000111;
    }
    if(ICU.GRPBL0.BIT.IS7 == 1){
        SCI3.SSR.BYTE &= 0b11000111;
    }
    if(ICU.GRPBL0.BIT.IS9 == 1){
        SCI4.SSR.BYTE &= 0b11000111;
    }
    if(ICU.GRPBL0.BIT.IS11 == 1){
        SCI5.SSR.BYTE &= 0b11000111;
    }
    if(ICU.GRPBL0.BIT.IS13 == 1){
        SCI6.SSR.BYTE &= 0b11000111;
    }
    if(ICU.GRPBL0.BIT.IS15 == 1){
        SCI7.SSR.BYTE &= 0b11000111;
    }
}

extern "C"
void group_bl1_handler_isr(void) __attribute__((interrupt(".rvectors", VECT(ICU, GROUPBL1)), used));
void group_bl1_handler_isr(void){
	if(hook_bl1_handler != NULL){
		(*hook_bl1_handler)();
	}
    if(ICU.GRPBL1.BIT.IS24 == 1){ // TEND interrupt for switching input.
    	PORTC.PODR.BIT.B5 = LOW; // Switch direction of RS-485 to INPUT
		SCI8.SCR.BIT.TEIE = 0;
    }
    if(ICU.GRPBL1.BIT.IS25 == 1){
        SCI8.SSR.BYTE &= 0b11000111;
    }
}
