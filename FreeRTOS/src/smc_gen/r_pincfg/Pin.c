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
* File Name    : Pin.c
* Version      : 1.0.2
* Device(s)    : R5F565NEDxFP
* Description  : This file implements SMC pin code generation.
* Creation Date: 2019-03-02
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
/* Start user code for include. Do not edit comment generated here */
#include "Pin.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Pins_Create
* Description  : This function initializes Smart Configurator pins
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Pins_Create(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set ET0_MDC pin */
    MPC.PA4PFS.BYTE = 0x11U;
    PORTA.PMR.BYTE |= 0x10U;

    /* Set ET0_MDIO pin */
    MPC.PA3PFS.BYTE = 0x11U;
    PORTA.PMR.BYTE |= 0x08U;

    /* Set REF50CK0 pin */
    MPC.PB2PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x04U;

    /* Set RMII0_CRS_DV pin */
    MPC.PB7PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x80U;

    /* Set RMII0_RXD0 pin */
    MPC.PB1PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x02U;

    /* Set RMII0_RXD1 pin */
    MPC.PB0PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x01U;

    /* Set RMII0_RX_ER pin */
    MPC.PB3PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x08U;

    /* Set RMII0_TXD0 pin */
    MPC.PB5PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x20U;

    /* Set RMII0_TXD1 pin */
    MPC.PB6PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x40U;

    /* Set RMII0_TXD_EN pin */
    MPC.PB4PFS.BYTE = 0x12U;
    PORTB.PMR.BYTE |= 0x10U;

    /* Set RXD2 pin */
    MPC.P52PFS.BYTE = 0x0AU;
    PORT5.PMR.BYTE |= 0x04U;

    /* Set TXD2 pin */
    PORT5.PODR.BYTE |= 0x01U;
    MPC.P50PFS.BYTE = 0x0AU;
    PORT5.PDR.BYTE |= 0x01U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

