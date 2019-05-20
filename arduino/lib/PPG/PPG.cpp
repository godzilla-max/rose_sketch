/*
  PPG.cpp - PPG library
  Copyright (c) 2019 Renesas Electronics Corp. All right reserved.

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

#pragma	GCC optimize ("O0")

#include	"Arduino.h"
#include	"PPG.h"
extern "C" {
#include "util.h"
}

#define	calcTGR(X)	((PCLKA / (1000U * 1000U)) * (X) / cycle_division)

PPGClass::PPGClass()
{
	pin[0] = pin[1] = pin[2] = pin[3] = invalidPO;
	cycle_count = D_MTU_CYCLE;
	cycle_division = 1;
	isRunning = false;
	r_mtudtc_tgr[MTU_3][TGR_C] = r_mtubuf_tgr[MTU_3][TGR_C] =  (cycle_count - 1U)                         | D_MAGIC_NUM_FOR_MTU;
	r_mtudtc_tgr[MTU_3][TGR_D] = r_mtubuf_tgr[MTU_3][TGR_D] = ((cycle_count - 1U) - D_PRESET_MTUTGR_TIME) | D_MAGIC_NUM_FOR_MTU;
	// default setting when user might call start before setTrigger.
	setTrigger(0, cycle_count / 2, cycle_count);
	setTrigger(1, cycle_count / 2, cycle_count);
	setTrigger(2, cycle_count / 2, cycle_count);
	setTrigger(3, cycle_count / 2, cycle_count);
}

bool	PPGClass::begin(
	bool		pin0,
	bool		pin1,
	bool		pin2,
	bool		pin3,
	uint32_t	interval,
	uint8_t		polarity
)
{
	if(true == pin0){
		pin[0] = PO0;
		setPinMode(D_PIN0_PPG, PinModePpg);
	} else {
		pin[0] = invalidPO;
	}
	if(true == pin1){
		pin[1] = PO13;
		setPinMode(D_PIN1_PPG, PinModePpg);
	} else {
		pin[1] = invalidPO;
	}
	if(true == pin2){
		pin[2] = PO24;
		setPinMode(D_PIN2_PPG, PinModePpg);
	} else {
		pin[2] = invalidPO;
	}
	if(true == pin3){
		pin[3] = PO10;
		setPinMode(D_PIN3_PPG, PinModePpg);
	} else {
		pin[3] = invalidPO;
	}

	//
#ifdef	USE_PPG_MICROSECOND
	cycle_count = calcTGR(interval);
	while((((1U << 16) <= cycle_count) && ((1U << 10) >= cycle_division)) || (128U == cycle_division) || (512U == cycle_division)) {
		cycle_division *= 2;
		cycle_count = calcTGR(interval);
	}
	if((1U == cycle_division) && (1000U > cycle_count)) {
		return false;
	}
#else	// !USE_PPG_MICROSECOND
	cycle_count = interval;
#endif	// USE_PPG_MICROSECOND
	r_mtudtc_tgr[MTU_3][TGR_C] = r_mtubuf_tgr[MTU_3][TGR_C] =  (uint16_t)(cycle_count - 1U)                         | D_MAGIC_NUM_FOR_MTU;
	r_mtudtc_tgr[MTU_3][TGR_D] = r_mtubuf_tgr[MTU_3][TGR_D] = ((uint16_t)(cycle_count - 1U) - D_PRESET_MTUTGR_TIME) | D_MAGIC_NUM_FOR_MTU;
	//
	SYSTEM.PRCR.WORD	= 0xA50BU;	// Enable writing to registers related to operating modes, LPC, CGC and software reset
	MPC.PWPR.BIT.B0WI	= 0U;		// Enable writing to MPC pin function control registers
	MPC.PWPR.BIT.PFSWE	= 1U;		// Enable writing to MPC pin function control registers
	//
	R_Config_DTC_Create();
	R_Config_MTU_Create();
	R_Config_PPG_Create(pin[0], pin[1], pin[2], pin[3], polarity);
	//
	return true;
}

void	PPGClass::end()
{
	pin[0] = pin[1] = pin[2] = pin[3] = invalidPO;
}

void	PPGClass::start()
{
	uint8_t		tcsystr;

	tcsystr = 0;
	if(invalidPO != pin[0]) {
		PORT2.PMR.BIT.B0		= 1U;	// Enable PortMode as PO0
		ICU.DTCER[209].BIT.DTCE = 1U;	// Enable DTC for TGIA0
		ICU.DTCER[210].BIT.DTCE = 1U;	// Enable DTC for TGIB0
		IEN(PERIA, INTA209)		= 1U;	// Enable TGIA0 interrupt
		IEN(PERIA, INTA210)		= 1U;	// Enable TGIB0 interrupt
		tcsystr |= 0b10000000;
	}
	if(invalidPO != pin[1]) {
		PORT1.PMR.BIT.B3		= 1U;	// Enable PortMode as PO13
		ICU.DTCER[208].BIT.DTCE = 1U;	// Enable DTC for TGIA1
		ICU.DTCER[216].BIT.DTCE = 1U;	// Enable DTC for TGIB1
		IEN(PERIA, INTA208)		= 1U;	// Enable TGIA1 interrupt
		IEN(PERIA, INTA216)		= 1U;	// Enable TGIB1 interrupt
		tcsystr |= 0b01000000;
	}
	if(invalidPO != pin[2]) {
		PORTC.PMR.BIT.B3		= 1U;	// Enable PortMode as PO24
		ICU.DTCER[219].BIT.DTCE = 1U;	// Enable DTC for TGIA2
		ICU.DTCER[220].BIT.DTCE = 1U;	// Enable DTC for TGIB2
		IEN(PERIA, INTA219)		= 1U;	// Enable TGIA2 interrupt
		IEN(PERIA, INTA220)		= 1U;	// Enable TGIB2 interrupt
		tcsystr |= 0b00100000;
	}
	if(invalidPO != pin[3]) {
		PORT3.PMR.BIT.B2		= 1U;	// Enable PortMode as PO10
		ICU.DTCER[223].BIT.DTCE	= 1U;	// Enable DTC for TGIA3
		ICU.DTCER[224].BIT.DTCE	= 1U;	// Enable DTC for TGIB3
		IEN(PERIA, INTA223)		= 1U;	// Enable TGIA3 interrupt
		IEN(PERIA, INTA224)		= 1U;	// Enable TGIB3 interrupt
		tcsystr |= 0b00010000;
	}
	if(0 != tcsystr) {
#ifdef	USE_DMA_TGID3
	    DMAC7.DMCNT.BIT.DTE		= 1U;	// Enable DMA7 for TGID3
#endif	// USE_DMA_TGID3
		IEN(PERIA, INTA226)		= 1U;	// Enable TGID3 interrupt
		//
		MSTP(MTU3)				= 0U;	// Release MTU channel 3 from stop state
//		MTU.TRWERA.BIT.RWE		= 1U;	// Enable read/write to MTU3 registers
		//
		MTU.TCSYSTR.BYTE		= tcsystr;	// Start MTU channel 0-3 counter
	}
}

void	PPGClass::stop()
{
	if(invalidPO != pin[0]) {
		PORT2.PMR.BIT.B0		= 0U;	// Disable PortMode as PO0
		ICU.DTCER[209].BIT.DTCE	= 0U;	// Disable DTC for TGIA0
		ICU.DTCER[210].BIT.DTCE	= 0U;	// Disable DTC for TGIB0
		IEN(PERIA, INTA209)		= 0U;	// Disable TGIA0 interrupt
		IEN(PERIA, INTA210)		= 0U;	// Disable TGIB0 interrupt
	}
	if(invalidPO != pin[1]) {
		PORT1.PMR.BIT.B3		= 0U;	// Disable PortMode as PO13
		ICU.DTCER[208].BIT.DTCE = 0U;	// Disable DTC for TGIA1
		ICU.DTCER[216].BIT.DTCE = 0U;	// Disable DTC for TGIB1
		IEN(PERIA, INTA208)		= 0U;	// Disable TGIA1 interrupt
		IEN(PERIA, INTA216)		= 0U;	// Disable TGIB1 interrupt
	}
	if(invalidPO != pin[2]) {
		PORTC.PMR.BIT.B3		= 0U;	// Disable PortMode as PO24
		ICU.DTCER[219].BIT.DTCE = 0U;	// Disable DTC for TGIA2
		ICU.DTCER[220].BIT.DTCE = 0U;	// Disable DTC for TGIB2
		IEN(PERIA, INTA219)		= 0U;	// Disable TGIA2 interrupt
		IEN(PERIA, INTA220)		= 0U;	// Disable TGIB2 interrupt
	}
	if(invalidPO != pin[3]) {
		PORT3.PMR.BIT.B2		= 0U;	// Disable PortMode as PO10
		ICU.DTCER[223].BIT.DTCE	= 0U;	// Disable DTC for TGIA3
		ICU.DTCER[224].BIT.DTCE	= 0U;	// Disable DTC for TGIB3
		IEN(PERIA, INTA223)		= 0U;	// Disable TGIA3 interrupt
		IEN(PERIA, INTA224)		= 0U;	// Disable TGIB3 interrupt
	}
	IEN(PERIA, INTA225)			= 0U;	// Disable TGIC3 interrupt
	IEN(PERIA, INTA226)			= 0U;	// Disable TGID3 interrupt
	MSTP(MTU3)					= 0U;	// Release MTU channel 3 from stop state
	MTU.TCSYSTR.BYTE			= 0U;	// Stop all MTU channel counter
}

void	PPGClass::enableTrigger()
{
	isRunning = true;
#ifdef	USE_DMA_TGID3
	DMAC7.DMSAR				= (void *)&r_mtubuf_tgr[0][0];	// Set DMAC7 source address
	DMAC7.DMDAR				= (void *)&r_mtudtc_tgr[0][0];	// Set DMAC7 destination address
	DMAC7.DMCRA				= 8;							// DMAC7_DMCRA_COUNT;
	DMAC7.DMCNT.BIT.DTE		= 1U;							// Enable DMA7 for TGID3
	IEN(PERIA, INTA226)		= 1U;							// Enable TGID3 interrupt in ICU
#else	// !USE_DMA_TGID3
	IEN(PERIA, INTA226)		= 1U;							// Enable TGID3 interrupt in ICU
#endif	// USE_DMA_TGID3
}

void	PPGClass::setTrigger(
	uint8_t		phase,
	uint32_t	tgra,
	uint32_t	tgrb
)
{
#ifdef	USE_PPG_MICROSECOND
	if(2U > calcTGR(tgra)) { tgra = (2U * PCLKA / (1000U * 1000U)) * cycle_division; }
	if(2U > calcTGR(tgrb)) { tgrb = (2U * PCLKA / (1000U * 1000U)) * cycle_division; }
	if(false == isRunning) {
		r_mtudtc_tgr[phase][TGR_A] = r_mtubuf_tgr[phase][TGR_A] = (uint16_t)(calcTGR(tgra) - 1U) | D_MAGIC_NUM_FOR_MTU;
		r_mtudtc_tgr[phase][TGR_B] = r_mtubuf_tgr[phase][TGR_B] = (uint16_t)(calcTGR(tgrb) - 1U) | D_MAGIC_NUM_FOR_MTU;
	}
	else {
		r_mtubuf_tgr[phase][TGR_A] = (uint16_t)(calcTGR(tgra) - 1U) | D_MAGIC_NUM_FOR_MTU;
		r_mtubuf_tgr[phase][TGR_B] = (uint16_t)(calcTGR(tgrb) - 1U) | D_MAGIC_NUM_FOR_MTU;
	}
#else	// !USE_PPG_MICROSECOND
	if(2U > tgra) { tgra = 2U; }
	if(2U > tgrb) { tgrb = 2U; }
	if(false == isRunning) {
		r_mtudtc_tgr[phase][TGR_A] = r_mtubuf_tgr[phase][TGR_A] = (uint16_t)(tgra - 1U) | D_MAGIC_NUM_FOR_MTU;
		r_mtudtc_tgr[phase][TGR_B] = r_mtubuf_tgr[phase][TGR_B] = (uint16_t)(tgrb - 1U) | D_MAGIC_NUM_FOR_MTU;
	}
	else {
		r_mtubuf_tgr[phase][TGR_A] = (uint16_t)(tgra - 1U) | D_MAGIC_NUM_FOR_MTU;
		r_mtubuf_tgr[phase][TGR_B] = (uint16_t)(tgrb - 1U) | D_MAGIC_NUM_FOR_MTU;
	}
#endif	// USE_PPG_MICROSECOND
}

extern "C" {
void isr_mtu0_tgia() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA209)), used));
void isr_mtu0_tgib() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA210)), used));
void isr_mtu1_tgia() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA208)), used));
void isr_mtu1_tgib() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA216)), used));
void isr_mtu2_tgia() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA219)), used));
void isr_mtu2_tgib() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA220)), used));
void isr_mtu3_tgia() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA223)), used));
void isr_mtu3_tgib() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA224)), used));
void isr_mtu3_tgic() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA225)), used));
void isr_mtu3_tgid() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA226)), used));

void isr_mtu0_tgia()
{
	// nothing to do
}
void isr_mtu0_tgib()
{
	// nothing to do
}
void isr_mtu1_tgia()
{
	// nothing to do
}
void isr_mtu1_tgib()
{
	// nothing to do
}
void isr_mtu2_tgia()
{
	// nothing to do
}
void isr_mtu2_tgib()
{
	// nothing to do
}
void isr_mtu3_tgia()
{
	// nothing to do
}
void isr_mtu3_tgib()
{
	// nothing to do
}
void isr_mtu3_tgic()
{
	// nothing to do
}
void isr_mtu3_tgid()
{
#ifndef	USE_DMA_TGID3
	for(int i = 0; i < PPG_NUM_PHASE; i++) {
		PPG.r_mtudtc_tgr[0][i] = PPG.r_mtubuf_tgr[0][i];
		PPG.r_mtudtc_tgr[1][i] = PPG.r_mtubuf_tgr[1][i];
	}
	IEN(PERIA, INTA226) = 0U;	// Disable TGID3 interrupt in ICU
#endif	// !USE_DMA_TGID3
}
} // end of "extern "C""

PPGClass PPG;

