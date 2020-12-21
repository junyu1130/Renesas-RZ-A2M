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
* @file         r_ceu.h
* @version      0.04
* $Rev: 1739 $
* $Date:: 2016-08-01 17:51:18 +0100#$
* @brief        CEU sample program header
******************************************************************************/

#ifndef R_CEU_H
#define R_CEU_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
/* Capture Control Register (CAPCR) 
 * MTCM[1:0] - unit for transferring data to a bus bridge module.
 */
typedef enum
{
    CEU_BURST_MODE_X1 = (0u),  /** Transferred to the bus in 4-burst 1-transfer(4QWx1) units */
    CEU_BURST_MODE_X2 = (1u),  /** Transferred to the bus in 4-burst 1-transfer(4QWx2) units */
    CEU_BURST_MODE_X4 = (2u),  /** Transferred to the bus in 4-burst 1-transfer(4QWx4) units */
    CEU_BURST_MODE_X8 = (3u)   /** Transferred to the bus in 4-burst 1-transfer(4QWx8) units */
} ceu_transfer_mode_t;

/* Capture Interface Control Register (CAMCR)
 * signal polarity
 */
typedef enum
{
    CEU_SIGNAL_HIGH_ACTIVE  = (0u),       /** signal is high-active */
    CEU_SIGNAL_LOW_ACTIVE   = (1u)        /** signal is low-active */
} ceu_polarity_t;

/* Capture Data Output Control Register (CDOCR)
 * COLS,COWS,COBS - Controls swapping in 32/16/8-bit units for data output from the CEU.
 */
typedef enum
{
    CEU_DATAOUT_SWAP_DISABLE    = (0u),   /** Data is not swapped */
    CEU_DATAOUT_SWAP_ENABLE     = (1u)    /** Data is swapped */
} ceu_dataout_swap_enable_t;

/* Capture Event Interrupt Enable Register (CEIER)
 */
typedef enum
{
    CEU_INT_ONE_FRAME_CAPTUER_END           = (0x00000001u),   /* One Frame caputer end interrupt */
    CEU_INT_ONE_FIELD_CAPTUER_END           = (0x00000002u),   /* One Filed caputer end interrupt */
    CEU_INT_ILLEGAL_REGISTER_WRITE          = (0x00000010u),   /* illegal register write interrupt */
    CEU_INT_DETECT_HD                       = (0x00000100u),   /* HD interrupt */
    CEU_INT_DETECT_VD                       = (0x00000200u),   /* VD interrupt */
    CEU_INT_BUNDLE_W_COMP_TOP1              = (0x00001000u),   /* bundle write complete interrupt (TOP field) */
    CEU_INT_BUNDLE_W_COMP_TOP2              = (0x00002000u),   /* bundle write complete interrupt (TOP field2) */
    CEU_INT_BUNDLE_W_COMP_BOTOM1            = (0x00004000u),   /* bundle write complete interrupt (Bottom field) */
    CEU_INT_BUNDLE_W_COMP_BOTOM2            = (0x00008000u),   /* bundle write complete interrupt (Bottom field2) */
    CEU_INT_CRAM_OVERFLOW                   = (0x00010000u),   /* CRAM data overflow interrupt */
    CEU_INT_ILLEGAL_HSYNC                   = (0x00020000u),   /* illegal H-Sync interrupt */
    CEU_INT_ILLEGAL_VSYNC                   = (0x00040000u),   /* illegal V-Sync interrupt */
    CEU_INT_INSUFFICIENT_VD                 = (0x00100000u),   /* insufficient verticalsync front porch interrupt */
    CEU_INT_FW_FAILED                       = (0x00800000u),   /* FW FAILED interrupt */
    CEU_INT_NON_HD                          = (0x01000000u),   /* Non-HD Interrupt */
    CEU_INT_NON_VD                          = (0x02000000u)    /* Non-VD Interrupt */
} ceu_interrupt_event_t;

/* Return value */
typedef enum
{
    CEU_ERROR_OK        = (0),           /** success */
    CEU_ERROR_PARAM     = (-1)           /** parameter error */
} ceu_error_t;

typedef enum {
    R_CEU_YUV_PLANAR,
    R_CEU_RAW
} ceu_mode_t;

typedef struct {
    ceu_mode_t MODE;
    uint32_t CAMCR;
    uint32_t CDOCR_A;

    uint16_t USEFUL_WIDTH;
    uint16_t USEFUL_HEIGHT;

    uint16_t LINE_WIDTH;
} ceu_config_t;



/******************************************************************************
Macro definitions
******************************************************************************/
#define SAFETY 10
#define HL_OFFSET 0 /* OV7670 left side has 7 pixels black * 2 data (Y, UV) */
#define HR_OFFSET 0 /* OV7670 right side has 6 pixels black * 2 data (Y, UV) */

#define CAMOR_A_INIT (0x00000000|(HL_OFFSET&0x1FFF)) /* Horizontal Offset*/


/******************************************************************************
Variable Externs
******************************************************************************/
extern ceu_config_t R_CEU_Config;


/******************************************************************************
Functions Prototypes
******************************************************************************/
void R_CEU_ModuleInit( void (* const init_func)(void) );
void R_CEU_ConfigInit(void);
void PekoeCeuPortInit(void);
ceu_error_t R_CEU_SetMemoryAddress( const uint32_t addr );
void R_CEU_CallbackISR( void (* const callback)(uint32_t interrupt_flag ) );
void R_CEU_CaptureStart( void );
void R_CEU_InterruptEnable( const uint32_t flag );
void R_CEU_InterruptDisable( void );
void CEU_Isr ( const uint32_t int_sense );
ceu_error_t CEU_CaptureOneFrame(void *Frame);
void CEU_ImageCaptureInit(void);

#endif /* R_CEU_H */
