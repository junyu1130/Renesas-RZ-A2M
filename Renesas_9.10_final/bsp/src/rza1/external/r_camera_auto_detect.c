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
 * File Name    : r_camera_auto_detect.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2 board
 * Description  : Camera Auto Detect OV7670 / OV7740
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
#include <stdio.h>
#include <string.h>

#include <r_bsp.h>            /* Device Driver common header */
#include "r_camera_auto_detect.h"


/******************************************************************************
 Macro Definitions
 ******************************************************************************/
/* I2C pins */
#define SCL_OUT(a) (GPIO.PMSR1 = ((a) ? (GPIO_PMSR1_PMSR118 | GPIO_PMSR1_PMSR12) : (GPIO_PMSR1_PMSR118)))
#define SDA_OUT(a) (GPIO.PMSR1 = ((a) ? (GPIO_PMSR1_PMSR119 | GPIO_PMSR1_PMSR13) : (GPIO_PMSR1_PMSR119)))
#define SDA_IN()  (GPIO.PPR1 & GPIO_PPR1_PPR13)

#define WAIT_TICK (100000)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void camera_i2c_delay (void);
static void camera_i2c_start (void);
static void camera_i2c_stop (void);
static void camera_i2c_repeated_start (void);
static uint8_t camera_i2c_write (uint8_t data);
static uint8_t camera_i2c_read (uint8_t ack);

static uint8_t detected_camera_id;

/******************************************************************************
 Public Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: camera_iic_read
 Description:   Read a register value from the camera module
 Parameters:    channel - Unused
 d_adr - Device Address
 w_adr - Register Address
 pdata -  Pointer for read data
 Return value:  1 - Error, 0 - Success
 *****************************************************************************/
static int32_t camera_iic_read (uint32_t channel, uint8_t d_adr, uint16_t w_adr, uint8_t * pdata)
{
    uint32_t error;

    /* Issue a start condition */
    camera_i2c_start();

    /* Send I2C slave address */
    error = camera_i2c_write(d_adr);

    /* Check status code */
    if (!error)
    {
        /* Write register address */
        error = camera_i2c_write(w_adr);
    }

    /* Check status code */
    if (!error)
    {
        /* Issue a start condition */
        camera_i2c_repeated_start();

        /* Send I2C slave address */
        error = camera_i2c_write(d_adr | 1);

        if (!error)
        {
            pdata[0] = camera_i2c_read(false);
        }

        /* Issue a stop condition */
        camera_i2c_stop();
    }

    /* Return status code */
    return error;
}
/*****************************************************************************
 End of function camera_iic_read
 ******************************************************************************/

/*****************************************************************************
 Function Name: camera_iic_read2
 Description:   Read a register value from the camera module
 Parameters:    channel - Unused
 d_adr - Device Address
 w_adr - Register Address
 pdata -  Pointer for read data
 Return value:  1 - Error, 0 - Success
 *****************************************************************************/
static int32_t camera_iic_read2 (uint32_t channel, uint8_t d_adr, uint16_t w_adr, uint8_t * data)
{
    uint32_t error;

    /* Issue a start condition */
    camera_i2c_start();

    /* Send I2C slave address */
    error = camera_i2c_write(d_adr);

    /* Check status code */
    if (!error)
    {
        /* Write register address */
        error = camera_i2c_write(w_adr);
    }

    if (!error)
    {
        /* Issue a stop condition */
        camera_i2c_stop();

        camera_i2c_delay();

        camera_i2c_start();

        /* Send I2C slave address */
        error = camera_i2c_write(d_adr | 1);

        if (!error)
        {
            data[0] = camera_i2c_read(false);
        }

        /* Issue a stop condition */
        camera_i2c_stop();
    }

    /* Return status code */
    return error;
}
/*****************************************************************************
 End of function camera_iic_read2
 ******************************************************************************/

