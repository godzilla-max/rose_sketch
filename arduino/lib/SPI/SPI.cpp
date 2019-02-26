/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
/* 25th Feb. 2019 : Modified for GR-ROSE by Yuuki Okamiya. */

#include "SPI.h"

SPIClass SPI;

uint8_t SPIClass::initialized = 0;
uint8_t SPIClass::interruptMode = 0;
uint8_t SPIClass::interruptMask = 0;
uint8_t SPIClass::interruptSave = 0;
#ifdef SPI_TRANSACTION_MISMATCH_LED
uint8_t SPIClass::inTransactionFlag = 0;
#endif

void SPIClass::begin()
{
#ifndef __RX600__
  uint8_t sreg = SREG;
#else
  uint8_t sreg = isNoInterrupts();
#endif
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
#ifndef __RX600__
  if (!initialized) {
    // Set SS to high so a connected chip will be "deselected" by default
    uint8_t port = digitalPinToPort(SS);
    uint8_t bit = digitalPinToBitMask(SS);
    volatile uint8_t *reg = portModeRegister(port);

    // if the SS pin is not already configured as an output
    // then set it high (to enable the internal pull-up resistor)
    if(!(*reg & bit)){
      digitalWrite(SS, HIGH);
    }

    // When the SS pin is set as OUTPUT, it can be used as
    // a general purpose output port (it doesn't influence
    // SPI operations).
    pinMode(SS, OUTPUT);

    // Warning: if the SS pin ever becomes a LOW INPUT then SPI
    // automatically switches to Slave, so the data direction of
    // the SS pin MUST be kept as OUTPUT.
    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);

    // Set direction register for SCK and MOSI pin.
    // MISO pin automatically overrides to INPUT.
    // By doing this AFTER enabling SPI, we avoid accidentally
    // clocking in a single bit since the lines go directly
    // from "input" to SPI control.
    // http://code.google.com/p/arduino/issues/detail?id=888
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
  }
#else
  BSET(portModeRegister(digitalPinToPort(SCK)), digitalPinToBit(SCK));
  BSET(portModeRegister(digitalPinToPort(MISO)), digitalPinToBit(MISO));
  BSET(portModeRegister(digitalPinToPort(MOSI)), digitalPinToBit(MOSI));

  assignPinFunction(SCK, 0b01101, 0, 0);
  assignPinFunction(MISO, 0b01101, 0, 0);
  assignPinFunction(MOSI, 0b01101, 0, 0);
/* SS is controlled by GPIO */
//  assignPinFunction(SS, 0b01101, 0, 0);

  /* Wake RSPI unit from standby mode */
  startModule(MstpIdRSPI1);

  RSPI1.SPCR.BYTE = 0x08u; //SPI Stop, Set to Master Mode
  /* Set SSL pin to active low */
  RSPI1.SSLP.BIT.SSL0P = 0u;
  /* Set SPPCR register */
  RSPI1.SPPCR.BYTE = 0u;
  /* Set bit rate to 3.75Mbit/s, by setting SPBR to 0 */
  RSPI1.SPBR = SPI_CLOCK_DIV16;
  /* Set SPDCR register */
  RSPI1.SPDCR.BYTE = 0x20u;
  /* Set RSPI sequence control pointer to SPCMD0 */
  RSPI1.SPSCR.BYTE = 0u;
  /* Set SPCMD0 register (command register 0) */
  RSPI1.SPCMD0.WORD = SPI_DEFAULT_SPCMD; //MSB first, 8bit, SPI MODE0, SSL0(External), div 2 clock

  RSPI1.SPCR.BIT.SPRIE = 1; // Enable Receive Interrupt Request
  RSPI1.SPCR.BIT.SPE = 1; //Start SPI

#endif
  initialized++; // reference count
#ifndef __RX600__
  SREG = sreg;
#else
  if (!sreg) {
    interrupts();
  }
#endif
}

void SPIClass::end() {
#ifndef __RX600__
  uint8_t sreg = SREG;
#else
  uint8_t sreg = isNoInterrupts();
#endif
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  // Decrease the reference counter
  if (initialized)
    initialized--;
  // If there are no more references disable SPI
  if (!initialized) {
#ifndef __RX600__
    SPCR &= ~_BV(SPE);
#else
    RSPI1.SPCR.BIT.SPE = 0; //Stop SPI
#endif
    interruptMode = 0;
    #ifdef SPI_TRANSACTION_MISMATCH_LED
    inTransactionFlag = 0;
    #endif
  }
#ifndef __RX600__
  SREG = sreg;
#else
  if (!sreg) {
    interrupts();
  }
#endif
}

