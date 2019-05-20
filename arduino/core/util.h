/*
  util.h - 
  Copyright (c) 2014 Nozomu Fujita.  All right reserved.

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
*/

#ifndef _UTIL_H
#define _UTIL_H

#include "Types.h"
#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MstpIdINVALID = -1,
    MstpIdEXDMAC,
    MstpIdEXDMAC0,
    MstpIdEXDMAC1,
    MstpIdDMAC,
    MstpIdDMAC0,
    MstpIdDMAC1,
    MstpIdDMAC2,
    MstpIdDMAC3,
    MstpIdDTC,
    MstpIdA27,
    MstpIdA24,
    MstpIdDA,
    MstpIdS12AD0,
    MstpIdS12AD1,
    MstpIdCMT0,
    MstpIdCMT1,
    MstpIdCMT2,
    MstpIdCMT3,
    MstpIdTPU0,
    MstpIdTPU1,
    MstpIdTPU2,
    MstpIdTPU3,
    MstpIdTPU4,
    MstpIdTPU5,
    MstpIdPPG0,
    MstpIdPPG1,
    MstpIdMTU,
    MstpIdMTU0,
    MstpIdMTU1,
    MstpIdMTU2,
    MstpIdMTU3,
    MstpIdMTU4,
    MstpIdMTU5,
    MstpIdTMR0,
    MstpIdTMR1,
    MstpIdTMR01,
    MstpIdTMR2,
    MstpIdTMR3,
    MstpIdTMR23,
    MstpIdSCI0,
    MstpIdSMCI0,
    MstpIdSCI1,
    MstpIdSMCI1,
    MstpIdSCI2,
    MstpIdSMCI2,
    MstpIdSCI3,
    MstpIdSMCI3,
    MstpIdSCI4,
    MstpIdSMCI4,
    MstpIdSCI5,
    MstpIdSMCI5,
    MstpIdSCI6,
    MstpIdSMCI6,
    MstpIdSCI7,
    MstpIdSMCI7,
    MstpIdCRC,
    MstpIdPDC,
    MstpIdRIIC0,
    MstpIdRIIC1,
    MstpIdUSB0,
    MstpIdRSPI0,
    MstpIdRSPI1,
    MstpIdEDMAC0,
    MstpIdEDMAC1,
    MstpIdTEMPS,
    MstpIdSCI12,
    MstpIdSMCI12,
    MstpIdCAN2,
    MstpIdCAN1,
    MstpIdCAN0,
    MstpIdSCI8,
    MstpIdSMCI8,
    MstpIdSCI9,
    MstpIdSMCI9,
    MstpIdSCI10,
    MstpIdSMCI10,
    MstpIdSCI11,
    MstpIdSMCI11,
    MstpIdRSPI2,
    MstpIdMCK,
    MstpIdIEB,
    MstpIdRIIC2,
    MstpIdRAM1,
    MstpIdRAM0,
    MstpIdDEU,
    NumOfMstpId,
} MstpId;

void startModule(MstpId module);
void stopModule(MstpId module);

void assignPinFunction(int pin, int psel, int isel, int asel);
void assignPinPSEL(int pin, int psel);
void assignPinISEL(int pin, int isel);
void assignPinASEL(int pin, int asel);

bool set_pwm_for_pin(int pin, uint16_t frequency, byte value, bool for_analog_write);

typedef enum {
    PinModeUnused = 0,
    PinModeInput,
    PinModeOutput,
    PinModeInputPullUp,
    PinModeOutputHigh,
    PinModeOutputOpenDrain,
    PinModeAnalogRead,
    PinModeAnalogWrite,
    PinModeTone,
    PinModeServo,
    PinModeDac,
	PinModePpg,
    PinModeOther,
    PinModeError,
} PinMode;

void setPinMode(int pin, PinMode mode);
PinMode getPinMode(int pin);
void changePinMode(int pin, PinMode mode);
void resetPinMode(int pin);
void setPinModeUnused(int pin) __attribute__ ((weak));
void setPinModeInput(int pin) __attribute__ ((weak));
void resetPinModeInput(int pin) __attribute__ ((weak));
void setPinModeOutput(int pin) __attribute__ ((weak));
void resetPinModeOutput(int pin) __attribute__ ((weak));
void setPinModeInputPullUp(int pin) __attribute__ ((weak));
void resetPinModeInputPullUp(int pin) __attribute__ ((weak));
void setPinModeOutputHigh(int pin) __attribute__ ((weak));
void resetPinModeOutputHigh(int pin) __attribute__ ((weak));
void setPinModeOutputOpenDrain(int pin) __attribute__ ((weak));
void resetPinModeOutputOpenDrain(int pin) __attribute__ ((weak));
void setPinModeAnalogRead(int pin) __attribute__ ((weak));
void resetPinModeAnalogRead(int pin) __attribute__ ((weak));
void setPinModeAnalogWrite(int pin) __attribute__ ((weak));
void resetPinModeAnalogWrite(int pin) __attribute__ ((weak));
void setPinModeTone(int pin) __attribute__ ((weak));
void resetPinModeTone(int pin) __attribute__ ((weak));
void setPinModeServo(int pin) __attribute__ ((weak));
void resetPinModeServo(int pin) __attribute__ ((weak));
void setPinModeDac(int pin) __attribute__ ((weak));
void resetPinModeDac(int pin) __attribute__ ((weak));
void setPinModePpg(int pin) __attribute__ ((weak));
void resetPinModePpg(int pin) __attribute__ ((weak));
void setPinModeOther(int pin) __attribute__ ((weak));
void resetPinModeOther(int pin) __attribute__ ((weak));
void setPinModeHardwarePWM(int pin, int period, int term, unsigned long length);
void changePinModeHardwarePWM(int pin, int period, int term, unsigned long length);
void resetPinModeHardwarePWM(int pin);
void setPinModeSoftwarePWM(int pin, int period, int term, unsigned long length);
void changePinModeSoftwarePWM(int pin, int period, int term, unsigned long length);
void resetPinModeSoftwarePWM(int pin);

#define MaxSoftwarePwmChannels 15
#define HardwarePWMFreq (PCLK / 16)
#define SoftwarePWMFreq (125 * 1000)

#define isHardwarePWMPin(pin) \
( \
	    ((pin) == 3) \
	    || ((pin) == 7) \
	    || ((pin) == 9) \
	    || ((pin) == 10) \
	    || ((pin) == 13) \
)
#define isDACPin(pin) \
( \
       ((pin) == 24) \
)

void setAnalogWriteClock(int pin, uint32_t clock);
float getPWMClockMultiple(int pin);

#ifdef __cplusplus
};
#endif

#endif/*_UTIL_H*/
