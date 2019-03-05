/*
  wiring_analog.c - analog input and output
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

#include "wiring_private.h"
#include "pins_arduino.h"
#ifdef __RX600__
#include "Arduino.h"
#include "utilities.h"
#include "rx65n/interrupt_handlers.h"
#include "util.h"
#endif/*__RX600__*/

static uint8_t analog_reference = DEFAULT;
static uint16_t table_ansa[NUM_ANALOG_INPUTS] = {10, 11, 12, 13, 6, 7}; // ex A0 = PD2/AN110, A1 = PD3/AN111

void analogReference(uint8_t mode)
{
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.
	analog_reference = mode;
}

#ifdef __RX600__
void setPinModeAnalogRead(int pin)
{
	int an = pin - PIN_A0;
	if (an >= 0 && an <= (NUM_ANALOG_INPUTS - 1)) {
		BCLR(&PORTD.PDR.BYTE, an + 2); // Analog pins are from PD2 to PD7
		BSET(&PORTD.PMR.BYTE, an + 2);
		assignPinFunction(pin, 0, 0, 1);
	}
}

void resetPinModeAnalogRead(int pin)
{
	(void)pin;
}
#endif/*__RX600__*/

int analogRead(uint8_t pin)
{
#ifndef __RX600__
	uint8_t low, high;

#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#endif
	pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
	if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#else
	if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif

#if defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
  
	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
#if defined(ADMUX)
	ADMUX = (analog_reference << 6) | (pin & 0x07);
#endif

	// without a delay, we seem to read from the wrong channel
	//delay(1);

#if defined(ADCSRA) && defined(ADCL)
	// start the conversion
	sbi(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));

	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	low  = ADCL;
	high = ADCH;
#else
	// we dont have an ADC, return 0
	low  = 0;
	high = 0;
#endif

	// combine the two bytes
	return (high << 8) | low;
#else /*__RX600__*/
	volatile uint16_t* adcdr = NULL;

    if (pin < A0) pin += A0; // allow for channel or pin numbers

	if (pin >= A0 && pin <= (A0 + NUM_ANALOG_INPUTS)) {
	    if (getPinMode(pin) != PinModeAnalogRead){
			setPinMode(pin, PinModeAnalogRead);
			startModule(MstpIdS12AD1);
			S12AD1.ADCSR.WORD = 0x00;
			S12AD1.ADADC.BIT.ADC = 0b000;
			S12AD1.ADCER.BIT.ADRFMT = 0;
			S12AD1.ADCER.BIT.ACE = 0;
	    }
		int an = pin - A0; // for setting offset of register address and bit
		S12AD1.ADANSA0.WORD = 1 << table_ansa[an];
		adcdr = (volatile uint16_t*)&S12AD1.ADDR0 + table_ansa[an];
	}
	if (pin == PIN_ANINT) {
		startModule(MstpIdS12AD1);
		S12AD1.ADEXICR.BIT.TSSA = 1;
		S12AD1.ADEXICR.BIT.OCSA = 0;
		S12AD1.ADEXICR.BIT.TSSAD = 1;
		S12AD1.ADEXICR.BIT.OCSAD = 0;
		S12AD.ADANSA0.WORD = 0;
		S12AD1.ADANSA1.WORD = 0;
		adcdr = &S12AD1.ADOCDR;
	}
	if (pin == PIN_ANTMP) {
		startModule(MstpIdS12AD1);
		S12AD1.ADEXICR.BIT.TSSA = 0;
		S12AD1.ADEXICR.BIT.OCSA = 1;
		S12AD1.ADEXICR.BIT.TSSAD = 0;
		S12AD1.ADEXICR.BIT.OCSAD = 1;
		S12AD.ADANSA0.WORD = 0;
		S12AD1.ADANSA1.WORD = 0;
		adcdr = &S12AD1.ADTSDR;
	}
	S12AD1.ADCSR.BIT.ADST = 1;
	while (S12AD1.ADCSR.BIT.ADST) {
		;
	}

	int val = *adcdr & 0x0fff;
	switch (analog_reference) {
	case DEFAULT:
		break;
	case INTERNAL:
		val = val * (1024 * 33) / (4096 * 11);
		if (val > 1023) {
			val = 1023;
		}
		break;
	case EXTERNAL:
		val = val * 1024 / 4096;
		break;
	case RAW12BIT:
		break;
	}
	return val;
