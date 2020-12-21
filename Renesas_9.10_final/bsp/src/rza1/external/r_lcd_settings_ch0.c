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
 * File Name    : r_lcd_settings_ch0.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2
 * Description  : Functions for VDC5 Channel 0 LCD Panel
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 02.08.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include <r_bsp.h>            /* Device Driver common header */
#include "r_lcd_panel.h"


/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* Analog RGB D-sub15 (RGB888), SVGA signal 800x600 */
/* TCON timing setting, VS */
static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_vs =
{ (uint16_t) (0 * 2u), /* Signal pulse start position */
(uint16_t) LCD_DISP_W_VSYNC, /* Pulse width */
VDC5_LCD_TCON_POLMD_NORMAL, VDC5_LCD_TCON_REFSEL_HSYNC, VDC5_SIG_POL_INVERTED, /* Polarity inversion control of signal */
VDC5_LCD_TCON_PIN_4, /* Output pin for LCD driving signal */
VDC5_EDGE_RISING /* Output phase control of signal */
};

/* TCON timing setting, HS */
static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_hs =
{ (uint16_t) 0u, /* Signal pulse start position */
(uint16_t) LCD_DISP_W_HSYNC, /* Pulse width */ // test VDC5 library // BRANDT - QVGA - OK
        VDC5_LCD_TCON_POLMD_NORMAL, VDC5_LCD_TCON_REFSEL_HSYNC, /* Signal operating reference select */
        VDC5_SIG_POL_INVERTED, /* Polarity inversion control of signal */
        VDC5_LCD_TCON_PIN_3, /* Output pin for LCD driving signal */
        VDC5_EDGE_RISING /* Output phase control of signal */
};

/* TCON timing setting, DE */
static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_de_ =
{ (uint16_t) 0, (uint16_t) 0, VDC5_LCD_TCON_POLMD_NORMAL, VDC5_LCD_TCON_REFSEL_HSYNC, VDC5_SIG_POL_NOT_INVERTED,
        VDC5_LCD_TCON_PIN_2, VDC5_EDGE_RISING };

/* TCON timing setting, VE - none */
static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_ve =
{ (uint16_t) ( LCD_CH0_DISP_VS * 2), /* Signal pulse start position */
(uint16_t) ( LCD_CH0_DISP_VW * 2), /* Pulse width */
VDC5_LCD_TCON_POLMD_NORMAL, /* Signal generation mode select */
(uint16_t) 0, /* Signal operating reference select */
(uint16_t) 0, /* Polarity inversion control of signal */
VDC5_LCD_TCON_PIN_NON, /* Output pin for LCD driving signal */
VDC5_EDGE_RISING /* Output phase control of signal */
};

/* TCON timing setting, HE - none */
static const vdc5_lcd_tcon_timing_t lcd_tcon_timing_he =
{ (uint16_t) LCD_CH0_DISP_HS, /* Signal pulse start position */
(uint16_t) LCD_CH0_DISP_HW, /* Pulse width */
VDC5_LCD_TCON_POLMD_NORMAL, /* Signal generation mode select */
(uint16_t) 0, /* Signal operating reference select */
(uint16_t) 0, /* Polarity inversion control of signal */
VDC5_LCD_TCON_PIN_NON, /* Output pin for LCD driving signal */
VDC5_EDGE_RISING /* Output phase control of signal */
};

