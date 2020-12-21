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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/******************************************************************************
 * File Name     : r_riic_stream_it.h
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream it! v2 board
 * Description   : Sample Program - RIIC sample program
 ******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 21.10.2014 1.00
 *******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RIIC_STREAM_IT_H
    #define RIIC_STREAM_IT_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
    #include "r_typedefs.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* FocalTech CTP Controller */
    #define RZA1H_APP_I2C_FT5216    (0x70)

/******************************************************************************
 Global variables
 ******************************************************************************/

/******************************************************************************
 Functions Prototypes
 ******************************************************************************/
int32_t R_RIIC_StreamItWrite (uint32_t channel, uint8_t d_adr, uint16_t w_adr, uint32_t w_byte, uint8_t * w_buffer);

int32_t R_RIIC_StreamItRead (uint32_t channel, uint8_t d_adr, uint16_t r_adr, uint32_t r_byte, uint8_t * r_buffer);

void R_RIIC_StreamItInit (void);

int32_t Sample_RIIC_ScanPort (uint32_t channel);

int32_t R_RIIC_CameraWrite (uint32_t channel, uint8_t d_adr, uint16_t w_adr, uint8_t data);
uint8_t R_RIIC_CameraRead (uint32_t channel, uint8_t d_adr, uint16_t r_adr);

/* RIIC_STREAM_IT_H */
#endif

/* End of File */
