/***************************************************************************
PURPOSE
     RX63N Library for Arduino compatible framework

TARGET DEVICE
     RX63N

AUTHOR
     Renesas Solutions Corp.
     AND Technology Research Ltd.

***************************************************************************
Copyright (C) 2014 Renesas Electronics. All rights reserved.

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2.1 of the License, or (at your option) any
later version.

See file LICENSE.txt for further informations on licensing terms.

***************************************************************************
 Copyright   : (c) AND Technology Research Ltd, 2013
 Address     : 4 Forest Drive, Theydon Bois, Essex, CM16 7EY
 Tel         : +44 (0) 1992 81 4655
 Fax         : +44 (0) 1992 81 3362
 Email       : ed.king@andtr.com
 Website     : www.andtr.com

 Project     : Arduino
 Module      : Core
 File        : Utilities.cpp
 Author      : E King
 Start Date  : 03/09/13
 Description : Implementation of the common utility functions.

 ******************************************************************************/
/*
 *  Modified 18 Apr 2019 by Yuuki Okamiya, using FreeRTOS timers for GR-ROSE.
 *  Modified 10 Jul 2018 by Yuuki Okamiya, for GR-ROSE.
 *  Modified 9 May 2014 by Yuuki Okamiya, for remove warnings, fixed timer frequency.
 *  Modified 12 May 2014 by Yuuki Okamiya, modify for analogWriteFrequency
 *  Modified 15 May 2014 by Yuuki Okamiya, change timer channel for each PWM pin.
 *  Modified 18 Jun 2014 by Nozomu Fujita : コメント修正
 *  Modified 19 Jun 2014 by Nozomu Fujita : 周期起動ハンドラ不具合修正
 *  Modified 19 Jun 2014 by Nozomu Fujita : INT_Excep_TPU2_TGI2A() 不具合修正
 *  Modified 19 Jun 2014 by Nozomu Fujita : timer_regist_userfunc() を追加
 */
#include <stddef.h>

#include "Arduino.h"
#include "Time.h"

#include "rx65n/interrupt_handlers.h"
#include "iodefine.h"
#include "util.h"
extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
}

// DEFINITIONS ****************************************************************/
/** The counter period required to give ~490Hz PWM frequency for 12MHz PCLK. */
#define COUNTER_PERIOD_FOR_490HZ_PWM    24490U
/** The maximum permitted frequency of a PWM signal, assuming a 12MHz PCLK. */
#define MAX_PWM_FREQ                    65535U
/** The counter value per duty cycle step for PWM of 490Hz from 12MHz PCLK. */
#define COUNTER_PER_DUTY_CYCLE_STEP     96U
/** The maximum frequency of a analogWrite signal, assuming a 12MHz PCLK. */
#define MAX_ANALOGWRITE_FREQ                    46875U
/** The minimum frequency of a analogWrite signal, assuming a 12MHz PCLK. */
#define MIN_ANALOGWRITE_FREQ                    184U

#define MAX_CYCLIC_HANDLER      (8)         //!< Number of maximum cyclic handler

// MACROS *********************************************************************/

#define COUNTER_INTERVAL

// DECLARATIONS ***************************************************************/
/** The current conversion reference. */
static word g_frequency_analogwrite = 490;
static fITInterruptFunc_t   g_fITInterruptFunc = NULL;  //!< ユーザー定義インターバルタイマハンドラ
static TimerHandle_t xTimers;

/** This module's name. */
//static const char *MODULE = "UTILITIES"; //comment out to remove warning

// IMPLEMENTATIONS ************************************************************/

bool set_frequency_for_analogwrite(word frequency)
{
    if (frequency < MIN_ANALOGWRITE_FREQ || frequency > MAX_ANALOGWRITE_FREQ){
        return false;
    }
    g_frequency_analogwrite = frequency;
    return true;
}


/**
 * Generate a BRK instruction. This differs from the usual built-in variant
 * because we need to manually reset the user stack pointer. This is because
 * we do not transition to user mode when the program starts.
 */
void generate_brk()
{
    asm("brk");
    asm("mvtc #30000h, psw");
}

/****************************************************************************
 * Attach interval timer function
 *
 * The callback function is called every 1ms interval
 * @param[in] fFunction Specify callback function
 *
 * @return none
 *
 ***************************************************************************/

static void vTimerCallback( TimerHandle_t xTimer ){
    if (g_fITInterruptFunc != NULL) {
        (*g_fITInterruptFunc)(millis());
    }
}

void attachIntervalTimerHandler(void (*fFunction)(unsigned long u32Milles))
{
	g_fITInterruptFunc = fFunction;
	xTimers = xTimerCreate("Timer", 1, pdTRUE, ( void * ) 0, vTimerCallback);
	xTimerStart(xTimers, 0);
}


/****************************************************************************
 * Detach interval timer function
 *
 * @param[in] none
 *
 * @return none
 *
 ***************************************************************************/
void detachIntervalTimerHandler()
{
    g_fITInterruptFunc = NULL;
	xTimerStop(xTimers, 0);
}


/****************************************************************************
 * Attach interval timer function
 *
 * The callback function is called every 1ms interval
 * @param[in] fFunction Specify callback function
 *
 * @return always 1
 *
 ***************************************************************************/
unsigned long timer_regist_userfunc(void (*fFunction)(void))
{
    if (fFunction != NULL) {
        attachIntervalTimerHandler((void (*)(unsigned long))fFunction);
    } else {
        detachIntervalTimerHandler();
    }
    return 1;
}
