/*
  HardwareSerial.h - Hardware serial library for Wiring
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

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
  Modified 13 July 2014 by Nozomu Fujita for GR-SAKURA
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Stream.h"
#ifdef __RX600__
#include "util.h"
#endif/*__RX600__*/

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
#ifndef __RX600__
#if (RAMEND < 1000)
  #define SERIAL_BUFFER_SIZE 16
#else
  #define SERIAL_BUFFER_SIZE 64
#endif
#else /*__RX600__*/
  #define SERIAL_BUFFER_SIZE 1024
#endif/*__RX600__*/

// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class HardwareSerial : public Stream
{
  protected:
#ifndef __RX600__
    volatile uint8_t * const _ubrrh;
    volatile uint8_t * const _ubrrl;
    volatile uint8_t * const _ucsra;
    volatile uint8_t * const _ucsrb;
    volatile uint8_t * const _ucsrc;
    volatile uint8_t * const _udr;
    // Has any byte been written to the UART since begin()
    bool _written;
#else /*__RX600__*/
    volatile st_sci0* _sci;
    MstpId _module;
    uint8_t _txpin;
    uint8_t _rxpin;
    int8_t _serial_channel;
    uint8_t _output_mode;
    volatile bool _sending;
    volatile bool _begin;
#endif/*__RX600__*/

#if SERIAL_BUFFER_SIZE < 256
    volatile uint8_t _rx_buffer_head;
    volatile uint8_t _rx_buffer_tail;
    volatile uint8_t _tx_buffer_head;
    volatile uint8_t _tx_buffer_tail;
#else
    volatile uint32_t _rx_buffer_head;
    volatile uint32_t _rx_buffer_tail;
    volatile uint32_t _tx_buffer_head;
    volatile uint32_t _tx_buffer_tail;
#endif

    // Don't put any members after these buffers, since only the first
    // 32 bytes of this struct can be accessed quickly using the ldd
    // instruction.
    unsigned char _rx_buffer[SERIAL_BUFFER_SIZE];
    unsigned char _tx_buffer[SERIAL_BUFFER_SIZE];

  public:
#ifndef __RX600__
     inline HardwareSerial(
      volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *ucsrc, volatile uint8_t *udr);
#else /*__RX600__*/
     HardwareSerial(
      int8_t serial_channel,
      volatile st_sci0* sci,
      MstpId module,
	  uint8_t txpin,
	  uint8_t rxpin,
	  uint8_t output_mode
	  );
     virtual ~HardwareSerial(){};// added to remove warning in __RX600__
#endif/*__RX600__*/
    void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
    void begin(unsigned long, uint8_t);
    void end();
    void direction(uint8_t);
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() { return true; }

#ifdef GRROSE
    int getDteRate(void);
    int getDteDataBits(void);
    int getDteParityType(void);
    int getDteCharFormat(void);
    bool getDteState(void);
    bool isConnected();
    uint8_t getDteConfig();
#endif
    // Interrupt handlers - Not intended to be called externally
#ifndef __RX600__
    inline void _rx_complete_irq(void);
#else /*__RX600__*/
    void _rx_complete_irq(void);
    inline int _store_char(unsigned char c);
    inline unsigned char _extract_char();
    bool _buffer_available();
#endif/*__RX600__*/
    void _tx_udr_empty_irq(void);
};

#define HAVE_HWSERIAL0
#define HAVE_HWSERIAL1
#define HAVE_HWSERIAL2
#define HAVE_HWSERIAL3
#define HAVE_HWSERIAL4
#define HAVE_HWSERIAL5
#define HAVE_HWSERIAL6
#define HAVE_HWSERIAL7

#if defined(HAVE_HWSERIAL0)
  extern HardwareSerial Serial;
#endif
#if defined(HAVE_HWSERIAL1)
  extern HardwareSerial Serial1;
#endif
#if defined(HAVE_HWSERIAL2)
  extern HardwareSerial Serial2;
#endif
#if defined(HAVE_HWSERIAL3)
  extern HardwareSerial Serial3;
#endif
#if defined(HAVE_HWSERIAL4)
  extern HardwareSerial Serial4;
#endif
#if defined(HAVE_HWSERIAL5)
  extern HardwareSerial Serial5;
#endif
#if defined(HAVE_HWSERIAL6)
  extern HardwareSerial Serial6;
#endif
#if defined(HAVE_HWSERIAL7)
  extern HardwareSerial Serial7;
#endif

#endif/*__RX600__*/

extern void serialEventRun(void) __attribute__((weak));

