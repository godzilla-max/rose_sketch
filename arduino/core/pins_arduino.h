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

#ifndef GRROSE
#define NUM_DIGITAL_PINS            20
#define NUM_ANALOG_INPUTS           6
#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 14 : -1)
#else
#define NUM_DIGITAL_PINS            35
#define NUM_ANALOG_INPUTS           5
#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 14 : -1)
#define INVALID_IO (-1)

#endif

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

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#ifndef GRROSE
#define PIN_WIRE_SDA        (18)
#define PIN_WIRE_SCL        (19)
#else
#define PIN_WIRE_SDA        (21)
#define PIN_WIRE_SCL        (20)
#endif
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#ifndef GRROSE
#define LED_BUILTIN 13
#else
#define LED_BUILTIN 28
#endif

#ifndef GRROSE
#define HAVE_EXTRA_ANALOG_PIN
#endif
#define PIN_A0   (14)
#define PIN_A1   (15)
#define PIN_A2   (16)
#define PIN_A3   (17)
#define PIN_A4   (18)
#ifndef GRROSE
#define PIN_A5   (19)
#define PIN_A6   (20)
#define PIN_A7   (21)
#endif

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
#ifndef GRROSE
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
#endif

#ifdef GRROSE
#define PIN_LED0    28
#define PIN_LED1    29
#define PIN_ESP_IO0 26
#define PIN_ESP_EN  27
#define PIN_ESP_IO15  30
#endif
// ANALOG IO PINS -------------------------------------------------------------/

#define PIN_AN000     14
#define PIN_AN001     15
#define PIN_AN002     16
#define PIN_AN003     17
#define PIN_AN004     18
#ifndef GRROSE
#define PIN_AN005     19
#define PIN_AN006     20
#define PIN_AN007     21
#endif

#ifdef GRSAKURA
#define PIN_AN008     36
#define PIN_AN009     37
#define PIN_AN010     38
#define PIN_AN011     39
#define PIN_AN012     40
#define PIN_AN013     41
#elif defined(GRCITRUS)
#define PIN_AN008     22
#define PIN_AN009     23
#define PIN_AN010     24
#define PIN_AN011     25
#define PIN_AN012     26
#define PIN_AN013     27
#endif

/** Analog input, internal */
#ifndef GRROSE
#define PIN_ANINT     28
#define PIN_ANTMP     29
#else
#define PIN_ANINT     35
#define PIN_ANTMP     36
#endif

#define PIN_TX3     31
#define PIN_RX3     32
#define PIN_SDA2    33
#define PIN_SCL2    34

