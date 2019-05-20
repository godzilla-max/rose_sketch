/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : PPG_sub.cpp
* Version      : 1.0.0
* Device(s)    : R5F565NEHxFP
* Description  : This file implements device driver for PPG_DTC_MTU
* Creation Date: 2019-05-05
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/

#pragma GCC optimize("O0")

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include	"iodefine.h"
#include	"r_cg_macrodriver.h"
#include	"PPG.h"
#include	"r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

const uint8_t	r_ppg0_ndra[] = {0x00, 0x0F, 0xF0, 0xF0};					// PO0, PO13, PO24, PO10
const uint8_t	r_ppg0_ndrb[] = {0x01, 0x2F, 0xF1, 0xF4};					// PO0, PO13, PO24, PO10
const uint8_t	r_ppg0_ndrs[] = {0x01, 0x0F, 0xF0, 0xF4};					// PO0, PO13, PO24, PO10
const uint32_t	mtu_tgia_adr[PPG_NUM_PHASE] = {0x000C1308, 0x000C1388, 0x000C1408, 0x000C1218};
const uint32_t	mtu_tgib_adr[PPG_NUM_PHASE] = {0x000C130A, 0x000C138A, 0x000C140A, 0x000C121A};
const uint32_t	ppg_ndr_adr[PPG_NUM_PHASE]  = {0x000881ED, 0x000881EC, 0x000881FE, 0x000881EE};

extern volatile uint32_t dtc_vector[];

volatile st_dtc_data_t dtc_transferdata_vector209;
volatile st_dtc_data_t dtc_transferdata_vector208;
volatile st_dtc_data_t dtc_transferdata_vector219;
volatile st_dtc_data_t dtc_transferdata_vector223;
volatile st_dtc_data_t dtc_transferdata_vector210[3];
volatile st_dtc_data_t dtc_transferdata_vector216[3];
volatile st_dtc_data_t dtc_transferdata_vector220[3];
volatile st_dtc_data_t dtc_transferdata_vector224[3];
volatile st_dtc_data_t dtc_transferdata_vector226[3];

/***********************************************************************************************************************
* Function Name: R_Config_DTC_TGIA0_Create
* Description  : This function initializes the DTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_TGIA0_Create(void)
{
	dtc_transferdata_vector209.mra_mrb	= (uint32_t)(1) << 24U;				// WBDIS
	dtc_transferdata_vector209.sar		= (uint32_t)&r_ppg0_ndra[0];		// PO0
	dtc_transferdata_vector209.dar		= ppg_ndr_adr[0];
	dtc_transferdata_vector209.cra_crb	= (uint32_t)(1) << 16U;				// TRANSFER_COUNT_CRA
}

/***********************************************************************************************************************
* Function Name: R_Config_DTC_TGIA1_Create
* Description  : This function initializes the DTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_TGIA1_Create(void)
{
	dtc_transferdata_vector208.mra_mrb	= ((uint32_t)(1) << 24U);			// WBDIS
	dtc_transferdata_vector208.sar		= (uint32_t)&r_ppg0_ndra[1];		// PO13
	dtc_transferdata_vector208.dar		= ppg_ndr_adr[1];
	dtc_transferdata_vector208.cra_crb	= (uint32_t)(1) << 16U;				// TRANSFER_COUNT_CRA
}

/***********************************************************************************************************************
* Function Name: R_Config_DTC_TGIA2_Create
* Description  : This function initializes the DTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_TGIA2_Create(void)
{
	dtc_transferdata_vector219.mra_mrb	= (uint32_t)(1) << 24U;				// WBDIS
	dtc_transferdata_vector219.sar		= (uint32_t)&r_ppg0_ndra[2];		// PO24
	dtc_transferdata_vector219.dar		= ppg_ndr_adr[2];
	dtc_transferdata_vector219.cra_crb	= (uint32_t)(1) << 16U;				// TRANSFER_COUNT_CRA
}

/***********************************************************************************************************************
* Function Name: R_Config_DTC_TGIA3_Create
* Description  : This function initializes the DTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_TGIA3_Create(void)
{
	dtc_transferdata_vector223.mra_mrb	= (uint32_t)(1) << 24U;				// WBDIS
	dtc_transferdata_vector223.sar		= (uint32_t)&r_ppg0_ndra[3];		// PO10
	dtc_transferdata_vector223.dar		= ppg_ndr_adr[3];
	dtc_transferdata_vector223.cra_crb	= (uint32_t)(1) << 16U;				// TRANSFER_COUNT_CRA
}

/***********************************************************************************************************************
* Function Name: R_Config_DTC_TGIB0_Create
* Description  : This function initializes the DTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_TGIB0_Create(void)
{
	/* Set DTC0 transfer data */
	dtc_transferdata_vector210[0].mra_mrb	= 0x01800000;							// WBDIS | CHAIN_TRANSFER_ENABLED
	dtc_transferdata_vector210[0].sar		= (uint32_t)&r_ppg0_ndrb[0];			// PO0
	dtc_transferdata_vector210[0].dar		= ppg_ndr_adr[0];
	dtc_transferdata_vector210[0].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
	/* Set DTC1 transfer data */
	dtc_transferdata_vector210[1].mra_mrb	= 0x11800000;							// 16BIT | WBDIS | CHAIN_TRANSFER_ENABLED
	dtc_transferdata_vector210[1].sar		= (uint32_t)&r_mtudtc_tgr[MTU_0][0];	// PO0
	dtc_transferdata_vector210[1].dar		= mtu_tgia_adr[0];
	dtc_transferdata_vector210[1].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
	/* Set DTC2 transfer data */
	dtc_transferdata_vector210[2].mra_mrb	= 0x11000000;							// 16BIT | WBDIS
	dtc_transferdata_vector210[2].sar		= (uint32_t)&r_mtudtc_tgr[MTU_0][1];	// PO0
	dtc_transferdata_vector210[2].dar		= mtu_tgib_adr[0];
	dtc_transferdata_vector210[2].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
}

