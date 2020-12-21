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
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name     : r_led_drive.c
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONE-EABI-v16.01
 * H/W Platform  : Stream it! v2
 * Description   : Sample Program - LED setting
 *               : Note
 *               : LED D13 is connected directly to processor
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 20.07.2016 1.00
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <r_bsp.h>            /* Device Driver common header */
/* Interface for directly connected LED header */
#include "r_led_drive.h"


/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/******************************************************************************
 * Function Name: R_LED_Init
 * Description  : Initialise port for LED.
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void R_LED_Init (void)
{
    /* ---- P7_8 : D32 direct connection to IP */
    RZA_IO_RegWrite_16(&GPIO.PMC7, 0, GPIO_PMC7_PMC78_SHIFT, GPIO_PMC7_PMC78);
    RZA_IO_RegWrite_16(&GPIO.P7, 1, GPIO_P7_P78_SHIFT, GPIO_P7_P78);
    RZA_IO_RegWrite_16(&GPIO.PM7, 0, GPIO_PM7_PM78_SHIFT, GPIO_PM7_PM78);
    RZA_IO_RegWrite_16(&GPIO.PIPC7, 0, GPIO_PIPC7_PIPC78_SHIFT, GPIO_PIPC7_PIPC78);
}

/******************************************************************************
 * End of Function R_LED_Init
 ******************************************************************************/

/*******************************************************************************
 * Function Name: R_LED_On
 * Description  : LED is turned on.
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void R_LED_On (void)
{
    RZA_IO_RegWrite_16(&GPIO.P7, 1, GPIO_P7_P78_SHIFT, GPIO_P7_P78);
}

/******************************************************************************
 * End of Function R_LED_On
 ******************************************************************************/

/*******************************************************************************
 * Function Name: R_LED_Off
 * Description  : LED is turned off.
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void R_LED_Off (void)
{
    RZA_IO_RegWrite_16(&GPIO.P7, 0, GPIO_P7_P78_SHIFT, GPIO_P7_P78);
}

/******************************************************************************
 * End of Function R_LED_Off
 ******************************************************************************/

/*******************************************************************************
 * Function Name: R_LED_Toggle
 * Description  : LED is toggled.
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void R_LED_Toggle (void)
{
    uint16_t tmp = RZA_IO_RegRead_16(&GPIO.P7,
    GPIO_P7_P78_SHIFT,
    GPIO_P7_P78);

    if (tmp)
    {
        RZA_IO_RegWrite_16(&GPIO.P7, 0, GPIO_P7_P78_SHIFT, GPIO_P7_P78);
    }
    else
    {
        RZA_IO_RegWrite_16(&GPIO.P7, 1, GPIO_P7_P78_SHIFT, GPIO_P7_P78);
    }
}

/******************************************************************************
 * End of Function R_LED_Toggle
 ******************************************************************************/

/* End of File */

