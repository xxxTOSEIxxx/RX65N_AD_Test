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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_s12ad_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F565NEDxFC
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
* Creation Date: 2021-01-28
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_s12ad_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ADC_PinSet_S12AD0
* Description  : This function initializes pins for r_s12ad_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_ADC_PinSet_S12AD0()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set AN000 pin */
    PORT4.PCR.BIT.B0 = 0U;
    PORT4.PDR.BIT.B0 = 0U;
    PORT4.PMR.BIT.B0 = 0U;
    MPC.P40PFS.BYTE = 0x80U;

    /* Set AN001 pin */
    PORT4.PCR.BIT.B1 = 0U;
    PORT4.PDR.BIT.B1 = 0U;
    PORT4.PMR.BIT.B1 = 0U;
    MPC.P41PFS.BYTE = 0x80U;

    /* Set AN002 pin */
    PORT4.PCR.BIT.B2 = 0U;
    PORT4.PDR.BIT.B2 = 0U;
    PORT4.PMR.BIT.B2 = 0U;
    MPC.P42PFS.BYTE = 0x80U;

    /* Set AN003 pin */
    PORT4.PCR.BIT.B3 = 0U;
    PORT4.PDR.BIT.B3 = 0U;
    PORT4.PMR.BIT.B3 = 0U;
    MPC.P43PFS.BYTE = 0x80U;

    /* Set AN004 pin */
    PORT4.PCR.BIT.B4 = 0U;
    PORT4.PDR.BIT.B4 = 0U;
    PORT4.PMR.BIT.B4 = 0U;
    MPC.P44PFS.BYTE = 0x80U;

    /* Set AN005 pin */
    PORT4.PCR.BIT.B5 = 0U;
    PORT4.PDR.BIT.B5 = 0U;
    PORT4.PMR.BIT.B5 = 0U;
    MPC.P45PFS.BYTE = 0x80U;

    /* Set AN006 pin */
    PORT4.PCR.BIT.B6 = 0U;
    PORT4.PDR.BIT.B6 = 0U;
    PORT4.PMR.BIT.B6 = 0U;
    MPC.P46PFS.BYTE = 0x80U;

    /* Set AN007 pin */
    PORT4.PCR.BIT.B7 = 0U;
    PORT4.PDR.BIT.B7 = 0U;
    PORT4.PMR.BIT.B7 = 0U;
    MPC.P47PFS.BYTE = 0x80U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

