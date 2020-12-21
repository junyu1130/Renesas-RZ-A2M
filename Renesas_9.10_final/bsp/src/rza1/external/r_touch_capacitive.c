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
 * File Name    : r_touch_capacitive.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2 board
 * Description  : Handles  communication with the capacitive controllers
 *               for the three screens of the Stream It board
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.08.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <r_bsp.h>            /* Device Driver common header */
#include "r_touch_capacitive.h"


/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
//static volatile int32_t touch_read_request = 0;
volatile int32_t g_touch_read_request = 0;

static uint8_t touch_get_points (uint8_t * data);

/* I2C related functions */
static void int_irq_touch_panel (uint32_t int_sense);

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/
extern uint16_t g_touch_Xpos;
extern uint16_t g_touch_Ypos;
extern uint8_t g_touch_Touch;
extern uint16_t g_touch_MultiPoint[5][2];

/******************************************************************************
 Public Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_TOUCH_InitCapacitive
 Description:   Initialises the capacitive touch screen system and controller
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_TOUCH_InitCapacitive (void)
{
    /* Set TOUCH (IRQ3) as input pin (P1_11) */
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC111_SHIFT, GPIO_PBDC1_PBDC111);
    RZA_IO_RegWrite_16(&GPIO.PM1, 1, GPIO_PM1_PM111_SHIFT, GPIO_PM1_PM111);
    RZA_IO_RegWrite_16(&GPIO.P1, 1, GPIO_P1_P111_SHIFT, GPIO_P1_P111);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 1, GPIO_PMC1_PMC111_SHIFT, GPIO_PMC1_PMC111);

    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE111_SHIFT, GPIO_PFCAE1_PFCAE111);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE111_SHIFT, GPIO_PFCE1_PFCE111);
    RZA_IO_RegWrite_16(&GPIO.PFC1, 1, GPIO_PFC1_PFC111_SHIFT, GPIO_PFC1_PFC111);

    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC111_SHIFT, GPIO_PIPC1_PIPC111);

    /* Configure and enable IRQ3 interrupts received from the touch panel */
    R_INTC_Disable(INTC_ID_IRQ3);
    R_INTC_SetPriority(INTC_ID_IRQ3, 10uL);

    /* Assign the IRQ3 function */
    R_INTC_RegistIntFunc(INTC_ID_IRQ3, &int_irq_touch_panel);

    /* Set the interrupt as rising edge trigger */
    RZA_IO_RegWrite_16(&INTCICR1, 1, INTC_ICR1_IRQ30S_SHIFT, INTC_ICR1_IRQ31S);

    R_INTC_Enable(INTC_ID_IRQ3);
}
/*****************************************************************************
 End of function R_TOUCH_InitCapacitive
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_TOUCH_RefreshCapPosition
 Description:   Refreshes the global touch position variables.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_TOUCH_RefreshCapPosition (void)
{
    static uint8_t debounce = 0;
    uint8_t data[0x20];
    uint8_t p;
    int32_t ret;
    g_touch_Touch = 0;

    if (debounce)
    {
        debounce--;
    }

    if (g_touch_read_request > 0)
    {
        /* no more touch points until next interrupt */
        g_touch_read_request = 0;
        ret = touch_get_points(data);
        if (DEVDRV_SUCCESS == ret)
        {
            if (data[2] > 0)
            {
                g_touch_Xpos = (((data[3] & 0x7) << 8) + data[4]);
                g_touch_Ypos = (((data[5] & 0x7) << 8) + data[6]);
                g_touch_MultiPoint[0][0] = (g_touch_Xpos);
                g_touch_MultiPoint[0][1] = (g_touch_Ypos);
                g_touch_Touch = 1;
                debounce = 0;
            }

            for (p = 1; p < data[2]; p++)
            {
                g_touch_MultiPoint[p][0] = (((data[3 + (p * 6)] & 0x0F) << 8) | data[4 + (p * 6)]);
                g_touch_MultiPoint[p][1] = (((data[5 + (p * 6)] & 0x0F) << 8) | data[6 + (p * 6)]);
            }

            for (p = data[2]; p < 5; p++)
            {
                g_touch_MultiPoint[p][0] = 9999;
                g_touch_MultiPoint[p][1] = 9999;
            }
        }

    }
    else if (0 == debounce)
    {
        g_touch_Xpos = 9999;
        g_touch_Ypos = 9999;
        for (p = 0; p < 5; p++)
        {
            g_touch_MultiPoint[p][0] = 9999;
            g_touch_MultiPoint[p][1] = 9999;
        }
    }
    else
    {
        /* Do nothing */
    }
}
/*****************************************************************************
 End of function R_TOUCH_RefreshCapPosition
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: touch_get_points
 Description:   Get touch points from IIC touch screen controller.
 Parameters:    Pointer uint8_t array for touch point data.
 Return value:  0 - No error; 1 = IIC communications error
 *****************************************************************************/
static uint8_t touch_get_points (uint8_t * data)
{
    uint8_t error;

    error = R_RIIC_StreamItRead(RIIC_CH_1, RZA1H_APP_I2C_FT5216, 0, 32, data);

    /* Return status code */
    return error;
}
/*****************************************************************************
 End of function touch_get_points
 ******************************************************************************/

/*****************************************************************************
 Function Name: int_irq_touch_panel
 Description:   Interrupt service routine for Touch screen controller
 Parameters:    int_sense - unused.
 Return value:  None
 *****************************************************************************/
static void int_irq_touch_panel (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    g_touch_read_request++;

    /* Clear IRQ flag */
    if (1 == RZA_IO_RegRead_16(&(INTCIRQRR),
    INTC_IRQRR_IRQ3F_SHIFT, (0x01 << INTC_IRQRR_IRQ3F_SHIFT)))
    {
        RZA_IO_RegWrite_16(&INTCIRQRR, 0,
        INTC_IRQRR_IRQ3F_SHIFT, (0x01 << INTC_IRQRR_IRQ3F_SHIFT));
    }
}
/*****************************************************************************
 End of function int_irq_touch_panel
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

