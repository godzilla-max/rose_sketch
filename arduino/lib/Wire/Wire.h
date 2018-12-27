/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
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

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2014 by Yuuki Okamiya for software I2C GR-SAKURA
  Modified 2014 by Yuuki Okamiya for add setFrequency for Hardware Wire
  Modified 10 Jun 2014 by Nozomu Fujita : Wire への TwoWire::setFrequency(int) 対応
  Modified 10 Jun 2014 by Nozomu Fujita : Wire3、Wire7 対応
  Modified 4th June 2016 by yuuki.okamiya : support slave function
*/

#ifndef TwoWire_h
#define TwoWire_h

#ifdef GRROSE
#define IIC_CHANNEL_NUM 9
#else
#define IIC_CHANNEL_NUM 9
#endif

#define HAVE_WIRE0
#define HAVE_WIRE1
#define HAVE_WIRE2
#define HAVE_WIRE3
//#define HAVE_RIIC


extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  #include "utility/twi_rx.h"
}

#include <inttypes.h>
#include "Stream.h"
#include "utility/I2cMaster.h"

#define BUFFER_LENGTH 32

class TwoWire : public Stream
{
  private:
      uint8_t rxBufferIndex;
      uint8_t rxBufferLength;

      uint8_t txAddress;
      uint8_t txBufferIndex;
      uint8_t txBufferLength;

      uint8_t transmitting;
      uint8_t wire_channel;
    int wire_frequency;
#ifdef HAVE_RIIC
    static void (*user_onRequest)(void);
    static void (*user_onReceive)(int);
    static void onRequestService(void);
    static void onReceiveService(uint8_t*, int);
#endif

  public:
    TwoWire(uint8_t channel);
    virtual ~TwoWire(){}; // to remove warning in GRSAKURA
    void begin();
    void begin(uint8_t);
    void begin(int);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void setFrequency(int freq);
#ifdef HAVE_RIIC
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );
#endif
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
};

#ifdef HAVE_WIRE0
extern TwoWire WireSoft;
#endif
#ifdef HAVE_WIRE1
extern TwoWire Wire;
#endif
#ifdef HAVE_WIRE2
extern TwoWire Wire2;
#endif
#ifdef HAVE_WIRE3
extern TwoWire Wire3;
#endif

#endif