/***********************************************************************************************************************
* Function Name: R_Config_DTC_TGIB1_Create
* Description  : This function initializes the DTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_TGIB1_Create(void)
{
	/* Set DTC0 transfer data */
	dtc_transferdata_vector216[0].mra_mrb	= 0x01800000;							// WBDIS | CHAIN_TRANSFER_ENABLED
	dtc_transferdata_vector216[0].sar		= (uint32_t)&r_ppg0_ndrb[1];			// PO13
	dtc_transferdata_vector216[0].dar		= ppg_ndr_adr[1];
	dtc_transferdata_vector216[0].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
	/* Set DTC1 transfer data */
	dtc_transferdata_vector216[1].mra_mrb	= 0x11800000;							// 16BIT | WBDIS | CHAIN_TRANSFER_ENABLED
	dtc_transferdata_vector216[1].sar		= (uint32_t)&r_mtudtc_tgr[MTU_1][0];	// PO13
	dtc_transferdata_vector216[1].dar		= mtu_tgia_adr[1];
	dtc_transferdata_vector216[1].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
	/* Set DTC2 transfer data */
	dtc_transferdata_vector216[2].mra_mrb	= 0x11000000;							// 16BIT | WBDIS
	dtc_transferdata_vector216[2].sar		= (uint32_t)&r_mtudtc_tgr[MTU_1][1];	// PO13
	dtc_transferdata_vector216[2].dar		= mtu_tgib_adr[1];
	dtc_transferdata_vector216[2].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
}