#ifdef __RX600__
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))
#endif/*__RX600__*/

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
#ifdef GRSAKURA
	_offset(PORT2), /* PIN_IO0 (P21) */
	_offset(PORT2), /* PIN_IO1 (P20) */
	_offset(PORT2), /* PIN_IO2 (P22) */
	_offset(PORT2), /* PIN_IO3 (P23) */
	_offset(PORT2), /* PIN_IO4 (P24) */
	_offset(PORT2), /* PIN_IO5 (P25) */
	_offset(PORT3), /* PIN_IO6 (P32) */
	_offset(PORT3), /* PIN_IO7 (P33) */
	_offset(PORTC), /* PIN_IO8 (PC2) */
	_offset(PORTC), /* PIN_IO9 (PC3) */
	_offset(PORTC), /* PIN_IO10(PC4) */
	_offset(PORTC), /* PIN_IO11(PC6) */
	_offset(PORTC), /* PIN_IO12(PC7) */
	_offset(PORTC), /* PIN_IO13(PC5) */
	_offset(PORT4), /* PIN_IO14(P40) */
	_offset(PORT4), /* PIN_IO15(P41) */
	_offset(PORT4), /* PIN_IO16(P42) */
	_offset(PORT4), /* PIN_IO17(P43) */
	_offset(PORT4), /* PIN_IO18(P44) */
	_offset(PORT4), /* PIN_IO19(P45) */
	_offset(PORT4), /* PIN_IO20(P46) */
	_offset(PORT4), /* PIN_IO21(P47) */
	_offset(PORTC), /* PIN_IO22(PC0) */
	_offset(PORTC), /* PIN_IO23(PC1) */
	_offset(PORT5), /* PIN_IO24(P50) */
	_offset(PORT5), /* PIN_IO25(P51) */
	_offset(PORT5), /* PIN_IO26(P52) */
	_offset(PORT5), /* PIN_IO27(P53) */
	_offset(PORT5), /* PIN_IO28(P54) */
	_offset(PORT5), /* PIN_IO29(P55) */
	_offset(PORT1), /* PIN_IO30(P12) */
	_offset(PORT1), /* PIN_IO31(P13) */
	_offset(PORT1), /* PIN_IO32(P14) */
	_offset(PORT1), /* PIN_IO33(P15) */
	_offset(PORT1), /* PIN_IO34(P16) */
	_offset(PORT1), /* PIN_IO35(P17) */
	_offset(PORTD), /* PIN_IO36(PD0) */
	_offset(PORTD), /* PIN_IO37(PD1) */
	_offset(PORTD), /* PIN_IO38(PD2) */
	_offset(PORTD), /* PIN_IO39(PD3) */
	_offset(PORTD), /* PIN_IO40(PD4) */
	_offset(PORTD), /* PIN_IO41(PD5) */
	_offset(PORTD), /* PIN_IO42(PD6) */
	_offset(PORTD), /* PIN_IO43(PD7) */
	_offset(PORTE), /* PIN_IO44(PE0) */
	_offset(PORTE), /* PIN_IO45(PE1) */
	_offset(PORTE), /* PIN_IO46(PE2) */
	_offset(PORTE), /* PIN_IO47(PE3) */
	_offset(PORTE), /* PIN_IO48(PE4) */
	_offset(PORTE), /* PIN_IO49(PE5) */
	_offset(PORTE), /* PIN_IO50(PE6) */
	_offset(PORTE), /* PIN_IO51(PE7) */
	_offset(PORT0), /* PIN_IO52(P07) */
	_offset(PORT0), /* PIN_IO53(P05) */
	_offset(PORT3), /* PIN_IO54(P35) */
	_offset(PORTJ), /* PIN_IO55(PJ3) */
	_offset(PORT2), /* PIN_IO56(P27) */
	_offset(PORT3), /* PIN_IO57(P34) */
	_offset(PORT2), /* PIN_IO58(P26) */
	_offset(PORT3), /* PIN_IO59(P31) */
	_offset(PORT3), /* PIN_IO60(P30) */
	_offset(PORTA), /* PIN_LED0(PA0) */
	_offset(PORTA), /* PIN_LED1(PA1) */
	_offset(PORTA), /* PIN_LED2(PA2) */
	_offset(PORTA), /* PIN_LED3(PA6) */
	_offset(PORTA), /* PIN_SW  (PA7) */
