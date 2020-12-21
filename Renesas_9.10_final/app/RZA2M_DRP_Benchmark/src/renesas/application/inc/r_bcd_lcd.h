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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
#ifndef R_BCD_LCD_H
#define R_BCD_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include "draw.h"
#include "r_bcd_main.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_BCD_LCD_WIDTH     (960u) //(640u) //(800u) // (1280u)   // XXX 1280-640
#define R_BCD_LCD_HEIGHT    (720u) //(480u) //(600u) // (720u)   // XXX 720-480
//#define R_BCD_LCD_WIDTH     (640u) //(640u) //(800u) // (1280u)   // XXX 1280-640
//#define R_BCD_LCD_HEIGHT    (480u) //(480u) //(600u) // (720u)   // XXX 720-480

#define R_BCD_LCD_STRIDE    (32u)

#define     LAYER0_DISP_AREA_HS         (0u)
#define     LAYER0_DISP_AREA_VS         (0u)
#define     LAYER0_DISP_AREA_HW         (R_BCD_LCD_WIDTH)
#define     LAYER0_DISP_AREA_VW         (R_BCD_LCD_HEIGHT)

#define     VIDEO_BUFFER_STRIDE         ((LAYER0_DISP_AREA_HW + (R_BCD_LCD_STRIDE - 1)) & ~(R_BCD_LCD_STRIDE - 1))
#define     VIDEO_BUFFER_HEIGHT         (LAYER0_DISP_AREA_VW)
#define     VIDEO_BUFFER_NUM            (1u)

#define     LAYER2_DISP_AREA_HS         (0u)
#define     LAYER2_DISP_AREA_VS         (0u)                // XXX 0->528 overlap in lower-half display
#define     LAYER2_DISP_AREA_HW         (R_BCD_LCD_WIDTH)
#define     LAYER2_DISP_AREA_VW         (R_BCD_LCD_HEIGHT)

#define     GRAPHICS2_BUFFER_STRIDE     (((LAYER2_DISP_AREA_HW / 2) + (R_BCD_LCD_STRIDE - 1)) & ~(R_BCD_LCD_STRIDE - 1))
#define     GRAPHICS2_BUFFER_HEIGHT     (LAYER2_DISP_AREA_VW)
#define     GRAPHICS2_BUFFER_NUM        (1u)

/******************************************************************************
Functions Prototypes
******************************************************************************/
void R_BCD_LcdVSync(void);
int32_t R_BCD_LcdInit(void);
uint8_t *R_BCD_LcdGetVramAddress(void);
void R_BCD_LcdSwapVideoBuffer( void );
void R_BCD_LcdSwapGraphicsBuffer(void);
void R_BCD_LcdWriteChar(uint8_t code, uint32_t x, uint32_t y, uint8_t color);
void R_BCD_LcdWriteString(uint8_t *pcode, uint32_t x, uint32_t y, uint8_t color);
void R_BCD_LcdSetLightness(uint32_t lightness);
void R_BCD_LcdWriteLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t color);
uint8_t *R_BCD_LcdGetVramAddress(void);
uint8_t *R_BCD_LcdGetOLVramAddress(void);
void R_BCD_LcdClearGraphicsBuffer(void);

#ifdef __cplusplus
}
#endif

#endif  /* R_BCD_LCD_H */

/* End of File */