/***********************************************************************************************************************
* Function Name: R_Config_DTC_TGIB2_Create
* Description  : This function initializes the DTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_TGIB2_Create(void)
{
	/* Set DTC0 transfer data */
	dtc_transferdata_vector220[0].mra_mrb	= 0x01800000;							// WBDIS | CHAIN_TRANSFER_ENABLED
	dtc_transferdata_vector220[0].sar		= (uint32_t)&r_ppg0_ndrb[2];			// PO24
	dtc_transferdata_vector220[0].dar		= ppg_ndr_adr[2];
	dtc_transferdata_vector220[0].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
	/* Set DTC1 transfer data */
	dtc_transferdata_vector220[1].mra_mrb	= 0x11800000;							// 16BIT | WBDIS | CHAIN_TRANSFER_ENABLED
	dtc_transferdata_vector220[1].sar		= (uint32_t)&r_mtudtc_tgr[MTU_2][0];	// PO24
	dtc_transferdata_vector220[1].dar		= mtu_tgia_adr[2];
	dtc_transferdata_vector220[1].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
	/* Set DTC2 transfer data */
	dtc_transferdata_vector220[2].mra_mrb	= 0x11000000;							// 16BIT | WBDIS
	dtc_transferdata_vector220[2].sar		= (uint32_t)&r_mtudtc_tgr[MTU_2][1];	// PO24
	dtc_transferdata_vector220[2].dar		= mtu_tgib_adr[2];
	dtc_transferdata_vector220[2].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
}

/***********************************************************************************************************************
* Function Name: R_Config_DTC_TGIB3_Create
* Description  : This function initializes the DTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_TGIB3_Create(void)
{
	/* Set DTC0 transfer data */
	dtc_transferdata_vector224[0].mra_mrb	= 0x01800000;							// WBDIS | CHAIN_TRANSFER_ENABLED
	dtc_transferdata_vector224[0].sar		= (uint32_t)&r_ppg0_ndrb[3];			// PO10
	dtc_transferdata_vector224[0].dar		= ppg_ndr_adr[3];
	dtc_transferdata_vector224[0].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
	/* Set DTC1 transfer data */
	dtc_transferdata_vector224[1].mra_mrb	= 0x11800000;							// 16BIT | WBDIS | CHAIN_TRANSFER_ENABLED
	dtc_transferdata_vector224[1].sar		= (uint32_t)&r_mtudtc_tgr[MTU_3][0];	// PO10
	dtc_transferdata_vector224[1].dar		= mtu_tgia_adr[3];
	dtc_transferdata_vector224[1].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
	/* Set DTC2 transfer data */
	dtc_transferdata_vector224[2].mra_mrb	= 0x11000000;							// 16BIT | WBDIS
	dtc_transferdata_vector224[2].sar		= (uint32_t)&r_mtudtc_tgr[MTU_3][1];	// PO10
	dtc_transferdata_vector224[2].dar		= mtu_tgib_adr[3];
	dtc_transferdata_vector224[2].cra_crb	= (uint32_t)(1) << 16U;					// TRANSFER_COUNT_CRA
}