#endif/*__RX600__*/
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint8_t pin, int val)
{
#ifndef __RX600__
	// We need to make sure the PWM output is enabled for those pins
	// that support it, as we turn it off when digitally reading or
	// writing with them.  Also, make sure the pin is in output mode
	// for consistenty with Wiring, which doesn't require a pinMode
	// call for the analog output pins.
	pinMode(pin, OUTPUT);
	if (val == 0)
	{
		digitalWrite(pin, LOW);
	}
	else if (val == 255)
	{
		digitalWrite(pin, HIGH);
	}
	else
	{
		switch(digitalPinToTimer(pin))
		{
			// XXX fix needed for atmega8
			#if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
			case TIMER0A:
				// connect pwm to pin on timer 0
				sbi(TCCR0, COM00);
				OCR0 = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR0A) && defined(COM0A1)
			case TIMER0A:
				// connect pwm to pin on timer 0, channel A
				sbi(TCCR0A, COM0A1);
				OCR0A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR0A) && defined(COM0B1)
			case TIMER0B:
				// connect pwm to pin on timer 0, channel B
				sbi(TCCR0A, COM0B1);
				OCR0B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR1A) && defined(COM1A1)
			case TIMER1A:
				// connect pwm to pin on timer 1, channel A
				sbi(TCCR1A, COM1A1);
				OCR1A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR1A) && defined(COM1B1)
			case TIMER1B:
				// connect pwm to pin on timer 1, channel B
				sbi(TCCR1A, COM1B1);
				OCR1B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2) && defined(COM21)
			case TIMER2:
				// connect pwm to pin on timer 2
				sbi(TCCR2, COM21);
				OCR2 = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2A) && defined(COM2A1)
			case TIMER2A:
				// connect pwm to pin on timer 2, channel A
				sbi(TCCR2A, COM2A1);
				OCR2A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2A) && defined(COM2B1)
			case TIMER2B:
				// connect pwm to pin on timer 2, channel B
				sbi(TCCR2A, COM2B1);
				OCR2B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3A1)
			case TIMER3A:
				// connect pwm to pin on timer 3, channel A
				sbi(TCCR3A, COM3A1);
				OCR3A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3B1)
			case TIMER3B:
				// connect pwm to pin on timer 3, channel B
				sbi(TCCR3A, COM3B1);
				OCR3B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3C1)
			case TIMER3C:
				// connect pwm to pin on timer 3, channel C
				sbi(TCCR3A, COM3C1);
				OCR3C = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR4A)
			case TIMER4A:
				//connect pwm to pin on timer 4, channel A
				sbi(TCCR4A, COM4A1);
				#if defined(COM4A0)		// only used on 32U4
				cbi(TCCR4A, COM4A0);
				#endif
				OCR4A = val;	// set pwm duty
				break;
			#endif
			
			#if defined(TCCR4A) && defined(COM4B1)
			case TIMER4B:
				// connect pwm to pin on timer 4, channel B
				sbi(TCCR4A, COM4B1);
				OCR4B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR4A) && defined(COM4C1)
			case TIMER4C:
				// connect pwm to pin on timer 4, channel C
				sbi(TCCR4A, COM4C1);
				OCR4C = val; // set pwm duty
				break;
			#endif
				
			#if defined(TCCR4C) && defined(COM4D1)
			case TIMER4D:				
				// connect pwm to pin on timer 4, channel D
				sbi(TCCR4C, COM4D1);
				#if defined(COM4D0)		// only used on 32U4
				cbi(TCCR4C, COM4D0);
				#endif
				OCR4D = val;	// set pwm duty
				break;
			#endif

							
			#if defined(TCCR5A) && defined(COM5A1)
			case TIMER5A:
				// connect pwm to pin on timer 5, channel A
				sbi(TCCR5A, COM5A1);
				OCR5A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR5A) && defined(COM5B1)
			case TIMER5B:
				// connect pwm to pin on timer 5, channel B
				sbi(TCCR5A, COM5B1);
				OCR5B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR5A) && defined(COM5C1)
			case TIMER5C:
				// connect pwm to pin on timer 5, channel C
				sbi(TCCR5A, COM5C1);
				OCR5C = val; // set pwm duty
				break;
			#endif

			case NOT_ON_TIMER:
			default:
				if (val < 128) {
					digitalWrite(pin, LOW);
				} else {
					digitalWrite(pin, HIGH);
				}
		}
	}
