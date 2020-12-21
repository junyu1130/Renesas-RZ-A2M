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
 *******************************************************************************/
/*******************************************************************************
 * File Name     : r_riic_stream_it.c
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream it! v2 board
 * Description   : Stream IT RI2C Access sample
 *               :
 *               : DEVDRV_CH_1 RSK TFT APP BOARD YROK77210C000BE devices
 *               :  Capacitive touch panel controller
 *               :  PanelBus~(TM) flat panel display
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 21.10.2014 1.00
 *******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 *******************************************************************************/
/* System headers */
#include <stdio.h>
#include <string.h>

#include "dev_drv.h"            /* Device driver header */
#include "../../intc/inc/devdrv_intc.h"
#include "iodefine.h"            /* I/O Register root header */
#include "rza_io_regrw.h"        /* Low level register read/write header */
#include "r_typedefs.h"            /* Default type definition header */
#include "../inc/r_riic_header.h"        /* RIIC Driver Header */
#include "../inc/r_riic_stream_it.h"   /* RIIC Driver Header */

/*******************************************************************************
 Macro definitions
 *******************************************************************************/
/* IIC data write code */
#define SAMPLE_RIIC_RWCODE_W        (0)
/* IIC data read code  */
#define SAMPLE_RIIC_RWCODE_R        (1)
/* Sampling loop */
#define DELAY_TIME_1MS              (260000uL)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static void r_riic_init_ch0 (void);
static void r_riic_init_ch1 (void);

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/
uint8_t g_w_byte;
uint8_t g_config_io;
uint8_t g_r_buffer = 0u;
uint8_t g_port_x_state = 0u;

//int32_t g_touch_read_request = 0;

/******************************************************************************
 * Function Name: delay_1ms_stream_it
 * Description  : Make the sampling interval
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
static void delay_1ms_stream_it (void)
{
    uint32_t delay_count = 0u;

    while (DELAY_TIME_1MS >= delay_count)
    {
        delay_count++;
    }
}
/*******************************************************************************
 End of function delay_1ms_stream_it
 *******************************************************************************/

/******************************************************************************
 * Function Name: riic0_port_init
 * Description  : Initialises channel 0 i2c port register for this board
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
static void riic0_port_init (void)
{
    /* ---- P1_0 : SCL0 ---- */
    /* Port initialise */
    RZA_IO_RegWrite_16(&GPIO.PIBC1, 0, GPIO_PIBC1_PIBC10_SHIFT, GPIO_PIBC1_PIBC10);
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC10_SHIFT, GPIO_PBDC1_PBDC10);
    RZA_IO_RegWrite_16(&GPIO.PM1, 1, GPIO_PM1_PM10_SHIFT, GPIO_PM1_PM10);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 0, GPIO_PMC1_PMC10_SHIFT, GPIO_PMC1_PMC10);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC10_SHIFT, GPIO_PIPC1_PIPC10);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC10_SHIFT, GPIO_PBDC1_PBDC10);
    RZA_IO_RegWrite_16(&GPIO.PFC1, 0, GPIO_PFC1_PFC10_SHIFT, GPIO_PFC1_PFC10);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE10_SHIFT, GPIO_PFCE1_PFCE10);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE10_SHIFT, GPIO_PFCAE1_PFCAE10);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC10_SHIFT, GPIO_PIPC1_PIPC10);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 1, GPIO_PMC1_PMC10_SHIFT, GPIO_PMC1_PMC10);

    /* ---- P1_1 : SDA0 ---- */
    /* Port initialise */
    RZA_IO_RegWrite_16(&GPIO.PIBC1, 0, GPIO_PIBC1_PIBC11_SHIFT, GPIO_PIBC1_PIBC11);
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC11_SHIFT, GPIO_PBDC1_PBDC11);
    RZA_IO_RegWrite_16(&GPIO.PM1, 1, GPIO_PM1_PM11_SHIFT, GPIO_PM1_PM11);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 0, GPIO_PMC1_PMC11_SHIFT, GPIO_PMC1_PMC11);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC11_SHIFT, GPIO_PIPC1_PIPC11);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC11_SHIFT, GPIO_PBDC1_PBDC11);
    RZA_IO_RegWrite_16(&GPIO.PFC1, 0, GPIO_PFC1_PFC11_SHIFT, GPIO_PFC1_PFC11);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE11_SHIFT, GPIO_PFCE1_PFCE11);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE11_SHIFT, GPIO_PFCAE1_PFCAE11);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC11_SHIFT, GPIO_PIPC1_PIPC11);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 1, GPIO_PMC1_PMC11_SHIFT, GPIO_PMC1_PMC11);
}
/*******************************************************************************
 End of function riic0_port_init
 *******************************************************************************/

