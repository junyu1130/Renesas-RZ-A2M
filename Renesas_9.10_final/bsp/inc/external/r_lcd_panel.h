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
 * File Name    : r_lcd_panel.h
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2
 * Description  : LCD panel definition header
 *                Contains panel specific timing parameters.
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 02.08.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

#ifndef LCD_PANEL_H
    #define LCD_PANEL_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
    #include    <stdlib.h>
    #include    <stdio.h>

    #include "r_typedefs.h"


    #define LOGO_16_BIT_PER_PIXEL

/* Select the TFT to drive */
/* #define        TFT_RSKRZA1H_GWP0700CNWV04_SVGA            (1) */
/* #define        TFT_EASTRISING_ER_TFT028_4_QVGA            (1) */
    #define        TFT_EASTRISING_ER_TFT043_3                    (1)

    #ifdef TFT_RSKRZA1H_GWP0700CNWV04_SVGA
        #define     LCD_CH0_DISP_HS         (120u)                        /* LCD display area size, horizontal start position */
        #define     LCD_CH0_DISP_HW         (800u)                        /* LCD display area size, horizontal width */
        #define     LCD_CH0_DISP_VS         (24u)                       /* LCD display area size, vertical start position */
        #define     LCD_CH0_DISP_VW         (480u)                      /* LCD display area size, height (vertical width) */

        #define     LCD_DISP_S_HSYNC        (868u)
        #define     LCD_DISP_W_HSYNC        (106u)

        #define     LCD_DISP_S_VSYNC        (0u)
        #define     LCD_DISP_W_VSYNC        (8u)

        #define     LCD_CH0_SIG_FV          (631u - 1u)                 /* Free-running Vsync period */
        #define     LCD_CH0_SIG_FH          (1000u - 1u)                /* Hsync period */

        #define     LCD_CH0_TCON_HALF       (LCD_CH0_SIG_FH / 2u)       /* TCON reference timing, 1/2fH timing */
        #define     LCD_CH0_TCON_OFFSET     (0u)                        /* TCON reference timing, offset Hsync signal timing */

        #define     LCD_CH0_PANEL_CLK       (VDC5_PANEL_ICKSEL_PERI)      /* Panel clock select */
        #define     LCD_CH0_PANEL_CLK_DIV   (VDC5_PANEL_CLKDIV_1_2)       /* Panel clock frequency division ratio */
        #define     LCD_CH0_OUT_EDGE        (VDC5_EDGE_RISING)            /* Output phase control of LCD_DATA[23:0] signal */

    #endif

    #ifdef TFT_EASTRISING_ER_TFT043_3
        #define     LCD_CH0_DISP_HS         (88u)           /* LCD display area size, horizontal start position */
        #define     LCD_CH0_DISP_HW         (480u)            /* LCD display area size, horizontal width */
        #define     LCD_DISP_W_HSYNC        (41u)            /* QVGA - OK 5 */
        #define     LCD_DISP_W_VSYNC        (10u)            /* QVGA - OK 3 * 2 */
        #define     LCD_CH0_DISP_VS         (12u)           /* LCD display area size, vertical start position */
        #define     LCD_CH0_DISP_VW         (272u)          /* LCD display area size, height (vertical width) */
        #define     LCD_CH0_SIG_FV          (287u - 1u)     /* Free-running Vsync period */
        #define     LCD_CH0_SIG_FH          (576u - 1u)     /* Hsync period */
        #define     LCD_CH0_TCON_HALF       (LCD_CH0_SIG_FH / 2u)       /* TCON reference timing, 1/2fH timing */
        #define     LCD_CH0_TCON_OFFSET     (0u)                        /* TCON reference timing, offset Hsync signal timing */
        #define     LCD_CH0_PANEL_CLK       (VDC5_PANEL_ICKSEL_PERI)      /* Panel clock select */
        #define     LCD_CH0_PANEL_CLK_DIV   (VDC5_PANEL_CLKDIV_1_8)       /* Panel clock frequency division ratio */
        #define     LCD_CH0_OUT_EDGE        (VDC5_EDGE_RISING)            /* Output phase control of LCD_DATA[23:0] signal */
    #endif

    #ifdef TFT_EASTRISING_ER_TFT028_4_QVGA
        #define     LCD_CH0_DISP_HS         (20u)           /* LCD display area size, horizontal start position */
        #define     LCD_CH0_DISP_HW         (240u)            /* LCD display area size, horizontal width */
        #define     LCD_DISP_W_HSYNC        (10u)            /* QVGA - OK 5 */
        #define     LCD_DISP_W_VSYNC        (2u)            /* QVGA - OK 3 * 2 */
        #define     LCD_CH0_DISP_VS         (4u)               /* LCD display area size, vertical start position */
        #define     LCD_CH0_DISP_VW         (320u)          /* LCD display area size, height (vertical width) */
        #define     LCD_CH0_SIG_FV          (330u - 1u)        /* Free-running Vsync period */
        #define     LCD_CH0_SIG_FH          (408u - 1u)     /* Hsync period */
        #define     LCD_CH0_TCON_HALF       (LCD_CH0_SIG_FH / 2u)       /* TCON reference timing, 1/2fH timing */
        #define     LCD_CH0_TCON_OFFSET     (5u)                        /* TCON reference timing, offset Hsync signal timing */
        #define     LCD_CH0_PANEL_CLK       (VDC5_PANEL_ICKSEL_PERI)      /* Panel clock select */
        #define     LCD_CH0_PANEL_CLK_DIV   (VDC5_PANEL_CLKDIV_1_9)       /* Panel clock frequency division ratio */
        #define     LCD_CH0_OUT_EDGE        (VDC5_EDGE_FALLING)            /* Output phase control of LCD_DATA[23:0] signal */
    #endif

    #ifdef LOGO_16_BIT_PER_PIXEL
        #define     LCD_CH0_OUT_FORMAT      (VDC5_LCD_OUTFORMAT_RGB565)   /* LCD output format select */
    #endif
    #ifdef LOGO_24_BIT_PER_PIXEL
        #define     LCD_CH0_OUT_FORMAT      (VDC5_LCD_OUTFORMAT_RGB888)   /* LCD output format select */
    #endif

