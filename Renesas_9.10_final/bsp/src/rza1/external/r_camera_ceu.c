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
 * File Name    : r_camera_ceu.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2 board
 * Description  : Top level API interface for the CEU. Abstracts from the low level
 *                driver.
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
#ifdef _CEU_DEBUG_
    #include    <stdio.h>
#endif

#include    <string.h>

#include <r_bsp.h>            /* Device Driver common header */
#include    "r_camera_ceu.h"

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define BIT_00      (0x0001u)
#define BIT_01      (0x0002u)
#define BIT_02      (0x0004u)
#define BIT_03      (0x0008u)
#define BIT_04      (0x0010u)
#define BIT_05      (0x0020u)
#define BIT_06      (0x0040u)
#define BIT_07      (0x0080u)
#define BIT_08      (0x0100u)
#define BIT_09      (0x0200u)
#define BIT_10      (0x0400u)
#define BIT_11      (0x0800u)
#define BIT_12      (0x1000u)
#define BIT_13      (0x2000u)
#define BIT_14      (0x4000u)
#define BIT_15      (0x8000u)

#define BIT_ALLSET  (0xFFFFu)

#define DATA_SHIFT  (0x2u)

#ifdef _CEU_DEBUG_
    #define R_INT_DRV_ERROR ("ERROR!! R_INT_DRV\n\n")
    #define R_CEU_DRV_ERROR ("ERROR!! R_CEU_DRV\n\n")
#endif

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void ceu_complete_callback (const uint32_t interrupt_flag);
static void ceu_init_func (void);
static void ceu_port_init (void);
static void ceu_stop_func (void);

static volatile uint32_t ceu_complete_flag; /** operation complete flag */
static volatile uint32_t ceu_error_flag; /** error flag */

/*****************************************************************************
 Function Name: R_CAM_CeuInit
 Description:   CEU module initialization.
 In this function, the operations are performed as below.
 - Initialization of a port.
 - Initialization of module (module clock on).
 - The configuration of a CEU module.
 - Initialization of an interrupt controller.
 - Registration of an interruption processing function.
 - CEU interrupt enable.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_CAM_CeuInit (void)
{
    int32_t ret_code;

    /* initialize port and module */
    R_CEU_ModuleInit(&ceu_port_init);

    /* CEU registers initialize for data enable capture mode. all interrupt is disabled. */

    CEU_ImageCaptureInit();

    /* ISR(Interrupt Service routine) registration */
    ret_code = R_INTC_RegistIntFunc(INTC_ID_CEUI, &CEU_Isr);
    if (DEVDRV_SUCCESS != ret_code)
    {
#ifdef _CEU_DEBUG_
        (void)printf( R_INT_DRV_ERROR );
#else
        /* Do nothing */
#endif

    }

    if ( DEVDRV_SUCCESS == ret_code)
    {
        /* set priority */
        ret_code = R_INTC_SetPriority(INTC_ID_CEUI, (uint8_t) CEU_INTERRUPT_PRIORITY);
        if (DEVDRV_SUCCESS != ret_code)
        {
#ifdef CEU_DEBUG
            (void)printf( R_INT_DRV_ERROR );
#else
            /* Do nothing */
#endif

        }
    }

    if ( DEVDRV_SUCCESS == ret_code)
    {
        /* interrupt enable */
        ret_code = R_INTC_Enable(INTC_ID_CEUI);
        if (DEVDRV_SUCCESS != ret_code)
        {
#ifdef _CEU_DEBUG_
            (void)printf( R_INT_DRV_ERROR );
#else
            /* Do nothing */
#endif
        }
    }

    if ( DEVDRV_SUCCESS == ret_code)
    {
        /* CEU interrupt callback func set */
        R_CEU_CallbackISR(&ceu_complete_callback);

#ifdef CEU_ENABLE_INTS
        /* interrupt enable for Data enable capture mode */
        R_CEU_InterruptEnable( CEU_INT_ONE_FRAME_CAPTUER_END );
#endif
    }

}