/******************************************************************************
 * Function Name: riic1_port_init
 * Description  : Initialises channel 1 i2c port register for this board
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
static void riic1_port_init (void)
{
    /* ---- P1_2 : SCL1 ---- */
    /* Port initialise */
    RZA_IO_RegWrite_16(&GPIO.PIBC1, 0, GPIO_PIBC1_PIBC12_SHIFT, GPIO_PIBC1_PIBC12);
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC12_SHIFT, GPIO_PBDC1_PBDC12);
    RZA_IO_RegWrite_16(&GPIO.PM1, 1, GPIO_PM1_PM12_SHIFT, GPIO_PM1_PM12);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 0, GPIO_PMC1_PMC12_SHIFT, GPIO_PMC1_PMC12);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC12_SHIFT, GPIO_PIPC1_PIPC12);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC12_SHIFT, GPIO_PBDC1_PBDC12);
    RZA_IO_RegWrite_16(&GPIO.PFC1, 0, GPIO_PFC1_PFC12_SHIFT, GPIO_PFC1_PFC12);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE12_SHIFT, GPIO_PFCE1_PFCE12);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE12_SHIFT, GPIO_PFCAE1_PFCAE12);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC12_SHIFT, GPIO_PIPC1_PIPC12);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 1, GPIO_PMC1_PMC12_SHIFT, GPIO_PMC1_PMC12);

    /* ---- P1_3 : SDA1 ---- */
    /* Port initialise */
    RZA_IO_RegWrite_16(&GPIO.PIBC1, 0, GPIO_PIBC1_PIBC13_SHIFT, GPIO_PIBC1_PIBC13);
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC13_SHIFT, GPIO_PBDC1_PBDC13);
    RZA_IO_RegWrite_16(&GPIO.PM1, 1, GPIO_PM1_PM13_SHIFT, GPIO_PM1_PM13);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 0, GPIO_PMC1_PMC13_SHIFT, GPIO_PMC1_PMC13);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC13_SHIFT, GPIO_PIPC1_PIPC13);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1, 1, GPIO_PBDC1_PBDC13_SHIFT, GPIO_PBDC1_PBDC13);
    RZA_IO_RegWrite_16(&GPIO.PFC1, 0, GPIO_PFC1_PFC13_SHIFT, GPIO_PFC1_PFC13);
    RZA_IO_RegWrite_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE13_SHIFT, GPIO_PFCE1_PFCE13);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE13_SHIFT, GPIO_PFCAE1_PFCAE13);
    RZA_IO_RegWrite_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC13_SHIFT, GPIO_PIPC1_PIPC13);
    RZA_IO_RegWrite_16(&GPIO.PMC1, 1, GPIO_PMC1_PMC13_SHIFT, GPIO_PMC1_PMC13);
}
/*******************************************************************************
 End of function riic1_port_init
 ******************************************************************************/

/******************************************************************************
 * Function Name: r_riic_init_ch0
 * Description  : Initialises channel 0 i2c for this board
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
static void r_riic_init_ch0 (void)
{
    uint32_t cks;
    uint32_t brl;
    uint32_t brh;

    riic0_port_init();

    /* End configuration register settings */
    cks = RIIC_CKS_DIVISION_8;
    brl = 19;
    brh = 16;

    R_RIIC_Init(DEVDRV_CH_0, cks, brl, brh);
}
/*******************************************************************************
 End of function r_riic_init_ch0
 *******************************************************************************/

/******************************************************************************
 * Function Name: r_riic_init_ch1
 * Description  : Initialises channel 1 i2c for this board
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
static void r_riic_init_ch1 (void)
{
    uint32_t cks;
    uint32_t brl;
    uint32_t brh;

    riic1_port_init();

    /* End configuration register settings */
    cks = RIIC_CKS_DIVISION_8;
    brl = 19;
    brh = 16;

    R_RIIC_Init(DEVDRV_CH_1, cks, brl, brh);
}
/*******************************************************************************
 End of function r_riic_init_ch1
 *******************************************************************************/

/******************************************************************************
 * Function Name: R_RIIC_StreamItInit
 * Description  : Manages the initialisation of the i2c channels
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
void R_RIIC_StreamItInit (void)
{
    /* r_riic_init_ch0(); */
    r_riic_init_ch1();
}
/*******************************************************************************
 End of function R_RIIC_StreamItInit
 *******************************************************************************/

/******************************************************************************
 * Function Name: R_RIIC_StreamItRead
 * Description  : Read data from slave in single byte addressing mode
 * Arguments    : uint32_t channel : RIIC channel (0 to 3)
 *              : uint8_t d_adr    : Slave device address
 *              : uint16_t r_adr   : Slave sub-address
 *              : uint32_t r_byte  : Number of bytes
 *              : uint8_t * r_buffer : buffer for data
 * Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
 *              : DEVDRV_ERROR     : Failure of RIIC operation
 ******************************************************************************/