/*****************************************************************************
 Function Name: camera_iic_init
 Description:   Initialises the IIC pins for the camera module
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void camera_iic_init (void)
{
    /* Configure SCL1 pin */
    GPIO.PMC1 &= (~GPIO_PMC1_PMC12);     /* PMC1.2 = 0 */
    GPIO.PIBC1 |= GPIO_PIBC1_PIBC12;     /* PIBC1.2 = 1 */
    GPIO.PM1 |= GPIO_PM1_PM12;             /* PM1.2 = 1 */
    GPIO.P1 &= (~GPIO_P1_P12);             /* P1.2 = 0 */

    /* Configure SDA1 pin */
    GPIO.PMC1 &= (~GPIO_PMC1_PMC13);     /* PMC1.3 = 0 */
    GPIO.PIBC1 |= GPIO_PIBC1_PIBC13;    /* PIBC1.3 = 1 */
    GPIO.PM1 |= GPIO_PM1_PM13;             /* PM1.3 = 1 */
    GPIO.P1 &= (~GPIO_P1_P13);             /* P1.3 = 0 */
}
/*****************************************************************************
 End of function camera_iic_init
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/
/*****************************************************************************
 Function Name: camera_i2c_delay
 Description:   Short software delay loop to support IIC bit-bashed timing
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void camera_i2c_delay (void)
{
    volatile uint16_t delay;

    /* Delay loop */
    for (delay = 0; delay < 1000; delay++)
    {
        ; /* Do nothing */
    }
}
/*****************************************************************************
 End of function camera_i2c_delay
 ******************************************************************************/

/*****************************************************************************
 Function Name: camera_i2c_start
 Description:   Issue an IIC start condition
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void camera_i2c_start (void)
{
    SDA_OUT(1);
    SCL_OUT(1);
    camera_i2c_delay();

    /* Pull SDA to low level */
    SDA_OUT(0);
    camera_i2c_delay();

    /* Pull SCL to low level */
    SCL_OUT(0);
    camera_i2c_delay();
}
/*****************************************************************************
 End of function camera_i2c_start
 ******************************************************************************/

/*****************************************************************************
 Function Name: camera_i2c_stop
 Description:   Issue an IIC stop condition
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void camera_i2c_stop (void)
{
    SDA_OUT(0);
    camera_i2c_delay();

    /* Release SCL */
    SCL_OUT(1);
    camera_i2c_delay();

    /* Release SDA */
    SDA_OUT(1);
    camera_i2c_delay();
}
/*****************************************************************************
 End of function camera_i2c_stop
 ******************************************************************************/

/*****************************************************************************
 Function Name: camera_i2c_repeated_start
 Description:   Issue and IIC repeated start condition
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void camera_i2c_repeated_start (void)
{
    /* Release SDA */
    SDA_OUT(1);
    camera_i2c_delay();

    /* Release SCL */
    SCL_OUT(1);
    camera_i2c_delay();

    /* Pull SDA to low level */
    SDA_OUT(0);
    camera_i2c_delay();

    /* Pull SCL to low level */
    SCL_OUT(0);
    camera_i2c_delay();
}
/*****************************************************************************
 End of function camera_i2c_repeated_start
 ******************************************************************************/

/*****************************************************************************
 Function Name: camera_i2c_write
 Description:   Writes byte to camera module.
 Parameters:    Data byte to write
 Return value:  1 - Error, 0 - Success
 *****************************************************************************/
static uint8_t camera_i2c_write (uint8_t data)
{
    uint8_t error;
    uint16_t i;

    /* Iterate 8 times */
    for (i = 0; i < 8; i++)
    {
        /* Set SDA state */
        if (data & 0x80)
        {
            SDA_OUT(1);
        }
        else
        {
            SDA_OUT(0);
        }

        /* Pulse SCL */
        camera_i2c_delay();
        SCL_OUT(1);
        camera_i2c_delay();
        SCL_OUT(0);
        camera_i2c_delay();

        /* Shift data byte */
        data <<= 1;
    }

    /* Release SDA in order to read ACK bit */
    SDA_OUT(1);
    camera_i2c_delay();

    /* Set SCL to high level */
    SCL_OUT(1);
    camera_i2c_delay();

    /* Retrieve ACK value */
    if (SDA_IN())
    {
        error = 1;
    }
    else
    {
        error = 0;
    }

    /* Pull SCL to low level */
    SCL_OUT(0);
    camera_i2c_delay();

    /* Return status code */
    return error;
}
/*****************************************************************************
 End of function camera_i2c_write
 ******************************************************************************/

/*****************************************************************************
 Function Name: camera_i2c_read
 Description:   Read byte from camera module
 Parameters:    1 - Send acknowledge, 0 - do not send acknowledge
 Return value:  Data Byte read from camera.
 *****************************************************************************/