/*****************************************************************************
 End of function  R_CAM_CeuInit
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_CAM_CeuSingleCapTerminate
 Description:    CEU module termination.
 In this function, the operations are performed as below.
 - CEU interrupt disable.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_CAM_CeuSingleCapTerminate (void)
{
    R_CEU_InterruptDisable();
}

/*****************************************************************************
 End of function  R_CAM_CeuSingleCapTerminate
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_CAM_CeuSingleCapture
 Description:    CEU Single Capture Start.
 In this function, the operations are performed as below.
 - Initialization of a variable.
 - Start capture.
 - Wait for capture end.
 Parameters:    pCapBuff Pointer of capture buffer.
 Return value:  None
 *****************************************************************************/
void R_CAM_CeuSingleCapture (const uint16_t * const pCapBuff)
{
    ceu_error_t ceu_error;

    /* Initialization of a variable */
    ceu_complete_flag = 0;
    ceu_error_flag = 0;

    /* Destination memory address setting */
    ceu_error = R_CEU_SetMemoryAddress((uint32_t) pCapBuff);
    if (ceu_error == CEU_ERROR_PARAM)
    {
#ifdef _CEU_DEBUG_
        (void)printf( R_CEU_DRV_ERROR );
#else
        /* Do nothing */
#endif
    }

    /* CEU Capture start */
    R_CEU_CaptureStart();

    /* wait capture operation complete */
    while ( CAPTURE_END != ceu_complete_flag)
    {
        /* wait */
    }

    if (0u != ceu_error_flag)
    {
#ifdef _CEU_DEBUG_
        (void)printf( R_CEU_DRV_ERROR );
#else
        /* Do nothing */
#endif
    }

    return;

}

/*****************************************************************************
 End of function  R_CAM_CeuSingleCapture
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: ceu_complete_callback
 Description:   CEU complete callback
 The callback function for the completion of capture processing.
 Parameters:    Interrupt_flag CETCR register value.
 Return value:  None
 *****************************************************************************/
static void ceu_complete_callback (const uint32_t interrupt_flag)
{
    ceu_complete_flag = CAPTURE_END;

    if ((CEU_INT_ONE_FRAME_CAPTUER_END & interrupt_flag) != 0u)
    {
        ceu_error_flag = 0u;
    }
    else
    {
        /* sample program is not supporting this interrupt factor. */
        ceu_error_flag = interrupt_flag;
    }
    return;

} /* End of function ceu_complete_callback() */

/*****************************************************************************
 End of function  ceu_complete_callback
 ******************************************************************************/

