/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Wiring project - http://wiring.uniandes.edu.co

  Copyright (c) 2004-05 Hernando Barragan

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
  
  Modified 24 November 2006 by David A. Mellis
  Modified 1 August 2010 by Mark Sproul
*/

#include <inttypes.h>
#ifndef __RX600__
#include <avr/io.h>
#include <avr/interrupt.h>
#endif /*__RX600__*/
#include <avr/pgmspace.h>
#include <stdio.h>

#include "wiring_private.h"
#include "util.h"

static volatile voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS];
// volatile static voidFuncPtr twiIntFunc;

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode) {
#ifndef GRROSE
  if(interruptNum < EXTERNAL_NUM_INTERRUPTS) {
#else
  if(interruptNum < NUM_DIGITAL_PINS) {
#endif
    // Configure the interrupt mode (trigger on low input, any change, rising
    // edge, or falling edge).  The mode constants were chosen to correspond
    // to the configuration bits in the hardware register, so we simply shift
    // the mode into place.

#ifdef __RX600__
    // Enable the interrupt.
    // Set the digital filters to use PCLK/1 and disable them.
    ICU.IRQFLTC0.WORD = 0U;
    ICU.IRQFLTE0.BYTE = 0U;
    ICU.IRQFLTC1.WORD = 0U;
    ICU.IRQFLTE1.BYTE = 0U;

    // Determine the mode mask.
    unsigned short mode_bitmask = 0x0;
    switch (mode)
    {
    // ------------------------------------------------------------------------/
    case LOW:
        mode_bitmask = 0x00;
        break;
    // ------------------------------------------------------------------------/
    case FALLING:
        mode_bitmask = 0x04;
        break;
    // ------------------------------------------------------------------------/
    case RISING:
        mode_bitmask = 0x08;
        break;
    // ------------------------------------------------------------------------/
    case CHANGE:
        mode_bitmask = 0x0C;
        break;
    // ------------------------------------------------------------------------/
    default:
        return;
    }
#endif //__RX600__

#ifndef __RX600__
    switch (interruptNum) {
#if defined(__AVR_ATmega32U4__)
	// I hate doing this, but the register assignment differs between the 1280/2560
	// and the 32U4.  Since avrlib defines registers PCMSK1 and PCMSK2 that aren't 
	// even present on the 32U4 this is the only way to distinguish between them.
    case 0:
	EICRA = (EICRA & ~((1<<ISC00) | (1<<ISC01))) | (mode << ISC00);
	EIMSK |= (1<<INT0);
	break;
    case 1:
	EICRA = (EICRA & ~((1<<ISC10) | (1<<ISC11))) | (mode << ISC10);
	EIMSK |= (1<<INT1);
	break;	
    case 2:
        EICRA = (EICRA & ~((1<<ISC20) | (1<<ISC21))) | (mode << ISC20);
        EIMSK |= (1<<INT2);
        break;
    case 3:
        EICRA = (EICRA & ~((1<<ISC30) | (1<<ISC31))) | (mode << ISC30);
        EIMSK |= (1<<INT3);
        break;
    case 4:
        EICRB = (EICRB & ~((1<<ISC60) | (1<<ISC61))) | (mode << ISC60);
        EIMSK |= (1<<INT6);
        break;
#elif defined(EICRA) && defined(EICRB) && defined(EIMSK)
    case 2:
      EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
      EIMSK |= (1 << INT0);
      break;
    case 3:
      EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
      EIMSK |= (1 << INT1);
      break;
    case 4:
      EICRA = (EICRA & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
      EIMSK |= (1 << INT2);
      break;
    case 5:
      EICRA = (EICRA & ~((1 << ISC30) | (1 << ISC31))) | (mode << ISC30);
      EIMSK |= (1 << INT3);
      break;
    case 0:
      EICRB = (EICRB & ~((1 << ISC40) | (1 << ISC41))) | (mode << ISC40);
      EIMSK |= (1 << INT4);
      break;
    case 1:
      EICRB = (EICRB & ~((1 << ISC50) | (1 << ISC51))) | (mode << ISC50);
      EIMSK |= (1 << INT5);
      break;
    case 6:
      EICRB = (EICRB & ~((1 << ISC60) | (1 << ISC61))) | (mode << ISC60);
      EIMSK |= (1 << INT6);
      break;
    case 7:
      EICRB = (EICRB & ~((1 << ISC70) | (1 << ISC71))) | (mode << ISC70);
      EIMSK |= (1 << INT7);
      break;
#else		
    case 0:
    #if defined(EICRA) && defined(ISC00) && defined(EIMSK)
      EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
      EIMSK |= (1 << INT0);
    #elif defined(MCUCR) && defined(ISC00) && defined(GICR)
      MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
      GICR |= (1 << INT0);
    #elif defined(MCUCR) && defined(ISC00) && defined(GIMSK)
      MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
      GIMSK |= (1 << INT0);
    #elif defined(__RX600__)
      //__RX600__


    #else
      #error attachInterrupt not finished for this CPU (case 0)
    #endif
      break;

    case 1:
    #if defined(EICRA) && defined(ISC10) && defined(ISC11) && defined(EIMSK)
      EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
      EIMSK |= (1 << INT1);
    #elif defined(MCUCR) && defined(ISC10) && defined(ISC11) && defined(GICR)
      MCUCR = (MCUCR & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
      GICR |= (1 << INT1);
    #elif defined(MCUCR) && defined(ISC10) && defined(GIMSK) && defined(GIMSK)
      MCUCR = (MCUCR & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
      GIMSK |= (1 << INT1);
    #elif defined(__RX600__)
      //__RX600__
    #else
      #warning attachInterrupt may need some more work for this cpu (case 1)
    #endif
      break;
    
    case 2:
    #if defined(EICRA) && defined(ISC20) && defined(ISC21) && defined(EIMSK)
      EICRA = (EICRA & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
      EIMSK |= (1 << INT2);
    #elif defined(MCUCR) && defined(ISC20) && defined(ISC21) && defined(GICR)
      MCUCR = (MCUCR & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
      GICR |= (1 << INT2);
    #elif defined(MCUCR) && defined(ISC20) && defined(GIMSK) && defined(GIMSK)
      MCUCR = (MCUCR & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
      GIMSK |= (1 << INT2);
    #endif
      break;
#endif
    }
  }
}

#else
    // Determine which IRQ is being requested. Each pin is mapped to a specific
    // IRQ (external pin interrupt) and some IRQs are mapped to multiple pins.
    // Some pin choices are unavailable on the Sakura.
    switch (interruptNum)
    {
    // IRQ2 -------------------------------------------------------------------/
    case 2 :
        *GET_ADR_IER(IRQ2) &= ~BIT_02;
        intFunc[0] = userFunc;
        pinMode(2, INPUT_PULLUP);
        assignPinFunction(2, 0, 1, 0);
        *GET_ADR_IRQCR(IRQ2) |= mode_bitmask;
        *GET_ADR_IR(IRQ2) &= ~BIT_00;
        *GET_ADR_IPR(IRQ2) = DEFAULT_INT_PRIORITY;
        *GET_ADR_IER(IRQ2) |= BIT_02;
        break;

    // IRQ3 -------------------------------------------------------------------/
    case 6 :
        *GET_ADR_IER(IRQ3) &= ~BIT_03;
        intFunc[1] = userFunc;
        pinMode(6, INPUT_PULLUP);
        assignPinFunction(6, 0, 1, 0);
        *GET_ADR_IRQCR(IRQ3) |= mode_bitmask;
        *GET_ADR_IR(IRQ3) &= ~BIT_00;
        *GET_ADR_IPR(IRQ3) = DEFAULT_INT_PRIORITY;
        *GET_ADR_IER(IRQ3) |= BIT_03;
        break;

    // IRQ6 -------------------------------------------------------------------/
    case 11 :
        *GET_ADR_IER(IRQ6) &= ~BIT_06;
        intFunc[2] = userFunc;
        pinMode(11, INPUT_PULLUP);
        assignPinFunction(11, 0, 1, 0);
        *GET_ADR_IRQCR(IRQ6) |= mode_bitmask;
        *GET_ADR_IR(IRQ6) &= ~BIT_00;
        *GET_ADR_IPR(IRQ6) = DEFAULT_INT_PRIORITY;
        *GET_ADR_IER(IRQ6) |= BIT_06;
        break;

    // IRQ7 -------------------------------------------------------------------/
    case 12 :
        *GET_ADR_IER(IRQ7) &= ~BIT_07;
        intFunc[3] = userFunc;
        pinMode(12, INPUT_PULLUP);
        assignPinFunction(12, 0, 1, 0);
        *GET_ADR_IRQCR(IRQ7) |= mode_bitmask;
        *GET_ADR_IR(IRQ7) &= ~BIT_00;
        *GET_ADR_IPR(IRQ7) = DEFAULT_INT_PRIORITY;
        *GET_ADR_IER(IRQ7) |= BIT_07;
        break;

    // IRQ13 ------------------------------------------------------------------/
    case 24:
        *GET_ADR_IER(IRQ13) &= ~BIT_05;
        intFunc[4] = userFunc;
        pinMode(24, INPUT_PULLUP);
        assignPinFunction(24, 0, 1, 0);
        *GET_ADR_IRQCR(IRQ13) |= mode_bitmask;
        *GET_ADR_IR(IRQ13) &= ~BIT_00;
        *GET_ADR_IPR(IRQ13) = DEFAULT_INT_PRIORITY;
        *GET_ADR_IER(IRQ13) |= BIT_05;
        break;

    // ------------------------------------------------------------------------/
    default:
        break;
    } // switch : interruptNum
#endif //__RX600__

  }
}
void detachInterrupt(uint8_t interruptNum) {
  if(interruptNum < EXTERNAL_NUM_INTERRUPTS) {
    // Disable the interrupt.  (We can't assume that interruptNum is equal
    // to the number of the EIMSK bit to clear, as this isn't true on the
    // ATmega8.  There, INT0 is 6 and INT1 is 7.)

#ifndef __RX600__
    switch (interruptNum) {
#if defined(__AVR_ATmega32U4__)
    case 0:
        EIMSK &= ~(1<<INT0);
        break;
    case 1:
        EIMSK &= ~(1<<INT1);
        break;
    case 2:
        EIMSK &= ~(1<<INT2);
        break;
    case 3:
        EIMSK &= ~(1<<INT3);
        break;
    case 4:
        EIMSK &= ~(1<<INT6);
        break;
#elif defined(EICRA) && defined(EICRB) && defined(EIMSK)
    case 2:
      EIMSK &= ~(1 << INT0);
      break;
    case 3:
      EIMSK &= ~(1 << INT1);
      break;
    case 4:
      EIMSK &= ~(1 << INT2);
      break;
    case 5:
      EIMSK &= ~(1 << INT3);
      break;
    case 0:
      EIMSK &= ~(1 << INT4);
      break;
    case 1:
      EIMSK &= ~(1 << INT5);
      break;
    case 6:
      EIMSK &= ~(1 << INT6);
      break;
    case 7:
      EIMSK &= ~(1 << INT7);
      break;
#else
    case 0:
    #if defined(EIMSK) && defined(INT0)
      EIMSK &= ~(1 << INT0);
    #elif defined(GICR) && defined(ISC00)
      GICR &= ~(1 << INT0); // atmega32
    #elif defined(GIMSK) && defined(INT0)
      GIMSK &= ~(1 << INT0);
    #elif defined(__RX600__)
      //__RX600__
    #else
      #error detachInterrupt not finished for this cpu
    #endif
      break;

    case 1:
    #if defined(EIMSK) && defined(INT1)
      EIMSK &= ~(1 << INT1);
    #elif defined(GICR) && defined(INT1)
      GICR &= ~(1 << INT1); // atmega32
    #elif defined(GIMSK) && defined(INT1)
      GIMSK &= ~(1 << INT1);
    #elif defined(__RX600__)
      //__RX600__
    #else
      #warning detachInterrupt may need some more work for this cpu (case 1)
    #endif
      break;
#endif
    }
#endif // __RX600__
    // Determine which IRQ is being disabled. Note that the NMI (IO54) cannot
    // be disabled.
    switch (interruptNum)
    {
    // IRQ2 -------------------------------------------------------------------/
    case 2 :
        *GET_ADR_IER(IRQ2) &= ~BIT_02;
        intFunc[0] = NULL;
        assignPinFunction(2, 0, 0, 0);
        *GET_ADR_IPR(IRQ2) = 0U;
        *GET_ADR_IR(IRQ2) &= ~BIT_00;
        break;

    // IRQ3 -------------------------------------------------------------------/
    case 6 :
        *GET_ADR_IER(IRQ3) &= ~BIT_03;
        intFunc[1] = NULL;
        assignPinFunction(6, 0, 0, 0);
        *GET_ADR_IPR(IRQ3) = 0U;
        *GET_ADR_IR(IRQ3) &= ~BIT_00;
        break;

    // IRQ6 -------------------------------------------------------------------/
    case 11 :
        *GET_ADR_IER(IRQ6) &= ~BIT_06;
        intFunc[2] = NULL;
        assignPinFunction(11, 0, 0, 0);
        *GET_ADR_IPR(IRQ6) = 0U;
        *GET_ADR_IR(IRQ6) &= ~BIT_00;
        break;

    // IRQ7 -------------------------------------------------------------------/
    case 12 :
        *GET_ADR_IER(IRQ7) &= ~BIT_07;
        intFunc[3] = NULL;
        assignPinFunction(12, 0, 0, 0);
        *GET_ADR_IPR(IRQ7) = 0U;
        *GET_ADR_IR(IRQ7) &= ~BIT_00;
        break;

    // IRQ13 ------------------------------------------------------------------/
    case 24 :
        *GET_ADR_IER(IRQ13) &= ~BIT_05;
        intFunc[4] = NULL;
        assignPinFunction(24, 0, 0, 0);
        *GET_ADR_IPR(IRQ13) = 0U;
        *GET_ADR_IR(IRQ13) &= ~BIT_00;
        break;

    default:
        break;
    } // switch : pin.

  }
}
/*
void attachInterruptTwi(void (*userFunc)(void) ) {
  twiIntFunc = userFunc;
}
*/

#if defined(__AVR_ATmega32U4__)
ISR(INT0_vect) {
	if(intFunc[EXTERNAL_INT_0])
		intFunc[EXTERNAL_INT_0]();
}

ISR(INT1_vect) {
	if(intFunc[EXTERNAL_INT_1])
		intFunc[EXTERNAL_INT_1]();
}

ISR(INT2_vect) {
    if(intFunc[EXTERNAL_INT_2])
		intFunc[EXTERNAL_INT_2]();
}

ISR(INT3_vect) {
    if(intFunc[EXTERNAL_INT_3])
		intFunc[EXTERNAL_INT_3]();
}

ISR(INT6_vect) {
    if(intFunc[EXTERNAL_INT_4])
		intFunc[EXTERNAL_INT_4]();
}

#elif defined(EICRA) && defined(EICRB)

ISR(INT0_vect) {
  if(intFunc[EXTERNAL_INT_2])
    intFunc[EXTERNAL_INT_2]();
}

ISR(INT1_vect) {
  if(intFunc[EXTERNAL_INT_3])
    intFunc[EXTERNAL_INT_3]();
}

ISR(INT2_vect) {
  if(intFunc[EXTERNAL_INT_4])
    intFunc[EXTERNAL_INT_4]();
}

ISR(INT3_vect) {
  if(intFunc[EXTERNAL_INT_5])
    intFunc[EXTERNAL_INT_5]();
}

ISR(INT4_vect) {
  if(intFunc[EXTERNAL_INT_0])
    intFunc[EXTERNAL_INT_0]();
}

ISR(INT5_vect) {
  if(intFunc[EXTERNAL_INT_1])
    intFunc[EXTERNAL_INT_1]();
}

ISR(INT6_vect) {
  if(intFunc[EXTERNAL_INT_6])
    intFunc[EXTERNAL_INT_6]();
}

ISR(INT7_vect) {
  if(intFunc[EXTERNAL_INT_7])
    intFunc[EXTERNAL_INT_7]();
}

#else

#ifndef __RX600__
ISR(INT0_vect) {
  if(intFunc[EXTERNAL_INT_0])
    intFunc[EXTERNAL_INT_0]();
}

ISR(INT1_vect) {
  if(intFunc[EXTERNAL_INT_1])
    intFunc[EXTERNAL_INT_1]();
}
#else

 //__RX600__
// INTERRUPT HANDLERS *********************************************************/
// NMI and IRQ interrupt handlers. Note that all of these are declared in
// interrupts_handlers.h but defined here for clarity.

void isr_irq2(void) __attribute__((interrupt(".rvectors", VECT(ICU, IRQ2)), used));
void isr_irq2(void)
{
    if (intFunc[0] != NULL)
    {
        intFunc[0]();
    }
}

void isr_irq3(void) __attribute__((interrupt(".rvectors", VECT(ICU, IRQ3)), used));
void isr_irq3(void)
{
    if (intFunc[1] != NULL)
    {
        intFunc[1]();
    }
}

void isr_irq6(void) __attribute__((interrupt(".rvectors", VECT(ICU, IRQ6)), used));
void isr_irq6(void)
{
    if (intFunc[2] != NULL)
    {
        intFunc[2]();
    }
}

void isr_irq7(void) __attribute__((interrupt(".rvectors", VECT(ICU, IRQ7)), used));
void isr_irq7(void)
{
    if (intFunc[3] != NULL)
    {
        intFunc[3]();
    }
}

void isr_irq13(void) __attribute__((interrupt(".rvectors", VECT(ICU, IRQ13)), used));
void isr_irq13(void)
{
    if (intFunc[4] != NULL)
    {
        intFunc[4]();
    }
}


#endif /*__RX600__*/

#if defined(EICRA) && defined(ISC20)
ISR(INT2_vect) {
  if(intFunc[EXTERNAL_INT_2])
    intFunc[EXTERNAL_INT_2]();
}
#endif

#endif

/*
ISR(TWI_vect) {
  if(twiIntFunc)
    twiIntFunc();
}
*/

