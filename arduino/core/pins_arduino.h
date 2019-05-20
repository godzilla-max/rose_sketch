/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

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

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/
/*
  Modified 12 July 2018 by Yuuki Okamiya for GR-ROSE
  Modified 13 July 2014 by Nozomu Fujita
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#ifndef __RX600__
#include <avr/pgmspace.h>
#else /*__RX600__*/
#include "rx65n/typedefine.h"
#endif/*__RX600__*/

#define NUM_DIGITAL_PINS            40
#define NUM_ANALOG_INPUTS           10
#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + 14 : -1)
#define INVALID_IO (-1)

#if defined(__AVR_ATmega8__)
#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 11)
#elif defined(GRROSE)
#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 11)
#else
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)
#endif

#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)
#define PIN_A0        (14)
#define PIN_A1        (15)
#define PIN_A2        (16)
#define PIN_A3        (17)
#define PIN_A4        (18)
#define PIN_A5        (19)
#define PIN_WIRE_SCL  (20)
#define PIN_WIRE_SDA  (21)
#define PIN_RS485_RX  (22)
#define PIN_RS485_TX  (23)
#define PIN_DAC       (24)
#define PIN_LED1      (25)
#define PIN_LED2      (26)
#define PIN_ESP_IO0   (27)
#define PIN_ESP_EN    (28)
#define PIN_ESP_IO15  (29)
#define PIN_ESP_RX    (30)
#define PIN_ESP_TX    (31)
#define PIN_ESP_RES   (32)
#define PIN_S1_SEL    (33)
#define PIN_S2_SEL    (34)
#define PIN_S3_SEL    (35)
#define PIN_S4_SEL    (36)
#define PIN_RS485_DIR (37)
#define PIN_ANINT     (38)
#define PIN_ANTMP     (39)

//extra analog pin
#define PIN_A6        (10)
#define PIN_A7        (11)
#define PIN_A8        (12)
#define PIN_A9        (13)
//pins for PPG library
#define PIN_PPG_PH0   (1)
#define PIN_PPG_PH1   (3)
#define PIN_PPG_PH2   (5)
#define PIN_PPG_PH3   (7)
#define NUM_PPG_PHASE (4)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

static const uint8_t SCL = PIN_WIRE_SCL;
static const uint8_t SDA = PIN_WIRE_SDA;

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
static const uint8_t A8 = PIN_A8;
static const uint8_t A9 = PIN_A9;

#ifndef GRROSE
#define LED_BUILTIN 13
#else
#define LED_BUILTIN PIN_LED1
#endif

// TODO : Confirm pins
#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)

// ATMEL ATMEGA1280 / ARDUINO
//
// 0-7 PE0-PE7   works
// 8-13 PB0-PB5  works
// 14-21 PA0-PA7 works 
// 22-29 PH0-PH7 works
// 30-35 PG5-PG0 works
// 36-43 PC7-PC0 works
// 44-51 PJ7-PJ0 works
// 52-59 PL7-PL0 works
// 60-67 PD7-PD0 works
// A0-A7 PF0-PF7
// A8-A15 PK0-PK7


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
#ifndef __RX600__
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};
#endif/*__RX600__*/

#define _offset(port) ((uint32_t)&port - (uint32_t)&PORT0)
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
    _offset(PORT2),//0: Serial1 RX
    _offset(PORT2),//1: Serial1 TX
    _offset(PORT1),//2: Serial2 RX
    _offset(PORT1),//3: Serial2 TX
    _offset(PORTC),//4: Serial3 RX
    _offset(PORTC),//5: Serial3 TX
    _offset(PORT3),//6: Serial4 RX
    _offset(PORT3),//7: Serial4 TX
    _offset(PORT3),//8: Serial5 RX
    _offset(PORT2),//9: Serial5 TX
    _offset(PORTE),//10: SPI SS
    _offset(PORTE),//11: SPI MO
    _offset(PORTE),//12: SPI MI
    _offset(PORTE),//13: SPI CK
    _offset(PORTD),//14: A0
    _offset(PORTD),//15: A1
    _offset(PORTD),//16: A2
    _offset(PORTD),//17: A3
    _offset(PORTD),//18: A4
    _offset(PORTD),//19: A5
    _offset(PORT5),//20: Wire CL
    _offset(PORT5),//21: Wire DA
    _offset(PORTC),//22: Serial7 RX
    _offset(PORTC),//23: Serial7 TX
    _offset(PORT0),//24: DAC
    _offset(PORTA),//25: LED1
    _offset(PORTA),//26: LED2
    _offset(PORT2),//27: ESP_IO0
    _offset(PORT2),//28: ESP_EN
    _offset(PORT3),//29: ESP_IO15
    _offset(PORT2),//30: ESP_RX
    _offset(PORT2),//31: ESP_TX
    _offset(PORT1),//32: ESP_RES
    _offset(PORT2),//33: S1_SEL
    _offset(PORT1),//34: S2_SEL
    _offset(PORTC),//35: S3_SEL
    _offset(PORT3),//36: S4_SEL
    _offset(PORTC),//37: RS485_SEL
};
#undef _offset

const uint8_t PROGMEM digital_pin_to_bit_PGM[] = {
    1,//0
    0,//1
    2,//2
    3,//3
    2,//4
    3,//5
    3,//6
    2,//7
    0,//8
    6,//9
    4,//10
    6,//11
    7,//12
    5,//13
    2,//14
    3,//15
    4,//16
    5,//17
    6,//18
    7,//19
    2,//20
    0,//21
    6,//22
    7,//23
    5,//24
    0,//25
    1,//26
    7,//27
    4,//28
    1,//29
    5,//30
    3,//31
    7,//32
    2,//33
    4,//34
    4,//35
    4,//36
    5,//37
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
    _BV(1),
    _BV(0),
    _BV(2),
    _BV(3),
    _BV(2),
    _BV(3),
    _BV(3),
    _BV(2),
    _BV(0),
    _BV(6),
    _BV(4),
    _BV(6),
    _BV(7),
    _BV(5),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    _BV(2),
    _BV(0),
    _BV(6),
    _BV(7),
    _BV(5),
    _BV(0),
    _BV(1),
    _BV(7),
    _BV(4),
    _BV(1),
    _BV(5),
    _BV(3),
    _BV(7),
    _BV(2),
    _BV(4),
    _BV(4),
    _BV(4),
    _BV(5),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
#ifndef __RX600__
	NOT_ON_TIMER, /* 0 - port D */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	// on the ATmega168, digital pin 3 has hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
#else
	TIMER2B,
#endif
	NOT_ON_TIMER,
	// on the ATmega168, digital pins 5 and 6 have hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
	NOT_ON_TIMER,
#else
	TIMER0B,
	TIMER0A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 8 - port B */
	TIMER1A,
	TIMER1B,
#if defined(__AVR_ATmega8__)
	TIMER2,
#else
	TIMER2A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 14 - port C */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
#else /*__RX600__*/
#endif/*__RX600__*/
};

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