// mapping of interrupt numbers to bits within SPI_AVR_EIMSK
#if defined(__AVR_ATmega32U4__)
  #define SPI_INT0_MASK  (1<<INT0)
  #define SPI_INT1_MASK  (1<<INT1)
  #define SPI_INT2_MASK  (1<<INT2)
  #define SPI_INT3_MASK  (1<<INT3)
  #define SPI_INT4_MASK  (1<<INT6)
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
  #define SPI_INT0_MASK  (1<<INT0)
  #define SPI_INT1_MASK  (1<<INT1)
  #define SPI_INT2_MASK  (1<<INT2)
  #define SPI_INT3_MASK  (1<<INT3)
  #define SPI_INT4_MASK  (1<<INT4)
  #define SPI_INT5_MASK  (1<<INT5)
  #define SPI_INT6_MASK  (1<<INT6)
  #define SPI_INT7_MASK  (1<<INT7)
#elif defined(EICRA) && defined(EICRB) && defined(EIMSK)
  #define SPI_INT0_MASK  (1<<INT4)
  #define SPI_INT1_MASK  (1<<INT5)
  #define SPI_INT2_MASK  (1<<INT0)
  #define SPI_INT3_MASK  (1<<INT1)
  #define SPI_INT4_MASK  (1<<INT2)
  #define SPI_INT5_MASK  (1<<INT3)
  #define SPI_INT6_MASK  (1<<INT6)
  #define SPI_INT7_MASK  (1<<INT7)
#else
  #ifdef INT0
  #define SPI_INT0_MASK  (1<<INT0)
  #endif
  #ifdef INT1
  #define SPI_INT1_MASK  (1<<INT1)
  #endif
  #ifdef INT2
  #define SPI_INT2_MASK  (1<<INT2)
  #endif
#endif

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
  uint8_t mask = 0;
#ifndef __RX600__
  uint8_t sreg = SREG;
#else
  uint8_t sreg = isNoInterrupts();
#endif
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  switch (interruptNumber) {
  #ifdef SPI_INT0_MASK
  case 0: mask = SPI_INT0_MASK; break;
  #endif
  #ifdef SPI_INT1_MASK
  case 1: mask = SPI_INT1_MASK; break;
  #endif
  #ifdef SPI_INT2_MASK
  case 2: mask = SPI_INT2_MASK; break;
  #endif
  #ifdef SPI_INT3_MASK
  case 3: mask = SPI_INT3_MASK; break;
  #endif
  #ifdef SPI_INT4_MASK
  case 4: mask = SPI_INT4_MASK; break;
  #endif
  #ifdef SPI_INT5_MASK
  case 5: mask = SPI_INT5_MASK; break;
  #endif
  #ifdef SPI_INT6_MASK
  case 6: mask = SPI_INT6_MASK; break;
  #endif
  #ifdef SPI_INT7_MASK
  case 7: mask = SPI_INT7_MASK; break;
  #endif
  default:
    interruptMode = 2;
    break;
  }
  interruptMask |= mask;
  if (!interruptMode)
    interruptMode = 1;
#ifndef __RX600__
  SREG = sreg;
#else
  if (!sreg) {
    interrupts();
  }
#endif
}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
{
  // Once in mode 2 we can't go back to 0 without a proper reference count
  if (interruptMode == 2)
    return;
  uint8_t mask = 0;
#ifndef __RX600__
  uint8_t sreg = SREG;
#else
  uint8_t sreg = isNoInterrupts();
#endif
  noInterrupts(); // Protect from a scheduler and prevent transactionBegin
  switch (interruptNumber) {
  #ifdef SPI_INT0_MASK
  case 0: mask = SPI_INT0_MASK; break;
  #endif
  #ifdef SPI_INT1_MASK
  case 1: mask = SPI_INT1_MASK; break;
  #endif
  #ifdef SPI_INT2_MASK
  case 2: mask = SPI_INT2_MASK; break;
  #endif
  #ifdef SPI_INT3_MASK
  case 3: mask = SPI_INT3_MASK; break;
  #endif
  #ifdef SPI_INT4_MASK
  case 4: mask = SPI_INT4_MASK; break;
  #endif
  #ifdef SPI_INT5_MASK
  case 5: mask = SPI_INT5_MASK; break;
  #endif
  #ifdef SPI_INT6_MASK
  case 6: mask = SPI_INT6_MASK; break;
  #endif
  #ifdef SPI_INT7_MASK
  case 7: mask = SPI_INT7_MASK; break;
  #endif
  default:
    break;
    // this case can't be reached
  }
  interruptMask &= ~mask;
  if (!interruptMask)
    interruptMode = 0;
#ifndef __RX600__
  SREG = sreg;
#else
  if (!sreg) {
    interrupts();
  }
#endif
}