#ifdef	USE_DMA_TGID3
/***********************************************************************************************************************
* Function Name: R_Config_DMAC7_TGID3_Create
* Description  : This function initializes the DMAC7 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DMAC7_TGID3_Create(void)
{
	MSTP(DMAC)			= 0U;							// Cancel DMAC/DTC module stop state in LPC
	IEN(DMAC,DMAC74I)	= 0U;							// Disable DMAC interrupts
	DMAC7.DMCNT.BIT.DTE	= 0U;							// Disable DMAC7 transfer
	ICU.DMRSR7			= 0xE2;							// Set DMAC7 activation source
	DMAC7.DMAMD.WORD	= 0x8080;						// SRC_ADDR_UPDATE_INCREMENT | DST_ADDR_UPDATE_INCREMENT
	DMAC7.DMTMD.WORD	= 0x2101;						// REPEAT_AREA_NONE | DATA_SIZE_16 | REQ_SOURCE_INT
	DMAC7.DMCSL.BYTE	= 0x00;							// Set DMAC7 interrupt flag control
	DMAC7.DMSAR			= (void *)&r_mtubuf_tgr[0][0];	// Set DMAC7 source address
	DMAC7.DMDAR			= (void *)&r_mtudtc_tgr[0][0];	// Set DMAC7 destination address
	DMAC7.DMCRA			= MTU_NUM * TGR_NUM;			// DMAC7_DMCRA_COUNT;
	DMAC.DMAST.BIT.DMST	= 1U;							// Enable DMAC activation
}
#endif	// USE_DMA_TGID3

/***********************************************************************************************************************
* Function Name: R_Config_MTU0_Create
* Description  : This function initializes the MTU0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_MTU0_Create(void)
{
	/* Release MTU channel 0 from stop state */
	MSTP(MTU3)				= 0U;
	/* Stop MTU channel 0 counter */
	MTU.TSTRA.BIT.CST0		= 0U;
	/* Set TGIA0 interrupt priority level */
	ICU.SLIAR209.BYTE		= 0x01U;
	IPR(PERIA, INTA209)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* Set TGIB0 interrupt priority level */
	ICU.SLIAR210.BYTE		= 0x02U;
	IPR(PERIA, INTA210)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* MTU channel 0 is used as normal mode */
	MTU.TSYRA.BIT.SYNC0		= 1U;
	MTU0.TCR.BYTE			= 0x60;				// _00_MTU_PCLK_1 | _60_MTU_CKCL_SYN
	MTU0.TCR2.BYTE			= 0x00;				// _00_MTU_PCLK_1;
	switch(cycle_division) {
	case    1:	/* nothing to do */		break;	// no-changed
	case    2:	MTU0.TCR2.BYTE	= 0x01;	break;	// _01_MTU_PCLK_2;
	case    4:	MTU0.TCR.BYTE	= 0x61;	break;	// _01_MTU_PCLK_4 | _60_MTU_CKCL_SYN
	case    8:	MTU0.TCR2.BYTE	= 0x02;	break;	// _02_MTU_PCLK_8;
	case   16:	MTU0.TCR.BYTE	= 0x62;	break;	// _02_MTU_PCLK_16 | _60_MTU_CKCL_SYN
	case   32:	MTU0.TCR2.BYTE	= 0x03;	break;	// _03_MTU_PCLK_32
	case   64:	MTU0.TCR.BYTE	= 0x63;	break;	// _03_MTU_PCLK_64 | _60_MTU_CKCL_SYN
	case  128:	/* nothing to do */		break;	// not working
	case  256:	MTU0.TCR2.BYTE	= 0x04;	break;	// _04_MTU_PCLK_256;
	case  512:	/* nothing to do */		break;	// not working
	case 1024:	MTU0.TCR2.BYTE	= 0x05;	break;	// _05_MTU_PCLK_1024;
	default:	/* nothing to do */		break;	// not working
	}
	MTU0.TIER.BYTE			= 0x01|0x02;		// _01_MTU_TGIEA_ENABLE | _02_MTU_TGIEB_ENABLE
	MTU0.TIER2.BYTE			= 0x00;
	MTU0.TIORH.BYTE			= 0x00;
	MTU0.TIORL.BYTE			= 0x00;
	MTU0.TGRA				= r_mtudtc_tgr[MTU_0][TGR_A];
	MTU0.TGRB				= r_mtudtc_tgr[MTU_0][TGR_B];
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU1_Create
* Description  : This function initializes the MTU1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_MTU1_Create(void)
{
	/* Release MTU channel 1 from stop state */
	MSTP(MTU3)				= 0U;
	/* Stop MTU channel 1 counter */
	MTU.TSTRA.BIT.CST1		= 0U;
	/* Set TGIA1 interrupt priority level */
	ICU.SLIAR208.BYTE		= 0x08U;
	IPR(PERIA, INTA208)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* Set TGIB1 interrupt priority level */
	ICU.SLIAR216.BYTE		= 0x09U;
	IPR(PERIA, INTA216)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* MTU channel 1 is used as normal mode */
	MTU.TSYRA.BIT.SYNC1		= 1U;
	MTU1.TCR.BYTE			= 0x60;				// _00_MTU_PCLK_1 | _60_MTU_CKCL_SYN;
	MTU1.TCR2.BYTE			= 0x00;				// _00_MTU_PCLK_1;
	switch(cycle_division) {
	case    1:	/* nothing to do */		break;	// no-changed
	case    2:	MTU1.TCR2.BYTE	= 0x01;	break;	// _01_MTU_PCLK_2;
	case    4:	MTU1.TCR.BYTE	= 0x61;	break;	// _01_MTU_PCLK_4  | _60_MTU_CKCL_SYN
	case    8:	MTU1.TCR2.BYTE	= 0x02;	break;	// _02_MTU_PCLK_8;
	case   16:	MTU1.TCR.BYTE	= 0x62;	break;	// _02_MTU_PCLK_16 | _60_MTU_CKCL_SYN
	case   32:	MTU1.TCR2.BYTE	= 0x03;	break;	// _03_MTU_PCLK_32
	case   64:	MTU1.TCR.BYTE	= 0x63;	break;	// _03_MTU_PCLK_64 | _60_MTU_CKCL_SYN
	case  128:	/* nothing to do */		break;	// not working
	case  256:	MTU1.TCR2.BYTE	= 0x04;	break;	// _04_MTU_PCLK_256;
	case  512:	/* nothing to do */		break;	// not working
	case 1024:	MTU1.TCR2.BYTE	= 0x05;	break;	// _05_MTU_PCLK_1024;
	default:	/* nothing to do */		break;	// not working
	}
	MTU1.TIER.BYTE			= 0x01|0x02;		// _01_MTU_TGIEA_ENABLE | _02_MTU_TGIEB_ENABLE
	MTU1.TIOR.BYTE			= 0x00;
	MTU1.TGRA				= r_mtudtc_tgr[MTU_1][TGR_A];
	MTU1.TGRB				= r_mtudtc_tgr[MTU_1][TGR_B];
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU2_Create
* Description  : This function initializes the MTU2 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_MTU2_Create(void)
{
	/* Release MTU channel 2 from stop state */
	MSTP(MTU3)				= 0U;
	/* Stop MTU channel 2 counter */
	MTU.TSTRA.BIT.CST2		= 0U;
	/* Set TGIA2 interrupt priority level */
	ICU.SLIAR219.BYTE		= 0x0CU;
	IPR(PERIA, INTA219)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* Set TGIB2 interrupt priority level */
	ICU.SLIAR220.BYTE		= 0x0DU;
	IPR(PERIA, INTA220)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* MTU channel 2 is used as normal mode */
	MTU.TSYRA.BIT.SYNC2		= 1U;
	MTU2.TCR.BYTE			= 0x60;				// _00_MTU_PCLK_1 | _60_MTU_CKCL_SYN;
	MTU2.TCR2.BYTE			= 0x00;				// _00_MTU_PCLK_1;
	switch(cycle_division) {
	case    1:	/* nothing to do */		break;	// no-changed
	case    2:	MTU2.TCR2.BYTE	= 0x01;	break;	// _01_MTU_PCLK_2;
	case    4:	MTU2.TCR.BYTE	= 0x61;	break;	// _01_MTU_PCLK_4  | _60_MTU_CKCL_SYN
	case    8:	MTU2.TCR2.BYTE	= 0x02;	break;	// _02_MTU_PCLK_8;
	case   16:	MTU2.TCR.BYTE	= 0x62;	break;	// _02_MTU_PCLK_16 | _60_MTU_CKCL_SYN
	case   32:	MTU2.TCR2.BYTE	= 0x03;	break;	// _03_MTU_PCLK_32
	case   64:	MTU2.TCR.BYTE	= 0x63;	break;	// _03_MTU_PCLK_64 | _60_MTU_CKCL_SYN
	case  128:	/* nothing to do */		break;	// not working
	case  256:	MTU2.TCR2.BYTE	= 0x04;	break;	// _04_MTU_PCLK_256;
	case  512:	/* nothing to do */		break;	// not working
	case 1024:	MTU2.TCR2.BYTE	= 0x05;	break;	// _05_MTU_PCLK_1024;
	default:	/* nothing to do */		break;	// not working
	}
	MTU2.TIER.BYTE			= 0x01|0x02;		// _01_MTU_TGIEA_ENABLE | _02_MTU_TGIEB_ENABLE
	MTU2.TIOR.BYTE			= 0x00;
	MTU2.TGRA				= r_mtudtc_tgr[MTU_2][TGR_A];
	MTU2.TGRB				= r_mtudtc_tgr[MTU_2][TGR_B];
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU3_Create
* Description  : This function initializes the MTU3 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_MTU3_Create(void)
{
	/* Release MTU channel 3 from stop state */
	MSTP(MTU3)				= 0U;
	/* Enable read/write to MTU3 registers */
	MTU.TRWERA.BYTE			= 0x01;				// _01_MTU_RWE_ENABLE;
	/* Stop MTU channel 3 counter */
	MTU.TSTRA.BIT.CST3		= 0U;
	/* Set TGIA3 interrupt priority level */
	ICU.SLIAR223.BYTE		= 0x10U;
	IPR(PERIA, INTA223)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* Set TGIB3 interrupt priority level */
	ICU.SLIAR224.BYTE		= 0x11U;
	IPR(PERIA, INTA224)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* Set TGIC3 interrupt priority level */
	ICU.SLIAR225.BYTE		= 0x12U;
	IPR(PERIA, INTA225)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* Set TGID3 interrupt priority level */
	ICU.SLIAR226.BYTE		= 0x13U;
	IPR(PERIA, INTA226)		= 10;				// _0A_MTU_PRIORITY_LEVEL10;
	/* MTU channel 3 is used as normal mode */
	MTU.TSYRA.BIT.SYNC3		= 1U;
	MTU3.TCR.BYTE			= 0xA0;				// _00_MTU_PCLK_1 | _A0_MTU_CKCL_C;
	MTU3.TCR2.BYTE			= 0x00;				// _00_MTU_PCLK_1;
	switch(cycle_division) {
	case    1:	/* nothing to do */		break;	// no-changed
	case    2:	MTU3.TCR2.BYTE	= 0x01;	break;	// _01_MTU_PCLK_2;
	case    4:	MTU3.TCR.BYTE	= 0xA1;	break;	// _01_MTU_PCLK_4  | _A0_MTU_CKCL_C
	case    8:	MTU3.TCR2.BYTE	= 0x02;	break;	// _02_MTU_PCLK_8;
	case   16:	MTU3.TCR.BYTE	= 0xA2;	break;	// _02_MTU_PCLK_16 | _A0_MTU_CKCL_C
	case   32:	MTU3.TCR2.BYTE	= 0x03;	break;	// _03_MTU_PCLK_32
	case   64:	MTU3.TCR.BYTE	= 0xA3;	break;	// _03_MTU_PCLK_64 | _A0_MTU_CKCL_C
	case  128:	/* nothing to do */		break;	// not working
	case  256:	MTU3.TCR2.BYTE	= 0x04;	break;	// _04_MTU_PCLK_256;
	case  512:	/* nothing to do */		break;	// not working
	case 1024:	MTU3.TCR2.BYTE	= 0x05;	break;	// _05_MTU_PCLK_1024;
	default:	/* nothing to do */		break;	// not working
	}
	MTU3.TIER.BYTE			= 0x01|0x02|0x04|0x08;	// _01_MTU_TGIEA_ENABLE | _02_MTU_TGIEB_ENABLE | _04_MTU_TGIEC_ENABLE | _08_MTU_TGIED_ENABLE
	MTU3.TIORH.BYTE			= 0x00;
	MTU3.TIORL.BYTE			= 0x00;
	MTU3.TGRA				= r_mtudtc_tgr[MTU_3][TGR_A];
	MTU3.TGRB				= r_mtudtc_tgr[MTU_3][TGR_B];
	MTU3.TGRC 				= r_mtudtc_tgr[MTU_3][TGR_C];
	MTU3.TGRD 				= r_mtudtc_tgr[MTU_3][TGR_D];
	/* Disable read/write to MTU3 registers */
//	MTU.TRWERA.BIT.RWE		= 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_PPG0_Create
* Description  : This function initializes the PPG0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_PPG0_Create(
		uint8_t		pin0,
		bool		pol0,
		uint8_t		pin3,
		bool		pol3,
		uint8_t		pin1,
		bool		pol1
)
{
	/* Cancel PPG module stop state */
	MSTP(PPG0)			= 0U;
	/* Set initial output values */
	PPG0.PODRL.BYTE		= 0x00;								// _00_PPG_OUTPUT_DATA_PO0_LOW
	PPG0.PODRH.BYTE		= 0x00;								// _00_PPG_OUTPUT_DATA_PO10_LOW | _00_PPG_OUTPUT_DATA_PO13_LOW
	/* Enable next data transfer */
	PPG0.NDERL.BYTE		= 0x01;								// _01_PPG_NEXT_DATA_PO0_ENABLED
	PPG0.NDERH.BYTE		= 0x04|0x20;						// _04_PPG_NEXT_DATA_PO10_ENABLED | _20_PPG_NEXT_DATA_PO13_ENABLED
	/* Set control registers */
	PPG0.PCR.BYTE		= 0x00|0x30|0x40;					// _00_PPG_GROUP0_COMPARE_MTU0 | _30_PPG_GROUP2_COMPARE_MTU3 | _40_PPG_GROUP3_COMPARE_MTU1
	PPG0.PMR.BYTE		= 0x01|0x04|0x08					// _01_PPG_GROUP0_NON_OVERLAP_ENABLE | _04_PPG_GROUP2_NON_OVERLAP_ENABLE | _08_PPG_GROUP3_NON_OVERLAP_ENABLE
							| ((true == pol0) ? 0 : 0x10)	// _10_PPG_GROUP0_POLARITY_DURECT
							| ((true == pol1) ? 0 : 0x40)	// _40_PPG_GROUP2_POLARITY_DIRECT
							| ((true == pol3) ? 0 : 0x80);	// _80_PPG_GROUP3_POLARITY_DIRECT
	/* Set next pulse output values */
	PPG0.NDRL.BYTE		= 0x01;								// _01_PPG_OUTPUT_DATA_PO0_HIGH
	PPG0.NDRH2.BYTE		= 0x04|0xF0;						// _04_PPG_OUTPUT_DATA_PO10_HIGH | _F0_PPG_NEXT_DATA_2_DEFAULT
	PPG0.NDRH.BYTE		= 0x20|0x0F;						// _20_PPG_OUTPUT_DATA_PO13_HIGH | _0F_PPG_NEXT_DATA_DEFAULT
	if(PO0 == pin0) {
		/* P20 initialized value is Low */
		MPC.P20PFS.BYTE			= 0x06U;	// Set PO0 pin
		PORT2.PODR.BIT.B0		= 0;
		PORT2.PMR.BIT.B0		= 0;
		PORT2.PDR.BIT.B0		= 1;
	}
	if(PO10 == pin3) {
		/* P32 initialized value is Low */
		MPC.P32PFS.BYTE			= 0x06U;	// Set PO10 pin
		PORT3.PODR.BIT.B2		= 0;
		PORT3.PMR.BIT.B2		= 0;
		PORT3.PDR.BIT.B2		= 1;
	}
	if(PO13 == pin1) {
		/* P13 initialized value is Low */
		MPC.P13PFS.BYTE			= 0x06U;	// Set PO13 pin
		PORT1.PODR.BIT.B3		= 0;
		PORT1.PMR.BIT.B3		= 0;
		PORT1.PDR.BIT.B3		= 1;
	}
}

 /***********************************************************************************************************************
 * Function Name: R_Config_PPG1_Create
 * Description  : This function initializes the PPG1 channel
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void	PPGClass::R_Config_PPG1_Create(
	uint8_t		pin2,
	bool		pol2
)
{
	/* Cancel PPG module stop state */
	MSTP(PPG1)			= 0U;
	/* Set initial output values */
	PPG1.PODRH.BYTE		= 0x00;								// _00_PPG_OUTPUT_DATA_PO24_LOW
	/* Enable next data transfer */
	PPG1.NDERH.BYTE		= 0x01;								// _01_PPG_NEXT_DATA_PO24_ENABLED
	/* Set control registers */
	PPG1.PTRSLR.BYTE	= 0x00;								// _00_PPG1_TRIGGER_SEL_MTU
	PPG1.PCR.BYTE		= 0x20;								// _20_PPG_GROUP6_COMPARE_MTU2
	PPG1.PMR.BYTE		= 0x04								// _04_PPG_GROUP6_NON_OVERLAP_ENABLE
							|((true == pol2) ? 0 : 0x40);	// _40_PPG_GROUP6_POLARITY_DIRECT
	/* Set next pulse output values */
	PPG1.NDRH2.BYTE			= 0x01|0xF0;					// _01_PPG_OUTPUT_DATA_PO24_HIGH | _F0_PPG_NEXT_DATA_2_DEFAULT
	if(PO24 == pin2) {
		/* PC3 initialized value is Low */
		MPC.PC3PFS.BYTE		= 0x06U;		// Set PO24 pin
		PORTC.PODR.BIT.B3	= 0;
		PORTC.PMR.BIT.B3	= 0;
		PORTC.PDR.BIT.B3	= 1;
	}
}

/***********************************************************************************************************************
* Function Name: R_Config_DTC_Create
* Description  : This function initializes DTC
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_DTC_Create(void)
{
	MSTP(DTC)			= 0U;			// Cancel DTC/DMAC module stop state
	DTC.DTCCR.BYTE		= 0x08;			// Disable transfer data read skip to clear the flag
	//
	R_Config_DTC_TGIA0_Create();
	R_Config_DTC_TGIA1_Create();
	R_Config_DTC_TGIA2_Create();
	R_Config_DTC_TGIA3_Create();
	R_Config_DTC_TGIB0_Create();
	R_Config_DTC_TGIB1_Create();
	R_Config_DTC_TGIB2_Create();
	R_Config_DTC_TGIB3_Create();
#ifdef	USE_DMA_TGID3
	R_Config_DMAC7_TGID3_Create();
#endif	// USE_DMA_TGID3
	//
	// Set transfer data start address in DTC vector table
	//
	dtc_vector[209]		= (uint32_t)&dtc_transferdata_vector209;
	dtc_vector[208]		= (uint32_t)&dtc_transferdata_vector208;
	dtc_vector[219]		= (uint32_t)&dtc_transferdata_vector219;
	dtc_vector[223]		= (uint32_t)&dtc_transferdata_vector223;
	dtc_vector[210]		= (uint32_t)&dtc_transferdata_vector210[0];
	dtc_vector[216]		= (uint32_t)&dtc_transferdata_vector216[0];
	dtc_vector[220]		= (uint32_t)&dtc_transferdata_vector220[0];
	dtc_vector[224]		= (uint32_t)&dtc_transferdata_vector224[0];
	//
	DTC.DTCADMOD.BYTE	= 0x00;					// Set address mode
	DTC.DTCVBR			= (void*)dtc_vector;	// Set base address
	DTC.DTCST.BYTE		= 0x01;					// Enable DTC module start
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU0_Create
* Description  : This function initializes the MTU0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void	PPGClass::R_Config_MTU_Create(void)
{
	R_Config_MTU0_Create();
	R_Config_MTU1_Create();
	R_Config_MTU2_Create();
	R_Config_MTU3_Create();
}

/***********************************************************************************************************************
 * Function Name: isBitSet
 * Description  : This function checks bit status of value
 * Arguments    : uint8_t value, uint8_t bit
 * Return Value : true = set, false = reset
 ***********************************************************************************************************************/
bool	PPGClass::isBitSet(
	uint8_t		value,
	uint8_t		bit
)
{
	return ((value && (1 << bit)) != 0);
}

/***********************************************************************************************************************
 * Function Name: R_Config_PPG_Create
 * Description  : This function initializes PPG
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void	PPGClass::R_Config_PPG_Create(
	uint8_t		pin0,
	uint8_t		pin1,
	uint8_t		pin2,
	uint8_t		pin3,
	uint8_t		polarity
)
{
	R_Config_PPG0_Create(pin0, isBitSet(polarity, 0), pin3, isBitSet(polarity, 3), pin1, isBitSet(polarity, 1));
	R_Config_PPG1_Create(pin2, isBitSet(polarity, 2));
}
