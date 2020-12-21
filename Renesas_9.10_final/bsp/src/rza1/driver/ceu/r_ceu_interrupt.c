/*******************************************************************************
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
* http://www.renesas.com/disclaimer*
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         r_ceu_interrupt.c
* @version      0.04
* $Rev: 1739 $
* $Date:: 2016-08-01 17:51:18 +0100#$
* @brief        CEU driver API functions
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_ceu.h"
#include "r_ceu_interrupt.h"
#include "iodefine.h"
#include "r_typedefs.h"
/******************************************************************************
Typedef definitions
******************************************************************************/
//#define NULL 0
/******************************************************************************
Macro definitions
******************************************************************************/
#define CETCR_CLARE_VALUE (0x00000000u)
#define CEIER_CLARE_VALUE (0x00000000u)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/*! List of the callback function pointers */
static void (*ceu_int_callback)(uint32_t interrupt_flag);

/**************************************************************************//**
 * Function Name: R_CEU_InterruptEnable
 * @brief       The Function which enables interrupt for data enable capture mode.
 *
 *              Description:<br>
 *              This function enables CEU interrupt.<br>
 * @param[in]   flag CEIER reg set value.<br>
 * @return none.
 *****************************************************************************/
void R_CEU_InterruptEnable( const uint32_t flag )
{
    /* Capture Event Interrupt Enable Register (CEIER)  */
    CEU.CEIER = flag;

    return;

}   /* End of function R_CEU_InterruptEnable() */

/**************************************************************************//**
 * Function Name: R_CEU_InterruptDisable
 * @brief       The Function which disables interrupt for data enable capture mode.
 *
 *              Description:<br>
 *              This function disables CEU interrupt.<br>
 * @return none.
 *****************************************************************************/
void R_CEU_InterruptDisable(void)
{
    /* Callback function pointer clear */
    ceu_int_callback = 0;

    /* Capture Event Interrupt Enable Register (CEIER) claer */
    CEU.CEIER = CEIER_CLARE_VALUE;

    return;

}   /* End of function R_CEU_InterruptDisable() */

/**************************************************************************//**
 * Function Name: CEU_Int_SetInterrupt
 * @brief       CEU interrupt callback func set
 *
 *              Description:<br>
 *              CEU interrupt callback func set
 * @param[in]   callback Pointer to the user-defined function
 * @return none.
 *****************************************************************************/
void CEU_Int_SetInterrupt( void (* const callback)(uint32_t interrupt_flag) )
{
    /* Callback function pointer */
    ceu_int_callback = callback;

    return;

}   /* End of function CEU_Int_SetInterrupt() */

/**************************************************************************//**
 * Function Name: CEU_Isr
 * @brief       CEU interrupt handler
 *
 *              Description:<br>
 *              interrupt handler
 * @return none.
 *****************************************************************************/

void CEU_Isr ( const uint32_t int_sense )
{

    CEU.CETCR = 0;     /* clear all */
    CEU.CSTSR = 0;

    return;

}   /* End of function CEU_Isr() */
