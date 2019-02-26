/*
  HardwareSerial_private.h - Hardware serial library for Wiring
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
  Modified 13 July 2014 by Nozomu Fujita for GR-SAKURA
*/

#include "wiring_private.h"

// this next line disables the entire HardwareSerial.cpp, 
// this is so I can support Attiny series and any other chip without a uart
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3) || defined(HAVE_HWSERIAL4) || defined(HAVE_HWSERIAL5) || defined(HAVE_HWSERIAL6)  || defined(HAVE_HWSERIAL7)

#ifndef __RX600__
// Ensure that the various bit positions we use are available with a 0
// postfix, so we can always use the values for UART0 for all UARTs. The
// alternative, passing the various values for each UART to the
// HardwareSerial constructor also works, but makes the code bigger and
// slower.
#if !defined(TXC0)
#if defined(TXC)
// On ATmega8, the uart and its bits are not numbered, so there is no TXC0 etc.
#define TXC0 TXC
#define RXEN0 RXEN
#define TXEN0 TXEN
#define RXCIE0 RXCIE
#define UDRIE0 UDRIE
#define U2X0 U2X
#define UPE0 UPE
#define UDRE0 UDRE
#elif defined(TXC1)
// Some devices have uart1 but no uart0
#define TXC0 TXC1
#define RXEN0 RXEN1
#define TXEN0 TXEN1
#define RXCIE0 RXCIE1
#define UDRIE0 UDRIE1
#define U2X0 U2X1
#define UPE0 UPE1
#define UDRE0 UDRE1
#else
#error No UART found in HardwareSerial.cpp
#endif
#endif // !defined TXC0
#endif/*__RX600__*/

// Check at compiletime that it is really ok to use the bit positions of
// UART0 for the other UARTs as well, in case these values ever get
// changed for future hardware.
#if defined(TXC1) && (TXC1 != TXC0 || RXEN1 != RXEN0 || RXCIE1 != RXCIE0 || \
		      UDRIE1 != UDRIE0 || U2X1 != U2X0 || UPE1 != UPE0 || \
		      UDRE1 != UDRE0)
#error "Not all bit positions for UART1 are the same as for UART0"
#endif
#if defined(TXC2) && (TXC2 != TXC0 || RXEN2 != RXEN0 || RXCIE2 != RXCIE0 || \
		      UDRIE2 != UDRIE0 || U2X2 != U2X0 || UPE2 != UPE0 || \
		      UDRE2 != UDRE0)
#error "Not all bit positions for UART2 are the same as for UART0"
#endif
#if defined(TXC3) && (TXC3 != TXC0 || RXEN3 != RXEN0 || RXCIE3 != RXCIE0 || \
		      UDRIE3 != UDRIE0 || U3X3 != U3X0 || UPE3 != UPE0 || \
		      UDRE3 != UDRE0)
#error "Not all bit positions for UART3 are the same as for UART0"
#endif

// Constructors ////////////////////////////////////////////////////////////////

#ifndef __RX600__
HardwareSerial::HardwareSerial(
  volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
  volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
  volatile uint8_t *ucsrc, volatile uint8_t *udr) :
    _ubrrh(ubrrh), _ubrrl(ubrrl),
    _ucsra(ucsra), _ucsrb(ucsrb), _ucsrc(ucsrc),
    _udr(udr),
    _rx_buffer_head(0), _rx_buffer_tail(0),
    _tx_buffer_head(0), _tx_buffer_tail(0)
{
}
#else /*__RX600__*/
HardwareSerial::HardwareSerial(
  int8_t serial_channel,
  volatile st_sci0* sci,
  MstpId module,
  uint8_t txpin,
  uint8_t rxpin,
  uint8_t output_mode) :
    _sci(sci),
    _module(module),
    _txpin(txpin),
	_rxpin(rxpin),
	_output_mode(output_mode),
    _serial_channel(serial_channel),
    _sending(false),
    _begin(false),
    _rx_buffer_head(0), _rx_buffer_tail(0),
    _tx_buffer_head(0), _tx_buffer_tail(0)
{
}
#endif/*__RX600__*/

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial::_rx_complete_irq(void)
{
#ifndef __RX600__
  if (bit_is_clear(*_ucsra, UPE0)) {
    // No Parity error, read byte and store it in the buffer if there is
    // room
    unsigned char c = *_udr;
    uint8_t i = (unsigned int)(_rx_buffer_head + 1) % SERIAL_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer_tail) {
      _rx_buffer[_rx_buffer_head] = c;
      _rx_buffer_head = i;
    }
  } else {
    // Parity error, read byte but discard it
    *_udr;
  };
#else /*__RX600__*/
  unsigned char c = _sci->RDR;
  if (_sci->SSR.BIT.ORER == 0 && _sci->SSR.BIT.FER == 0 && _sci->SSR.BIT.PER == 0) {
    _store_char(c);
  } else {

    while (_sci->SSR.BIT.ORER) {
    	_sci->SSR.BIT.ORER = 0;
    }
    while (_sci->SSR.BIT.FER) {
    	_sci->SSR.BIT.FER = 0;
    }
    while (_sci->SSR.BIT.PER) {
    	_sci->SSR.BIT.PER = 0;
    }
	_sci->SSR.BIT.RDRF = 1;
	_sci->SSR.BIT.TDRE = 1;
  }
#endif/*__RX600__*/
}

#ifdef __RX600__
int HardwareSerial::_store_char(unsigned char c)
{
#if SERIAL_BUFFER_SIZE < 256
  uint8_t i = (uint8_t)(_rx_buffer_head + 1) % SERIAL_BUFFER_SIZE;
#else
  uint32_t i = (uint32_t)(_rx_buffer_head + 1) % SERIAL_BUFFER_SIZE;
#endif
  if (i != _rx_buffer_tail) {
    _rx_buffer[_rx_buffer_head] = c;
    _rx_buffer_head = i;
    return 1;
  } else {
    return 0;
  }
}

unsigned char HardwareSerial::_extract_char()
{
    unsigned char c = _tx_buffer[_tx_buffer_tail];
    if (_tx_buffer_head != _tx_buffer_tail) {
      _tx_buffer_tail = (_tx_buffer_tail + 1) % SERIAL_BUFFER_SIZE;
    }
    return c;
}
bool HardwareSerial::_buffer_available()
{
    return (_tx_buffer_head != _tx_buffer_tail);
}

#endif/*__RX600__*/

#endif // whole file