#elif defined(GRCITRUS)
    _offset(PORT2), /* PIN_IO0 (P20) */
    _offset(PORT2), /* PIN_IO1 (P21) */
    _offset(PORTC), /* PIN_IO2 (PC0) */
    _offset(PORTC), /* PIN_IO3 (PC1) */
    _offset(PORTC), /* PIN_IO4 (PC2) */
    _offset(PORT5), /* PIN_IO5 (P50) */
    _offset(PORT5), /* PIN_IO6 (P52) */
    _offset(PORT3), /* PIN_IO7 (P32) */
    _offset(PORT3), /* PIN_IO8 (P33) */
    _offset(PORT0), /* PIN_IO9 (P05) */
    _offset(PORTC), /* PIN_IO10(PC4) */
    _offset(PORTC), /* PIN_IO11(PC6) */
    _offset(PORTC), /* PIN_IO12(PC7) */
    _offset(PORTC), /* PIN_IO13(PC5) */
    _offset(PORT4), /* PIN_IO14(P40) */
    _offset(PORT4), /* PIN_IO15(P41) */
    _offset(PORT4), /* PIN_IO16(P42) */
    _offset(PORT4), /* PIN_IO17(P43) */
    _offset(PORT1), /* PIN_IO18(P12) */
    _offset(PORT1), /* PIN_IO19(P13) */
    _offset(PORT1), /* PIN_IO20(P15) */
    _offset(PORT3), /* PIN_IO21(P31) */
    _offset(PORT3), /* PIN_IO22(P30) */
    _offset(PORT2), /* PIN_IO23(P25) */
    _offset(PORT3), /* PIN_IO24(P34) */
    _offset(PORT5), /* PIN_IO25(P55) */
    _offset(PORT2), /* PIN_IO26(P26) */
    _offset(PORT2), /* PIN_IO27(P27) */
    _offset(PORTB), /* PIN_IO28(PB3) */
    _offset(PORTB), /* PIN_IO29(PB5) */
    _offset(PORTE), /* PIN_IO30(PE1) */
    _offset(PORT3), /* PIN_IO31(P35) */
    _offset(PORT2), /* PIN_IO32(P24) */
    _offset(PORTA), /* PIN_IO33(PA0) */
    _offset(PORT4), /* PIN_IO34(P44) */
    _offset(PORT4), /* PIN_IO35(P45) */
    _offset(PORT4), /* PIN_IO36(P46) */
    _offset(PORT4), /* PIN_IO37(P47) */
    _offset(PORT5), /* PIN_IO38(P51) */
    _offset(PORT5), /* PIN_IO39(P53) */
    _offset(PORT5), /* PIN_IO40(P54) */
    _offset(PORT1), /* PIN_IO41(P14) */
    _offset(PORT1), /* PIN_IO42(P16) */
    _offset(PORT1), /* PIN_IO43(P17) */
    _offset(PORTD), /* PIN_IO44(PD0) */
    _offset(PORTD), /* PIN_IO45(PD1) */
    _offset(PORTD), /* PIN_IO46(PD2) */
    _offset(PORTD), /* PIN_IO47(PD3) */
    _offset(PORTD), /* PIN_IO48(PD4) */
    _offset(PORTD), /* PIN_IO49(PD5) */
    _offset(PORTD), /* PIN_IO50(PD6) */
    _offset(PORTD), /* PIN_IO51(PD7) */
    _offset(PORTE), /* PIN_IO52(PE0) */
    _offset(PORTE), /* PIN_IO53(PE2) */
    _offset(PORTE), /* PIN_IO54(PE3) */
    _offset(PORTE), /* PIN_IO55(PE4) */
    _offset(PORTE), /* PIN_IO56(PE5) */
    _offset(PORTE), /* PIN_IO57(PE6) */
    _offset(PORTE), /* PIN_IO58(PE7) */
    _offset(PORT0), /* PIN_IO59(P07) */
    _offset(PORTJ), /* PIN_IO60(PJ3) */
    _offset(PORTA), /* PIN_LED0(PA0) */
    _offset(PORTA), /* PIN_LED1(PA1) */
    _offset(PORTA), /* PIN_LED2(PA2) */
    _offset(PORTA), /* PIN_LED3(PA6) */
    _offset(PORTA), /* PIN_SW  (PA7) */
