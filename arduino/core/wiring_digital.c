/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/

#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"

#ifdef __RX600__
#include "util.h"

static void _setPinModeCommon(int pin, PinMode mode)
{
	uint8_t bit = digitalPinToBit(pin);
	uint8_t port = digitalPinToPort(pin);

	if (port == NOT_A_PIN) return;

	volatile uint8_t* pmr = portModeRegister(port);
	volatile uint8_t* pdr = portDirectionRegister(port);
	volatile uint8_t* pcr = portPullupControlRegister(port);

	BCLR(pmr, bit);
	if (mode == PinModeInput || mode == PinModeInputPullUp) {
		BCLR(pdr, bit);
		if (mode == PinModeInputPullUp) {
			BSET(pcr, bit);
		} else {
			BCLR(pcr, bit);
		}
	} else if (mode == PinModeOutput || mode == PinModeOutputHigh || mode == PinModeOutputOpenDrain) {
		volatile uint8_t* odr = portOpendrainRegister(port, bit);
		volatile uint8_t* dscr = portDriveSpecControlRegister(port);

		BSET(pdr, bit);
		BCLR(pcr, bit);
		if (mode == PinModeOutputHigh) {
			BSET(dscr, bit);
		} else {
			BCLR(dscr, bit);
		}
		if (mode == PinModeOutputOpenDrain) {
			BSET(odr, 2 * bit);
		} else {
			BCLR(odr, 2 * bit);
		}
	}
}

static void _resetPinModeCommon(int pin, PinMode mode)
{
	(void)pin;
	(void)mode;
}

void setPinModeInput(int pin)
{
	_setPinModeCommon(pin, PinModeInput);
}

void resetPinModeInput(int pin)
{
	_resetPinModeCommon(pin, PinModeInput);
}

void setPinModeOutput(int pin)
{
	_setPinModeCommon(pin, PinModeOutput);
}

void resetPinModeOutput(int pin)
{
	_resetPinModeCommon(pin, PinModeOutput);
}

void setPinModeInputPullUp(int pin)
{
	_setPinModeCommon(pin, PinModeInputPullUp);
}

void resetPinModeInputPullUp(int pin)
{
	_resetPinModeCommon(pin, PinModeInputPullUp);
}

void setPinModeOutputHigh(int pin)
{
	_setPinModeCommon(pin, PinModeOutputHigh);
}

void resetPinModeOutputHigh(int pin)
{
	_resetPinModeCommon(pin, PinModeOutputHigh);
}

void setPinModeOutputOpenDrain(int pin)
{
	_setPinModeCommon(pin, PinModeOutputOpenDrain);
}

void resetPinModeOutputOpenDrain(int pin)
{
	_resetPinModeCommon(pin, PinModeOutputOpenDrain);
}
#endif/*__RX600__*/

void pinMode(uint8_t pin, uint8_t mode)
{
#ifndef __RX600__
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg, *out;

	if (port == NOT_A_PIN) return;

	// JWS: can I let the optimizer do this?
	reg = portModeRegister(port);
	out = portOutputRegister(port);

	if (mode == INPUT) { 
		uint8_t oldSREG = SREG;
                cli();
		*reg &= ~bit;
		*out &= ~bit;
		SREG = oldSREG;
	} else if (mode == INPUT_PULLUP) {
		uint8_t oldSREG = SREG;
                cli();
		*reg &= ~bit;
		*out |= bit;
		SREG = oldSREG;
	} else {
		uint8_t oldSREG = SREG;
                cli();
		*reg |= bit;
		SREG = oldSREG;
	}
#else /*__RX600__*/
	switch (mode) {
	case INPUT:
		setPinMode(pin, PinModeInput);
		break;
	case OUTPUT:
		setPinMode(pin, PinModeOutput);
		break;
	case INPUT_PULLUP:
		setPinMode(pin, PinModeInputPullUp);
		break;
	case OUTPUT_HIGH:
		setPinMode(pin, PinModeOutputHigh);
		break;
	case OUTPUT_OPENDRAIN:
		setPinMode(pin, PinModeOutputOpenDrain);
		break;
	}
#endif/*__RX600__*/
}

