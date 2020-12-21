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
******************************************************************************/
/******************************************************************************
* File Name     : dev_drv.h
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : Platform Independent
* Description   : Device driver header
******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 06.07.2016 1.00    Ported from RZA1H
*******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef DEV_DRV_H
#define DEV_DRV_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* Success */
#define DEVDRV_SUCCESS      (0)
/* Failure */
#define DEVDRV_ERROR        (-1)
/* NACK reception (No acknowledge from the slave device) */
#define DEVDRV_ERROR_RIIC_NACK  (-2)
/* Timeout */
#define DEVDRV_ERROR_TIMEOUT      (-3)

/* Flag OFF */
#define DEVDRV_FLAG_OFF     (0)
/* Flag ON  */
#define DEVDRV_FLAG_ON      (1)

typedef enum devdrv_ch
{
    /* Channel 0  */
    DEVDRV_CH_0,

    /* Channel 1  */
    DEVDRV_CH_1,

    /* Channel 2  */
    DEVDRV_CH_2,

    /* Channel 3  */
    DEVDRV_CH_3,

    /* Channel 4  */
    DEVDRV_CH_4,

    /* Channel 5  */
    DEVDRV_CH_5,

    /* Channel 6  */
    DEVDRV_CH_6,

    /* Channel 7  */
    DEVDRV_CH_7,

    /* Channel 8  */
    DEVDRV_CH_8,

    /* Channel 9  */
    DEVDRV_CH_9,

    /* Channel 10 */
    DEVDRV_CH_10,

    /* Channel 11 */
    DEVDRV_CH_11,

    /* Channel 12 */
    DEVDRV_CH_12,

    /* Channel 13 */
    DEVDRV_CH_13,

    /* Channel 14 */
    DEVDRV_CH_14,

    /* Channel 15 */
    DEVDRV_CH_15
} devdrv_ch_t;

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/

#endif  /* DEV_DRV_H */

/* End of File */