/*****************************************************************************
 Function Name: ceu_port_init
 Description:   CEU port setting initialization
 Port initialize for CEU.
 CMOS digital image sensor (OV7670) is 8bit bus width.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void ceu_port_init (void)
{
    uint32_t reg_data;

    /* Port settings as follows.
     Pin    Port    Function    No function
     153    P1_4    VIO_D0        4
     154    P1_5    VIO_D1        4
     155    P1_6    VIO_D2        4
     156    P1_7    VIO_D3        4
     98    P1_12    VIO_D4        4
     99    P1_13    VIO_D5        4
     100    P1_14    VIO_D6        4
     101    P1_15    VIO_D7        4

     157    P3_15    VIO_FLD        2
     189    P4_1    VIO_HD        2
     188    P4_0    VIO_VD        2
     148    P3_14    VIO_CLK        2

     160 P2.8     CAM_PWRDN    1    -> Input port
     161 P2.9     CAM_RST        1    -> Input port   */

    GPIO.P2 &= (uint16_t) ~( BIT_08 | BIT_09); /* P2.8=Low, P2.9=Low */

    /* Port IP Control register (PIPC) PIPCnx : b'1 (SW input/output control) */
    GPIO.PIPC1 |= (uint16_t) (((((((BIT_15 | BIT_14) | BIT_13) | BIT_12) | BIT_07) | BIT_06) | BIT_05) | BIT_04);
    GPIO.PIPC2 |= (uint16_t) ( BIT_09 | BIT_08);
    GPIO.PIPC3 |= (uint16_t) (BIT_14);
    GPIO.PIPC4 |= (uint16_t) (BIT_01 | BIT_00);

    /* Port Mode register   PMnx : b=0 (output) PMnx=1 (input) */
    GPIO.PM1 |= (uint16_t) (((((((BIT_15 | BIT_14) | BIT_13) | BIT_12) | BIT_07) | BIT_06) | BIT_05) | BIT_04);
    GPIO.PM2 |= (uint16_t) ( BIT_09 | BIT_08);
    GPIO.PM3 |= (uint16_t) (BIT_14);
    GPIO.PM4 |= (uint16_t) (BIT_01 | BIT_00);

    /* Port Mode Control register PMCnx : b=0 (gpio mode) PMCnx : b=1 (function mode) */
    GPIO.PMC1 |= (uint16_t) (((((((BIT_15 | BIT_14) | BIT_13) | BIT_12) | BIT_07) | BIT_06) | BIT_05) | BIT_04);
    GPIO.PMC2 &= (uint16_t) ~( BIT_09 | BIT_08); //Force to port mode
    GPIO.PMC3 |= (uint16_t) (BIT_14);
    GPIO.PMC4 |= (uint16_t) (BIT_01 | BIT_00);

    /* set "Function6 (input)"
     * register setting is as follows.
     * PFCAE    PFCE   PFC    PM
     *  0        0      0      1  : Function1 (input)    <--- SET for P2
     *  0        0      1      1  : Function2 (input)   <--- SET for P3 & P4
     *  0        1      1      1  : Function4 (input)   <--- SET for P1         */

    /* Port Function Control register (PFC) PFCn15 - PFCn0 : b=1 (function 4 input) */
    GPIO.PFC1 |= (uint16_t) (((((((BIT_15 | BIT_14) | BIT_13) | BIT_12) | BIT_07) | BIT_06) | BIT_05) | BIT_04);

    /* Port Function Control register (PFC) PFCn15 - PFCn0 : b=1 (function 2 input) */
    GPIO.PFC3 |= (uint16_t) (BIT_14);

    /* Port Function Control register (PFC) PFCn15 - PFCn0 : b=1 (function 2 input) */
    GPIO.PFC4 |= (uint16_t) ( BIT_01 | BIT_00);

    /* Port Function Control Extend register (PFCE) PFCEn15 - PFCEn0 : b=1 (function 4 input) */
    GPIO.PFCE1 |= (uint16_t) (((((((BIT_15 | BIT_14) | BIT_13) | BIT_12) | BIT_07) | BIT_06) | BIT_05) | BIT_04);

    /* Port Function Control Extend register (PFCE) PFCEn15 - PFCEn0 : b=0 (function 2 input) */
    GPIO.PFCE3 &= (uint16_t) ~(BIT_14);

    /* Port Function Control Extend register (PFCE) PFCEn15 - PFCEn0 : b=0 (function 2 input) */
    GPIO.PFCE4 &= (uint16_t) ~( BIT_01 | BIT_00);

    /* Port Function Control Additional Extend register (PFCAE) PFCAEn15 - PFCAEn0 : b=0 (function 2 input) */
    GPIO.PFCAE1 &= (uint16_t) ~(((((((BIT_15 | BIT_14) | BIT_13) | BIT_12) | BIT_07) | BIT_06) | BIT_05) | BIT_04);

    /* Port Function Control Additional Extend register (PFCAE) PFCAEn15 - PFCAEn0 : b=0 (function 2 input) */
    GPIO.PFCAE3 &= (uint16_t) ~(BIT_14);

    /* Port Function Control Additional Extend register (PFCAE) PFCAEn15 - PFCAEn0 : b=0 (function 2 input) */
    GPIO.PFCAE4 &= (uint16_t) ~( BIT_01 | BIT_00);

    /* Port bi-directionally control register (PBDC) PBDCn15 : b=0 (disable)  PBDCn14 : b=1 (enable)  */
    GPIO.PBDC1 |= (uint16_t) (((((((BIT_15 | BIT_14) | BIT_13) | BIT_12) | BIT_07) | BIT_06) | BIT_05) | BIT_04);
    GPIO.PBDC2 |= (uint16_t) ( BIT_09 | BIT_08); //Force to port mode
    GPIO.PBDC3 |= (uint16_t) (BIT_14);
    GPIO.PBDC4 |= (uint16_t) (BIT_01 | BIT_00);

    return;
}

/*****************************************************************************
 End of function  ceu_port_init
 ******************************************************************************/

/*****************************************************************************
 Function Name: ceu_stop_func
 Description:   Stops the CEU Module
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void ceu_stop_func (void)
{
    CEU.CAPSR |= 0x00;
}

/*****************************************************************************
 End of function  ceu_stop_func
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