#elif defined(GRROSE)
    _offset(PORT2),//0
    _offset(PORT2),//1
    _offset(PORT1),//2
    _offset(PORT1),//3
    _offset(PORTC),//4
    _offset(PORTC),//5
    _offset(PORT3),//6
    _offset(PORT3),//7
    _offset(PORT3),//8
    _offset(PORT2),//9
    _offset(PORTE),//10
    _offset(PORTE),//11
    _offset(PORTE),//12
    _offset(PORTE),//13
    _offset(PORT4),//14
    _offset(PORT4),//15
    _offset(PORT4),//16
    _offset(PORT4),//17
    _offset(PORT4),//18
    _offset(PORT0),//19
    _offset(PORTE),//20
    _offset(PORTE),//21
    _offset(PORT2),//22
    _offset(PORT1),//23
    _offset(PORT5),//24
    _offset(PORT3),//25
    _offset(PORT1),//26
    _offset(PORT2),//27
    _offset(PORTA),//28
    _offset(PORTA),//29
    _offset(PORTC),//30
    _offset(PORT2),//31
    _offset(PORT2),//32
    _offset(PORT5),//33
    _offset(PORT5),//34
#endif
};
#undef _offset

const uint8_t PROGMEM digital_pin_to_bit_PGM[] = {
#ifdef GRSAKURA
	1, /* PIN_IO0 (P21) */
	0, /* PIN_IO1 (P20) */
	2, /* PIN_IO2 (P22) */
	3, /* PIN_IO3 (P23) */
	4, /* PIN_IO4 (P24) */
	5, /* PIN_IO5 (P25) */
	2, /* PIN_IO6 (P32) */
	3, /* PIN_IO7 (P33) */
	2, /* PIN_IO8 (PC2) */
	3, /* PIN_IO9 (PC3) */
	4, /* PIN_IO10(PC4) */
	6, /* PIN_IO11(PC6) */
	7, /* PIN_IO12(PC7) */
	5, /* PIN_IO13(PC5) */
	0, /* PIN_IO14(P40) */
	1, /* PIN_IO15(P41) */
	2, /* PIN_IO16(P42) */
	3, /* PIN_IO17(P43) */
	4, /* PIN_IO18(P44) */
	5, /* PIN_IO19(P45) */
	6, /* PIN_IO20(P46) */
	7, /* PIN_IO21(P47) */
	0, /* PIN_IO22(PC0) */
	1, /* PIN_IO23(PC1) */
	0, /* PIN_IO24(P50) */
	1, /* PIN_IO25(P51) */
	2, /* PIN_IO26(P52) */
	3, /* PIN_IO27(P53) */
	4, /* PIN_IO28(P54) */
	5, /* PIN_IO29(P55) */
	2, /* PIN_IO30(P12) */
	3, /* PIN_IO31(P13) */
	4, /* PIN_IO32(P14) */
	5, /* PIN_IO33(P15) */
	6, /* PIN_IO34(P16) */
	7, /* PIN_IO35(P17) */
	0, /* PIN_IO36(PD0) */
	1, /* PIN_IO37(PD1) */
	2, /* PIN_IO38(PD2) */
	3, /* PIN_IO39(PD3) */
	4, /* PIN_IO40(PD4) */
	5, /* PIN_IO41(PD5) */
	6, /* PIN_IO42(PD6) */
	7, /* PIN_IO43(PD7) */
	0, /* PIN_IO44(PE0) */
	1, /* PIN_IO45(PE1) */
	2, /* PIN_IO46(PE2) */
	3, /* PIN_IO47(PE3) */
	4, /* PIN_IO48(PE4) */
	5, /* PIN_IO49(PE5) */
	6, /* PIN_IO50(PE6) */
	7, /* PIN_IO51(PE7) */
	7, /* PIN_IO52(P07) */
	5, /* PIN_IO53(P05) */
	5, /* PIN_IO54(P35) */
	3, /* PIN_IO55(PJ3) */
	7, /* PIN_IO56(P27) */
	4, /* PIN_IO57(P34) */
	6, /* PIN_IO58(P26) */
	1, /* PIN_IO59(P31) */
	0, /* PIN_IO60(P30) */
	0, /* PIN_LED0(PA0) */
	1, /* PIN_LED1(PA1) */
	2, /* PIN_LED2(PA2) */
	6, /* PIN_LED3(PA6) */
	7, /* PIN_SW  (PA7) */
#elif defined(GRCITRUS)
    0, /* PIN_IO0 (P20) */
    1, /* PIN_IO1 (P21) */
    0, /* PIN_IO2 (PC0) */
    1, /* PIN_IO3 (PC1) */
    2, /* PIN_IO4 (PC2) */
    0, /* PIN_IO5 (P50) */
    2, /* PIN_IO6 (P52) */
    2, /* PIN_IO7 (P32) */
    3, /* PIN_IO8 (P33) */
    5, /* PIN_IO9 (P05) */
    4, /* PIN_IO10(PC4) */
    6, /* PIN_IO11(PC6) */
    7, /* PIN_IO12(PC7) */
    5, /* PIN_IO13(PC5) */
    0, /* PIN_IO14(P40) */
    1, /* PIN_IO15(P41) */
    2, /* PIN_IO16(P42) */
    3, /* PIN_IO17(P43) */
    2, /* PIN_IO18(P12) */
    3, /* PIN_IO19(P13) */
    5, /* PIN_IO20(P15) */
    1, /* PIN_IO21(P31) */
    0, /* PIN_IO22(P30) */
    5, /* PIN_IO23(P25) */
    4, /* PIN_IO24(P34) */
    5, /* PIN_IO25(P55) */
    6, /* PIN_IO26(P26) */
    7, /* PIN_IO27(P27) */
    3, /* PIN_IO28(PB3) */
    5, /* PIN_IO29(PB5) */
    1, /* PIN_IO30(PE1) */
    5, /* PIN_IO31(P35) */
    4, /* PIN_IO32(P24) */
    0, /* PIN_IO33(PA0) */
    4, /* PIN_IO34(P44) */
    5, /* PIN_IO35(P45) */
    6, /* PIN_IO36(P46) */
    7, /* PIN_IO37(P47) */
    1, /* PIN_IO38(P51) */
    3, /* PIN_IO39(P53) */
    4, /* PIN_IO40(P54) */
    4, /* PIN_IO41(P14) */
    6, /* PIN_IO42(P16) */
    7, /* PIN_IO43(P17) */
    0, /* PIN_IO44(PD0) */
    1, /* PIN_IO45(PD1) */
    2, /* PIN_IO46(PD2) */
    3, /* PIN_IO47(PD3) */
    4, /* PIN_IO48(PD4) */
    5, /* PIN_IO49(PD5) */
    6, /* PIN_IO50(PD6) */
    7, /* PIN_IO51(PD7) */
    0, /* PIN_IO52(PE0) */
    2, /* PIN_IO53(PE2) */
    3, /* PIN_IO54(PE3) */
    4, /* PIN_IO55(PE4) */
    5, /* PIN_IO56(PE5) */
    6, /* PIN_IO57(PE6) */
    7, /* PIN_IO58(PE7) */
    7, /* PIN_IO59(P07) */
    3, /* PIN_IO60(PJ3) */
    0, /* PIN_LED0(PA0) */
    1, /* PIN_LED1(PA1) */
    2, /* PIN_LED2(PA2) */
    6, /* PIN_LED3(PA6) */
    7, /* PIN_SW  (PA7) */
#elif defined(GRROSE)
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
    0,//14
    1,//15
    2,//16
    3,//17
    4,//18
    5,//19
    2,//20
    1,//21
    2,//22
    4,//23
    1,//24
    4,//25
    5,//26
    4,//27
    0,//28
    1,//29
    4,//30
    3,//31
    5,//32
    0,//33
    2,//34

#endif
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
#ifdef GRSAKURA
	_BV(1), /* PIN_IO0 (P21) */
	_BV(0), /* PIN_IO1 (P20) */
	_BV(2), /* PIN_IO2 (P22) */
	_BV(3), /* PIN_IO3 (P23) */
	_BV(4), /* PIN_IO4 (P24) */
	_BV(5), /* PIN_IO5 (P25) */
	_BV(2), /* PIN_IO6 (P32) */
	_BV(3), /* PIN_IO7 (P33) */
	_BV(2), /* PIN_IO8 (PC2) */
	_BV(3), /* PIN_IO9 (PC3) */
	_BV(4), /* PIN_IO10(PC4) */
	_BV(6), /* PIN_IO11(PC6) */
	_BV(7), /* PIN_IO12(PC7) */
	_BV(5), /* PIN_IO13(PC5) */
	_BV(0), /* PIN_IO14(P40) */
	_BV(1), /* PIN_IO15(P41) */
	_BV(2), /* PIN_IO16(P42) */
	_BV(3), /* PIN_IO17(P43) */
	_BV(4), /* PIN_IO18(P44) */
	_BV(5), /* PIN_IO19(P45) */
	_BV(6), /* PIN_IO20(P46) */
	_BV(7), /* PIN_IO21(P47) */
	_BV(0), /* PIN_IO22(PC0) */
	_BV(1), /* PIN_IO23(PC1) */
	_BV(0), /* PIN_IO24(P50) */
	_BV(1), /* PIN_IO25(P51) */
	_BV(2), /* PIN_IO26(P52) */
	_BV(3), /* PIN_IO27(P53) */
	_BV(4), /* PIN_IO28(P54) */
	_BV(5), /* PIN_IO29(P55) */
	_BV(2), /* PIN_IO30(P12) */
	_BV(3), /* PIN_IO31(P13) */
	_BV(4), /* PIN_IO32(P14) */
	_BV(5), /* PIN_IO33(P15) */
	_BV(6), /* PIN_IO34(P16) */
	_BV(7), /* PIN_IO35(P17) */
	_BV(0), /* PIN_IO36(PD0) */
	_BV(1), /* PIN_IO37(PD1) */
	_BV(2), /* PIN_IO38(PD2) */
	_BV(3), /* PIN_IO39(PD3) */
	_BV(4), /* PIN_IO40(PD4) */
	_BV(5), /* PIN_IO41(PD5) */
	_BV(6), /* PIN_IO42(PD6) */
	_BV(7), /* PIN_IO43(PD7) */
	_BV(0), /* PIN_IO44(PE0) */
	_BV(1), /* PIN_IO45(PE1) */
	_BV(2), /* PIN_IO46(PE2) */
	_BV(3), /* PIN_IO47(PE3) */
	_BV(4), /* PIN_IO48(PE4) */
	_BV(5), /* PIN_IO49(PE5) */
	_BV(6), /* PIN_IO50(PE6) */
	_BV(7), /* PIN_IO51(PE7) */
	_BV(7), /* PIN_IO52(P07) */
	_BV(5), /* PIN_IO53(P05) */
	_BV(5), /* PIN_IO54(P35) */
	_BV(3), /* PIN_IO55(PJ3) */
	_BV(7), /* PIN_IO56(P27) */
	_BV(4), /* PIN_IO57(P34) */
	_BV(6), /* PIN_IO58(P26) */
	_BV(1), /* PIN_IO59(P31) */
	_BV(0), /* PIN_IO60(P30) */
	_BV(0), /* PIN_LED0(PA0) */
	_BV(1), /* PIN_LED1(PA1) */
	_BV(2), /* PIN_LED2(PA2) */
	_BV(6), /* PIN_LED3(PA6) */
	_BV(7), /* PIN_SW  (PA7) */
#elif defined(GRCITRUS)
    _BV(0), /* PIN_IO0 (P20) */
    _BV(1), /* PIN_IO1 (P21) */
    _BV(0), /* PIN_IO2 (PC0) */
    _BV(1), /* PIN_IO3 (PC1) */
    _BV(2), /* PIN_IO4 (PC2) */
    _BV(0), /* PIN_IO5 (P50) */
    _BV(2), /* PIN_IO6 (P52) */
    _BV(2), /* PIN_IO7 (P32) */
    _BV(3), /* PIN_IO8 (P33) */
    _BV(5), /* PIN_IO9 (P05) */
    _BV(4), /* PIN_IO10(PC4) */
    _BV(6), /* PIN_IO11(PC6) */
    _BV(7), /* PIN_IO12(PC7) */
    _BV(5), /* PIN_IO13(PC5) */
    _BV(0), /* PIN_IO14(P40) */
    _BV(1), /* PIN_IO15(P41) */
    _BV(2), /* PIN_IO16(P42) */
    _BV(3), /* PIN_IO17(P43) */
    _BV(2), /* PIN_IO18(P12) */
    _BV(3), /* PIN_IO19(P13) */
    _BV(5), /* PIN_IO20(P15) */
    _BV(1), /* PIN_IO21(P31) */
    _BV(0), /* PIN_IO22(P30) */
    _BV(5), /* PIN_IO23(P25) */
    _BV(4), /* PIN_IO24(P34) */
    _BV(5), /* PIN_IO25(P55) */
    _BV(6), /* PIN_IO26(P26) */
    _BV(7), /* PIN_IO27(P27) */
    _BV(3), /* PIN_IO28(PB3) */
    _BV(5), /* PIN_IO29(PB5) */
    _BV(1), /* PIN_IO30(PE1) */
    _BV(5), /* PIN_IO31(P35) */
    _BV(4), /* PIN_IO32(P24) */
    _BV(0), /* PIN_IO33(PA0) */
    _BV(4), /* PIN_IO34(P44) */
    _BV(5), /* PIN_IO35(P45) */
    _BV(6), /* PIN_IO36(P46) */
    _BV(7), /* PIN_IO37(P47) */
    _BV(1), /* PIN_IO38(P51) */
    _BV(3), /* PIN_IO39(P53) */
    _BV(4), /* PIN_IO40(P54) */
    _BV(4), /* PIN_IO41(P14) */
    _BV(6), /* PIN_IO42(P16) */
    _BV(7), /* PIN_IO43(P17) */
    _BV(0), /* PIN_IO44(PD0) */
    _BV(1), /* PIN_IO45(PD1) */
    _BV(2), /* PIN_IO46(PD2) */
    _BV(3), /* PIN_IO47(PD3) */
    _BV(4), /* PIN_IO48(PD4) */
    _BV(5), /* PIN_IO49(PD5) */
    _BV(6), /* PIN_IO50(PD6) */
    _BV(7), /* PIN_IO51(PD7) */
    _BV(0), /* PIN_IO52(PE0) */
    _BV(2), /* PIN_IO53(PE2) */
    _BV(3), /* PIN_IO54(PE3) */
    _BV(4), /* PIN_IO55(PE4) */
    _BV(5), /* PIN_IO56(PE5) */
    _BV(6), /* PIN_IO57(PE6) */
    _BV(7), /* PIN_IO58(PE7) */
    _BV(7), /* PIN_IO59(P07) */
    _BV(3), /* PIN_IO60(PJ3) */
    _BV(0), /* PIN_LED0(PA0) */
    _BV(1), /* PIN_LED1(PA1) */
    _BV(2), /* PIN_LED2(PA2) */
    _BV(6), /* PIN_LED3(PA6) */
    _BV(7), /* PIN_SW  (PA7) */
#elif defined(GRROSE)
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
    _BV(0),
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(2),
    _BV(1),
    _BV(2),
    _BV(4),
    _BV(1),
    _BV(4),
    _BV(5),
    _BV(4),
    _BV(0),
    _BV(1),
    _BV(4),
    _BV(3),
    _BV(5),
    _BV(0),
    _BV(2),

#endif
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
