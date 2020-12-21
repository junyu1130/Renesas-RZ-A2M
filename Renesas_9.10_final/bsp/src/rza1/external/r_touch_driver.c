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
 * File Name    : r_touch_driver.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2
 * Description  : Abstraction layer for touch screen controllers.
 *                Provides a common interface and can be extended to support
 *                different controllers at build-time.
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.08.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <math.h>

#include <r_bsp.h>            /* Device Driver common header */
#include "r_touch_capacitive.h"
#include "r_touch_driver.h"

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void refresh_touch_position (void);

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
uint16_t g_touch_Xpos = 0;
uint16_t g_touch_Ypos = 0;
uint8_t g_touch_Touch = 0;
uint16_t g_touch_MultiPoint[5][2];

/*****************************************************************************
 Function Name: R_TOUCH_GetTouchX
 Description:   Gets the X touch position.
 Parameters:    None
 Return value:  X - touch position.
 *****************************************************************************/
uint16_t R_TOUCH_GetTouchX (void)
{
    return g_touch_Xpos;
}

/*****************************************************************************
 End of function  R_TOUCH_GetTouchX
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_TOUCH_GetTouchY
 Description:   Gets the Y touch position.
 Parameters:    None
 Return value:  Y - touch position.
 *****************************************************************************/
uint16_t R_TOUCH_GetTouchY (void)
{
    return g_touch_Ypos;
}

/*****************************************************************************
 End of function  R_TOUCH_GetTouchY
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_TOUCH_NewTouchPosition
 Description:   Refreshes the touch positions.
 Parameters:    None
 Return value:  1 - screen is touched; 0 - screen is not touched.
 *****************************************************************************/
uint16_t R_TOUCH_NewTouchPosition (void)
{
    static uint16_t x;
    static uint16_t y;
    bool_t ret;
    refresh_touch_position();

    ret = (x != g_touch_Xpos) || (y != g_touch_Ypos);
    x = g_touch_Xpos;
    y = g_touch_Ypos;
    return (ret && g_touch_Touch);
}

/*****************************************************************************
 End of function  R_TOUCH_NewTouchPosition
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: refresh_touch_position
 Description:   Calls lower level refresh position routines.
 Compile time options can be placed here to invoke
 different drivers.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void refresh_touch_position (void)
{
    R_TOUCH_RefreshCapPosition();

    /* ... Place calls to other touch drivers here ... */
}

/*****************************************************************************
 End of function  refresh_touch_position
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