static uint8_t camera_i2c_read (uint8_t ack)
{
    uint8_t i;
    uint8_t data;

    /* Clear data */
    data = 0;

    /* Release SDA */
    SDA_OUT(1);
    camera_i2c_delay();

    /* Iterate 8 times */
    for (i = 0; i < 8; i++)
    {
        /* Shift data byte */
        data <<= 1;

        /* Set SCL to high level */
        SCL_OUT(1);
        camera_i2c_delay();

        /* Retrieve bit value */
        if (SDA_IN())
        {
            data |= 1;
        }

        /* Pull SCL to low level */
        SCL_OUT(0);
        camera_i2c_delay();
    }

    /* Write ACK bit */
    if (ack)
    {
        SDA_OUT(0);
    }
    else
    {
        SDA_OUT(1);
    }

    /* Pulse SCL */
    camera_i2c_delay();
    SCL_OUT(1);
    camera_i2c_delay();
    SCL_OUT(0);
    camera_i2c_delay();

    /* Return data byte */
    return data;
}
/*****************************************************************************
 End of function camera_i2c_read
 ******************************************************************************/

/*****************************************************************************
 Function Name: msleep_auto_detect
 Description:   Software delay function.
 This function could be replaced by RTOS delay function
 Parameters:    wait - number of WAIT_TICK cycles to delay for.
 Return value:  None
 *****************************************************************************/
static void msleep_auto_detect (uint32_t wait)
{
    uint32_t i;
    for (i = WAIT_TICK; ((i >= 0) && (0 != wait)); i--)
    {
        if (0 == i)
        {
            wait--;
            i = WAIT_TICK;
        }
    }
}
/*****************************************************************************
 End of function  msleep_auto_detect
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_CAMERA_AutoDetect
 Description:   Detect camera model fitted
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_CAMERA_AutoDetect (void)
{
    uint8_t id_msb = 0;

    camera_iic_init();                     /* set up the ports */

    GPIO.PMC2 &= (~GPIO_PMC2_PMC28);    /* PMC2.8 = 0 */
    GPIO.PM2 &= (~GPIO_PM2_PM28);       /* PM2.8 = 0 */
    GPIO.P2 |= GPIO_P2_P28;             /* P2.8 = 1 */

    msleep_auto_detect(10); /* The Omnivision camera needs some wait time after reset */

    /* Active Power up on the camera */
    GPIO.PMC2 &= (~GPIO_PMC2_PMC28);    /* PMC2.8 = 0 */
    GPIO.PM2 &= (~GPIO_PM2_PM28);       /* PM2.8 = 0 */
    GPIO.P2 &= (~GPIO_P2_P28);          /* P2.8 = 0 */

    msleep_auto_detect(10);             /* The Omnivision camera needs some wait time after reset */

    int8_t count = 25;

    do
    {
        camera_iic_read(DEVDRV_CH_1, 0x42, 0x0A, &id_msb);
        count--;
    } while ((count >= 0) && (0 == id_msb));

    if (count < 0)
    {
        count = 25;

        do
        {
            camera_iic_read2(DEVDRV_CH_1, 0x42, 0x0A, &id_msb);
            count--;
        } while ((count >= 0) && (0 == id_msb));
    }

    detected_camera_id = id_msb;

    if (0x77 == detected_camera_id)
    {
        TRACE_INFO("Detected OV7740 camera\r\n");
    }

    if (0x76 == detected_camera_id)
    {
        TRACE_INFO("Detected OV7670 camera\r\n");
    }

    if (0 == detected_camera_id)
    {
        TRACE_INFO("No camera detected\r\n");
    }
}
/*****************************************************************************
 End of function  R_CAMERA_AutoDetect
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_CAMERA_GetDetectedId
 Description:   Get the id byte of the detected camera
 Parameters:    None
 Return value:  ID byte of camera, 0 if camera was not detected
 0x76    OV7670 camera detected
 0x77    OV7740 camera detected
 *****************************************************************************/
uint8_t R_CAMERA_GetDetectedId ()
{
    return detected_camera_id;
}
/*****************************************************************************
 End of function  R_CAMERA_GetDetectedId
 ******************************************************************************/

/* End of File */

