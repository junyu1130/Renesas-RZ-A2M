/******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *******************************************************************************
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : r_lcd_settings.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2
 * Description  : VDC5 driver LCD panel settings sample
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 02.08.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include <r_bsp.h>            /* Device Driver common header */
#include "r_lcd_panel.h"


/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_LCD_SetLcdPanel
 Description:   Invokes user low-level hardware setup function for the LCD
 panel.
 Parameters:    channel - VDC Channel for the settings,
 Return value:  None
 *****************************************************************************/
void R_LCD_SetLcdPanel (vdc5_channel_t channel)
{
    if (channel == VDC5_CHANNEL_0)
    {
        R_LCD_SetLcdPanelCh0();
    }
}

/*****************************************************************************
 End of function  R_LCD_SetLcdPanel
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_LCD_SetLcdTconSettings
 Description:   Set timing control parameters in VDC5 for the LCD panel.
 Parameters:    channel - VDC Channel for the settings,
 outctrl - Timing control parameters to set
 Return value:  None
 *****************************************************************************/
void R_LCD_SetLcdTconSettings (vdc5_channel_t channel, const vdc5_lcd_tcon_timing_t * * outctrl)
{
    /* LCD TCON timing setting */
    if (channel == VDC5_CHANNEL_0)
    {
        R_LCD_SetLcdTconSettingsCh0(outctrl);
    }
}

/*****************************************************************************
 End of function  R_LCD_SetLcdTconSettings
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