int32_t R_RIIC_StreamItRead (uint32_t channel, uint8_t d_adr, uint16_t r_adr, uint32_t r_byte, uint8_t * r_buffer)
{
    volatile struct st_riic * priic;

    int32_t ret;
    uint8_t pw_buffer[3];
    uint8_t dummy_data;

    priic = priic_get_reg_addr(channel);

    pw_buffer[0] = (uint8_t) (d_adr | SAMPLE_RIIC_RWCODE_W);
    pw_buffer[1] = (uint8_t) (r_adr & 0x00ff);
    pw_buffer[2] = (uint8_t) (d_adr | SAMPLE_RIIC_RWCODE_R);

    /* Is the bus free? */
    while (0x00008 == (priic->RIICnCR2.UINT32 & 0x0008))
    {
        __asm__("nop");
    }

    R_RIIC_WriteCond(channel, RIIC_TX_MODE_START);

    /* For EEPROM devices with 1 byte addressing */
    ret = R_RIIC_Write(channel, pw_buffer, 2);

    if (DEVDRV_SUCCESS == ret)
    {
        R_RIIC_WriteCond(channel, RIIC_TX_MODE_RESTART);

        ret = R_RIIC_SingleWrite(channel, pw_buffer[2], RIIC_TEND_WAIT_RECEIVE);

        if (DEVDRV_SUCCESS == ret)
        {
            R_RIIC_Read(channel, r_buffer, r_byte);
        }
        else
        {
            R_RIIC_WriteCond(channel, RIIC_TX_MODE_STOP);
            R_RIIC_SingleRead(channel, &dummy_data);
            R_RIIC_DetectStop(channel);
        }
    }

    return (ret);
}
/*******************************************************************************
 End of function R_RIIC_StreamItRead
 *******************************************************************************/

/******************************************************************************
 * Function Name: R_RIIC_StreamItWrite
 * Description  : Write data to slave in single byte addressing mode
 * Arguments    : uint32_t channel : RIIC channel (0 to 3)
 *              : uint8_t d_adr    : Slave device address
 *              : uint16_t w_adr   : Slave sub-address
 *              : uint32_t w_byte  : Number of bytes
 *              : uint8_t * pw_buffer : buffer for data
 * Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
 *              : DEVDRV_ERROR     : Failure of RIIC operation
 ******************************************************************************/
int32_t R_RIIC_StreamItWrite (uint32_t channel, uint8_t d_adr, uint16_t w_adr, uint32_t w_byte, uint8_t * pw_buffer)
{
    int32_t ret;
    uint8_t buf[18];
    uint8_t count;
    uint32_t delay = 200000;

    volatile struct st_riic * priic;

    priic = priic_get_reg_addr(channel);

    buf[0] = (uint8_t) (d_adr | SAMPLE_RIIC_RWCODE_W);
    buf[1] = (uint8_t) (w_adr & 0x00ff);

    for (count = 2; count < (w_byte + 2); count++)
    {
        buf[count] = (*pw_buffer++);
    }

    /* Is the channel's bus free? */
    while (0x00008 == (priic->RIICnCR2.UINT32 & 0x0008))
    {
        __asm__("nop");
    }

    R_RIIC_WriteCond(channel, RIIC_TX_MODE_START);

    ret = R_RIIC_Write(channel, buf, (2 + w_byte));

    R_RIIC_WriteCond(channel, RIIC_TX_MODE_STOP);
    R_RIIC_DetectStop(channel);

    /* Add delay to ensure IIC device has completed internal writes */
    while (delay--)
    {
        __asm__("nop");
    }

    return (ret);
}
/*******************************************************************************
 End of function R_RIIC_StreamItWrite
 *******************************************************************************/

/******************************************************************************
 * Function Name: R_RIIC_CameraWrite
 * Description  : Write to configuration register for Camera
 ******************************************************************************/
int32_t R_RIIC_CameraWrite (uint32_t channel, uint8_t d_adr, uint16_t w_adr, uint8_t data)
{
    return R_RIIC_StreamItWrite(channel, d_adr, w_adr, 1, &data);
}
/*******************************************************************************
 End of function R_RIIC_CameraWrite
 *******************************************************************************/

/******************************************************************************
 * Function Name: R_RIIC_CameraRead
 * Description  : Read data from slave in single byte addressing mode
 * Arguments    : uint32_t channel : RIIC channel (0 to 3)
 *              : uint8_t d_adr    : Slave device address
 *              : uint16_t r_adr   : Slave sub-address
 * Return Value : Read data
 ******************************************************************************/
uint8_t R_RIIC_CameraRead (uint32_t channel, uint8_t d_adr, uint16_t r_adr)
{
    uint8_t ret;

    R_RIIC_StreamItRead(channel, d_adr, r_adr, 1, &ret);

    return ret;
}
/*******************************************************************************
 End of function R_RIIC_CameraRead
 *******************************************************************************/

/* End of File */