// Forcing this inline keeps the callers from having to push their own stuff
// on the stack. It is a good performance win and only takes 1 more byte per
// user than calling. (It will take more bytes on the 168.)
//
// But shouldn't this be moved into pinMode? Seems silly to check and do on
// each digitalread or write.
//
// Mark Sproul:
// - Removed inline. Save 170 bytes on atmega1280
// - changed to a switch statment; added 32 bytes but much easier to read and maintain.
// - Added more #ifdefs, now compiles for atmega645
//
//static inline void turnOffPWM(uint8_t timer) __attribute__ ((always_inline));
//static inline void turnOffPWM(uint8_t timer)
static void turnOffPWM(uint8_t timer)
{
#ifndef __RX600__
	switch (timer)
	{
		#if defined(TCCR1A) && defined(COM1A1)
		case TIMER1A:   cbi(TCCR1A, COM1A1);    break;
		#endif
		#if defined(TCCR1A) && defined(COM1B1)
		case TIMER1B:   cbi(TCCR1A, COM1B1);    break;
		#endif
		
		#if defined(TCCR2) && defined(COM21)
		case  TIMER2:   cbi(TCCR2, COM21);      break;
		#endif
		
		#if defined(TCCR0A) && defined(COM0A1)
		case  TIMER0A:  cbi(TCCR0A, COM0A1);    break;
		#endif
		
		#if defined(TIMER0B) && defined(COM0B1)
		case  TIMER0B:  cbi(TCCR0A, COM0B1);    break;
		#endif
		#if defined(TCCR2A) && defined(COM2A1)
		case  TIMER2A:  cbi(TCCR2A, COM2A1);    break;
		#endif
		#if defined(TCCR2A) && defined(COM2B1)
		case  TIMER2B:  cbi(TCCR2A, COM2B1);    break;
		#endif
		
		#if defined(TCCR3A) && defined(COM3A1)
		case  TIMER3A:  cbi(TCCR3A, COM3A1);    break;
		#endif
		#if defined(TCCR3A) && defined(COM3B1)
		case  TIMER3B:  cbi(TCCR3A, COM3B1);    break;
		#endif
		#if defined(TCCR3A) && defined(COM3C1)
		case  TIMER3C:  cbi(TCCR3A, COM3C1);    break;
		#endif

		#if defined(TCCR4A) && defined(COM4A1)
		case  TIMER4A:  cbi(TCCR4A, COM4A1);    break;
		#endif					
		#if defined(TCCR4A) && defined(COM4B1)
		case  TIMER4B:  cbi(TCCR4A, COM4B1);    break;
		#endif
		#if defined(TCCR4A) && defined(COM4C1)
		case  TIMER4C:  cbi(TCCR4A, COM4C1);    break;
		#endif			
		#if defined(TCCR4C) && defined(COM4D1)
		case TIMER4D:	cbi(TCCR4C, COM4D1);	break;
		#endif			
			
		#if defined(TCCR5A)
		case  TIMER5A:  cbi(TCCR5A, COM5A1);    break;
		case  TIMER5B:  cbi(TCCR5A, COM5B1);    break;
		case  TIMER5C:  cbi(TCCR5A, COM5C1);    break;
		#endif
	}
#else /*__RX600__*/
	(void)timer;
#endif/*__RX600__*/
}

void digitalWrite(uint8_t pin, uint8_t val)
{
	uint8_t timer = digitalPinToTimer(pin);
#ifndef __RX600__
	uint8_t bit = digitalPinToBitMask(pin);
#else /*__RX600__*/
	uint8_t bit = digitalPinToBit(pin);
#endif/*__RX600__*/
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out;

	if (port == NOT_A_PIN) return;

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	out = portOutputRegister(port);

#ifndef __RX600__
	uint8_t oldSREG = SREG;
	cli();

	if (val == LOW) {
		*out &= ~bit;
	} else {
		*out |= bit;
	}

	SREG = oldSREG;
#else /*__RX600__*/
	switch (getPinMode(pin)) {
	case PinModeOutput:
	case PinModeOutputHigh:
	case PinModeOutputOpenDrain:
	default:
		if (val == LOW) {
			BCLR(out, bit);
		} else {
			BSET(out, bit);
		}
		break;
	case PinModeInput:
		if (val != LOW) {
			volatile uint8_t* pcr = portPullupControlRegister(port);
			BSET(pcr, bit);
			changePinMode(pin, PinModeInputPullUp);
		}
		break;
	case PinModeInputPullUp:
		if (val == LOW) {
			volatile uint8_t* pcr = portPullupControlRegister(port);
			BCLR(pcr, bit);
			changePinMode(pin, PinModeInput);
		}
		break;
	}
#endif/*__RX600__*/
}

int digitalRead(uint8_t pin)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

	if (port == NOT_A_PIN) return LOW;

	// If the pin that support PWM output, we need to turn it off
	// before getting a digital reading.
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	if (*portInputRegister(port) & bit) return HIGH;
	return LOW;
}
