/*******************************************************************************
* DISCLAIMER
* This software is supplied by Rinses Electronics Corporation and is only
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
* @file         r_ceu_user_config.h
* @version      0.04
* $Rev: 1741 $
* $Date:: 2016-08-02 10:11:07 +0100#$
* @brief        CEU sample program configuration header
******************************************************************************/

#ifndef CEU_USER_CONFIG_H
#define CEU_USER_CONFIG_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_ceu.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/* board specific (Don't change) */
#define CEU_CONFIG_BURST_MODE                       CEU_BURST_MODE_X1          /** Transferred to the bus in 4-burst 1-transfer(4QWx1) units */
#define CEU_CONFIG_32BIT_DATA_SWAP                  CEU_DATAOUT_SWAP_ENABLE  //CEU_DATAOUT_SWAP_ENABLE     /** swapped */
#define CEU_CONFIG_16BIT_DATA_SWAP                  CEU_DATAOUT_SWAP_ENABLE //CEU_DATAOUT_SWAP_ENABLE     /** swapped */
#define CEU_CONFIG_8BIT_DATA_SWAP                   CEU_DATAOUT_SWAP_DISABLE    /** not swapped */
#define CEU_CONFIG_CAMERA_SIGNAL_POLARITY_VD        CEU_SIGNAL_HIGH_ACTIVE      /** VD signal is high-active */
#define CEU_CONFIG_CAMERA_SIGNAL_POLARITY_HD        CEU_SIGNAL_HIGH_ACTIVE      /** HD signal is high-active */

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

#endif /* CEU_USER_CONFIG_H */