#else /*__RX600__*/
	bool hardPwm = isHardwarePWMPin(pin);
	int period;
	int term;
	if (hardPwm) {
		float m = getPWMClockMultiple(pin);
		period = (int)(255 * m);
		term = (int)(val * m);
	} else if(isDACPin(pin)){
		analogWriteDAC(pin, val);
		return;
	} else {
		period = 255;
		term = val;
	}
	if (getPinMode(pin) != PinModeAnalogWrite) {
		setPinMode(pin, PinModeAnalogWrite);
		if (hardPwm) {
			setPinModeHardwarePWM(pin, period, term, 0);
		} else {
			setPinModeSoftwarePWM(pin, period, term, 0);
		}
	} else {
		if (hardPwm) {
			changePinModeHardwarePWM(pin, period, term, 0);
		} else {
			changePinModeSoftwarePWM(pin, period, term, 0);
		}
	}
#endif/*__RX600__*/
}

#ifdef __RX600__
void analogWriteFrequency(uint32_t freq)
{
    uint32_t clock = 255 * freq;
    analogWriteClock(3, clock);
    analogWriteClock(7, clock);
    analogWriteClock(9, clock);
    analogWriteClock(10, clock);
    analogWriteClock(13, clock);
}

void analogWriteClock(uint8_t pin, uint32_t clock)
{
    setAnalogWriteClock(pin, clock);
}

/*
void setPinModeAnalogWrite(int pin)
{
}
*/

void resetPinModeAnalogWrite(int pin)
{
	if (isHardwarePWMPin(pin)) {
		resetPinModeHardwarePWM(pin);
	} else {
		resetPinModeSoftwarePWM(pin);
	}
}
#endif/*__RX600__*/

#ifdef __RX600__
void analogWriteDAC(int pin, int val)
{
	if (getPinMode(pin) != PinModeDac) {
		setPinMode(pin, PinModeDac);
	}
	DA.DADR0 = 0;
	DA.DADR1 = val;
	DA.DACR.BIT.DAE = 0;
	DA.DACR.BIT.DAOE0 = 0;
	DA.DACR.BIT.DAOE1 = 1;
	DA.DADPR.BIT.DPSEL = 0;
	DA.DAADSCR.BIT.DAADST = 0;
}

void setPinModeDac(int pin)
{
	if (isDACPin(pin)) {
		int port = digitalPinToPort(pin);
		int bit = digitalPinToBit(pin);
		startModule(MstpIdDA);
		BCLR(portModeRegister(port), bit);
		BCLR(portDirectionRegister(port), bit);
		assignPinFunction(pin, 0, 0, 1);
	}
}

void resetPinModeDac(int pin)
{
    if (isDACPin(pin)) {
		stopModule(MstpIdDA);
		pinMode(pin, INPUT);
	}
}
#endif/*__RX600__*/
