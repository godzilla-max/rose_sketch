/*
  PPG.h - PPG library
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

#ifndef __PPG_h__
#define __PPG_h__
#include	"Types.h"
#include	"rx65n/r_cg_dtc.h"
#include	"rx65n/r_cg_mtu3.h"
#include	"rx65n/r_cg_ppg.h"

#define 	USE_PPG_MICROSECOND
#define 	USE_DMA_TGID3

#define 	PPG_NUM_PHASE			(4)
#define 	PCLKA					(120 * 1000 * 1000)
#define 	D_MTU_CYCLE				(3000)
#define		D_MTU_CENTER			(D_MTU_CYCLE / 2)
#define		D_MAGIC_NUM_FOR_MTU		(0b11)				// for PCLKA != PCLKB
#define		D_PRESET_MTUTGR_TIME	(64)

typedef struct {
	uint32_t	mra_mrb;
	uint32_t	sar;
	uint32_t	dar;
	uint32_t	cra_crb;
} st_dtc_data_t;

typedef enum
{
	invalidPO = -1,
	PO0, PO1, PO2, PO3,	PO4, PO5, PO6, PO7, PO8,
	PO9, PO10, PO11, PO12, PO13, PO14, PO15, PO16,
	PO17, PO18, PO19, PO20, PO21, PO22, PO23, PO24,
	PO25, PO26, PO27, PO28, PO29, PO30, PO31, PO32
} NumPo;

typedef enum {
	TGR_A=0, TGR_B, TGR_C, TGR_D, TGR_NUM
} tgr_e;

typedef enum {
	MTU_0=0, MTU_1, MTU_2, MTU_3, MTU_NUM
} mtu_e;


#ifdef __cplusplus
class PPGClass {
public:
	PPGClass();
	bool begin(bool pin0, bool pin1, bool pin2, bool pin3, uint32_t interval, uint8_t polarity = 0b00000000);
	void end();
	void start();
	void stop();
	void enableTrigger(void);
	void setTrigger(uint8_t phase, uint32_t triggerA_time, uint32_t triggerB_time, bool isRunning = true);
	//
	void R_Config_DTC_Create(void);
	void R_Config_MTU_Create(void);
	void R_Config_PPG_Create(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t polarity);
	//
	volatile uint16_t	r_mtudtc_tgr[MTU_NUM][TGR_NUM], r_mtubuf_tgr[MTU_NUM][TGR_NUM];
//	volatile uint16_t	r_mtudtc_tgrc, r_mtubuf_tgrc, r_mtu_tgrd;
	//
	uint32_t	cycle_count;
	uint16_t	cycle_division;
private:
	bool isBitSet(uint8_t value, uint8_t bit);
	//
	void R_Config_DTC_TGIA0_Create(void);
	void R_Config_DTC_TGIA1_Create(void);
	void R_Config_DTC_TGIA2_Create(void);
	void R_Config_DTC_TGIA3_Create(void);
	void R_Config_DTC_TGIB0_Create(void);
	void R_Config_DTC_TGIB1_Create(void);
	void R_Config_DTC_TGIB2_Create(void);
	void R_Config_DTC_TGIB3_Create(void);
	void R_Config_DMAC7_TGID3_Create(void);
	//
	void R_Config_MTU0_Create(void);
	void R_Config_MTU1_Create(void);
	void R_Config_MTU2_Create(void);
	void R_Config_MTU3_Create(void);
	void R_Config_PPG0_Create(uint8_t pin0, bool pol0, uint8_t pin1, bool pol1, uint8_t pin2, bool pol2);
	void R_Config_PPG1_Create(uint8_t pin3, bool pol3);
	//
	volatile uint8_t	pin[PPG_NUM_PHASE];
};
#endif

extern PPGClass PPG;
#endif	// __PPG_h__