/*****************************************************************************
 Function Name: R_LCD_SetLcdPanelCh0
 Description:   Low Level hardware setup for LCD connected to CH0
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_LCD_SetLcdPanelCh0 (void)
{
    volatile uint16_t dummy_read;
    volatile uint16_t dummy_read6;
    volatile uint16_t dummy_read7;
    volatile uint16_t dummy_read8;
    //volatile uint16_t dummy_read10;
    //volatile uint16_t dummy_read11;
    uint8_t data[0x2];

    /* Suppresses the 'variable set but not used' warning */
    //UNUSED_VARIABLE(dummy_read);
    //UNUSED_VARIABLE(dummy_read10);
    //UNUSED_VARIABLE(dummy_read11);

    /* Port 8 */
    GPIO.PMC8 &= (uint16_t) ~0x9FFFu;
    dummy_read = GPIO.PMC8;

    /* PFCAE8, PFCE8, PFC8 ... 1st alternative function
     PIPC8, PMC8
     b15:b13  : P8_15 ~ P8_13
     b7:b0    : P8_7 ~ P8_0 */
    GPIO.PFCAE8 &= (uint16_t) ~0x9FFFu;
    GPIO.PFCE8 &= (uint16_t) ~0x9FFFu;
    GPIO.PFC8 &= (uint16_t) ~0x9FFFu;

    GPIO.PIPC8 |= (uint16_t) 0x9FFFu;

    GPIO.PIPC8 &= (uint16_t) ~0x0300u; /* on force Mode et Dith en sortie - 1 */
    GPIO.PM8 |= (uint16_t) 0x0300u; /* on force Mode et Dith en sortie - 1 */

    GPIO.PMC8 |= (uint16_t) 0x9CFFu;

    dummy_read8 = GPIO.PMC8;

    /* Port 6 */
    GPIO.PMC6 &= (uint16_t) ~0x00FFu;
    dummy_read = GPIO.PMC6;

    /* PFCAE6, PFCE6, PFC6 ... 2nd alternative function
     PIPC6, PMC6
     b15:b0   : P6_15 ~ P6_0 */
    GPIO.PFCAE6 &= (uint16_t) ~0x00FFu;
    GPIO.PFCE6 &= (uint16_t) ~0x00FFu;

    GPIO.PFC6 = (uint16_t) 0x00FFu;

    GPIO.PIPC6 |= (uint16_t) 0x00FFu;
    GPIO.PMC6 = (uint16_t) 0x00FFu;

    dummy_read6 = GPIO.PMC6;

    /* Port 7_4 -> LCD0_CLK */
    GPIO.PMC7 &= (uint16_t) ~0x0010u;
    dummy_read = GPIO.PMC7;

    /* PFCAE7, PFCE7, PFC7 ... 6th alternative function
     PIPC7, PMC7
     b15:b0   : P7_15 ~ P7_0 */
    GPIO.PFCAE7 |= (uint16_t) 0x0010u;
    GPIO.PFCE7 &= (uint16_t) ~0x0010u;

    GPIO.PFC7 |= (uint16_t) 0x0010u;

    GPIO.PIPC7 |= (uint16_t) 0x0010u;
    GPIO.PMC7 |= (uint16_t) 0x0010u;

    dummy_read7 = GPIO.PMC7;
}

/*****************************************************************************
 End of function  R_LCD_SetLcdPanelCh0
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_LCD_SetLcdTconSettingsCh0
 Description:   Set timing control parameters in VDC5 for the LCD panel.
 Parameters:    channel - VDC Channel for the settings,
 outctrl - Timing control parameters to set
 Return value:  None
 *****************************************************************************/
void R_LCD_SetLcdTconSettingsCh0 (const vdc5_lcd_tcon_timing_t * * outctrl)
{
    if (0 == outctrl)
    {
        return;
    }
    *outctrl++ = &lcd_tcon_timing_vs; /* STVA/VS: Vsync Not used*/
    *outctrl++ = &lcd_tcon_timing_ve; /* STVB/VE:  */
    *outctrl++ = &lcd_tcon_timing_hs; /* STH/SP/HS: */
    *outctrl++ = &lcd_tcon_timing_he; /* STB/LP/HE: */
    *outctrl++ = NULL; /* CPV/GCK: Not used */
    *outctrl++ = NULL; /* POLA: Not used */
    *outctrl++ = NULL; /* POLB: Not used */
    *outctrl = &lcd_tcon_timing_de_; /* DE: Not used */
}

/*****************************************************************************
 End of function  R_LCD_SetLcdTconSettingsCh0
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