/* VDC5_CHANNEL_0 or VDC5_CHANNEL_1 */
    #define     GRAPHICS_CHANNEL    (VDC5_CHANNEL_0)
/*! Graphics format: RGB565, 2 bytes per pixel */
    #ifdef LOGO_16_BIT_PER_PIXEL
        #define     GRAPHICS_FORMAT             (VDC5_GR_FORMAT_RGB565)
    #endif
    #ifdef LOGO_24_BIT_PER_PIXEL
        #define     GRAPHICS_FORMAT             (VDC5_GR_FORMAT_ARGB888)
    #endif
/*! Frame buffer stride: Frame buffer stride should be set to a multiple of 32 or 128
 in accordance with the frame buffer burst transfer mode. */
    #ifdef LOGO_16_BIT_PER_PIXEL
        #define     FRAME_BUFFER_STRIDE         (((LCD_CH0_DISP_HW * 2u) + 31u) & ~31u)
    #endif
    #ifdef LOGO_24_BIT_PER_PIXEL
        #define     FRAME_BUFFER_STRIDE         (((LCD_CH0_DISP_HW * 4u) + 31u) & ~31u)
    #endif

    #define     FRAME_BUFFER_HEIGHT         (LCD_CH0_DISP_VW)
    #define     VSYNC_WAIT_COUNT            (2)

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
/* Functions in r_lcd_settings.c */
void R_LCD_SetLcdPanel (vdc5_channel_t channel);
void R_LCD_SetLcdTconSettings (vdc5_channel_t channel, const vdc5_lcd_tcon_timing_t * * outctrl);

/* Functions in r_lcd_settings_ch0.c */
void R_LCD_SetLcdPanelCh0 (void);
void R_LCD_SetLcdTconSettingsCh0 (const vdc5_lcd_tcon_timing_t * * outctrl);

#endif  /* LCD_PANEL_H */
