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
 * File Name     : r_switch_driver.h
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONE-EABI-v16.01
 * H/W Platform  : Stream it! v2
 * Description   : Routines for handling USER key switch input.
 *
 *******************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.06.2016 1.00     First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

#ifndef USER_SWITCH_H_
    #define USER_SWITCH_H_

    #include "r_typedefs.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
    #if STREAM_IT_VERSION < 2
        #define USER_SWITCH          ((GPIO.PPR1 & GPIO_P1_P111) != GPIO_P1_P111)
        #define USER_SWITCH_PRESSED  ((GPIO.PPR1 & GPIO_P1_P111) != GPIO_P1_P111)
        #define USER_SWITCH_RELEASED ((GPIO.PPR1 & GPIO_P1_P111) == GPIO_P1_P111)
#else
        #define USER_SWITCH          ((GPIO.PPR7 & GPIO_P7_P79) != GPIO_P7_P79)
        #define USER_SWITCH_PRESSED  ((GPIO.PPR7 & GPIO_P7_P79) != GPIO_P7_P79)
        #define USER_SWITCH_RELEASED ((GPIO.PPR7 & GPIO_P7_P79) == GPIO_P7_P79)
    #endif

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/
extern volatile uint8_t g_switch_press_flg;

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
void R_SWITCH_Init (bool_t interrupt);
void R_SWITCH_SetPressCallback (void (*func) (void));
void R_SWITCH_SetReleaseCallback (void (*callback) (void));

#endif /* USER_SWITCH_H_ */
