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
 * File Name    : r_camera_api.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2 board
 * Description  : User API functions for Camera Setup and Access
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

#include <r_bsp.h>            /* Device Driver common header */

#include "r_camera_auto_detect.h"
#include "r_camera_ceu.h"
#include "r_camera_ov7670.h"
#include "r_camera_ov7740.h"
#include "r_camera_api.h"

/******************************************************************************
 Macro Definitions
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define DELAY_TIME_1MS                      (260000uL)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static uint8_t pingpong = 0;
static void msleep_ov7xxx (uint32_t wait);
static void clean_buffer (video_buffer_t *Frame);
static void delay_1ms (void);

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/
extern uint8_t g_camera_frame_buffer;

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
camera_config_t g_camera_config =
{ .initialized = 0, .resolution = CAMERA_RES_QVGA, .model = CAMERA_TYPE_NONE };

/*****************************************************************************
 Function Name: R_CAM_Init
 Description:   Initialises the external camera module and CEU.
 Camera model defined by : g_camera_config.model
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_CAM_Init (void)
{
    uint8_t detected_camera_id;

    g_camera_config.initialized = false;

    /* camera auto detection must already have been run */
    detected_camera_id = R_CAMERA_GetDetectedId();

    if ((0x76 == detected_camera_id) && (CAMERA_TYPE_AUTO == g_camera_config.model))
    {
        g_camera_config.model = CAMERA_TYPE_OV7670;
    }
    else if ((0x77 == detected_camera_id) && (CAMERA_TYPE_AUTO == g_camera_config.model))
    {
        g_camera_config.model = CAMERA_TYPE_OV7740;
    }
    else if (CAMERA_TYPE_AUTO == g_camera_config.model)
    {
        g_camera_config.model = CAMERA_TYPE_NONE;
    }
    else
    {
        /* Do nothing */
    }

    clean_buffer((video_buffer_t *) &g_camera_frame_buffer);

    /* Initialise the Omnivision internal registers */
    if ((CAMERA_TYPE_OV7740 == g_camera_config.model) && (0x77 == detected_camera_id))
    {
        R_CAMERA_Ov7740Init(g_camera_config.resolution);
    }
    else if ((CAMERA_TYPE_OV7670 == g_camera_config.model) && (0x76 == detected_camera_id))
    {
        R_CAMERA_Ov7670Init(g_camera_config.resolution);
    }
    else
    {
        /* Do nothing */
    }

    if (CAMERA_RES_QVGA == g_camera_config.resolution)
    {
        R_CEU_Config.USEFUL_HEIGHT = VGA_HEIGHT / 2;
        R_CEU_Config.USEFUL_WIDTH = VGA_WIDTH / 2;
    }
    else
    {
        R_CEU_Config.USEFUL_HEIGHT = VGA_HEIGHT;
        R_CEU_Config.USEFUL_WIDTH = VGA_WIDTH;
    }

    R_CEU_Config.LINE_WIDTH = TOTAL_LINE_WIDTH;

    /* RZ-CEU Initialisation */
    R_CAM_CeuInit();

    if (g_camera_config.initialized)
    {
        R_CAM_StartCapture();
    }
}
/*****************************************************************************
 End of function  R_CAM_Init
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_CAM_StartCapture
 Description:   xxxx
 Parameters:    None
 Return value:  None
 *****************************************************************************/
void R_CAM_StartCapture (void)
{
    while (CEU_CaptureOneFrame((video_buffer_t *) &g_camera_frame_buffer) != CEU_ERROR_OK)
    {
        /* Do nothing */
    }

    pingpong = 1;
}
/*****************************************************************************
 End of function  R_CAM_StartCapture
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_CAM_GetFrame
 Description:   Gets one frame from the capture driver.
 Parameters:    None
 Return value:  Pointer to camera buffer.
 *****************************************************************************/
uint8_t * R_CAM_GetFrame (void)
{
    uint8_t * pcapture_buffer;

    if (0 == pingpong)
    {
        pingpong = 1;
        pcapture_buffer = (unsigned char *) &g_camera_frame_buffer;
    }
    else
    {
        /* Get the YUV420 frame buffer pointer from capture driver */
        while (CEU_CaptureOneFrame((video_buffer_t *) &g_camera_frame_buffer) != CEU_ERROR_OK)
        {
            /* Do nothing */
        }
        pingpong = 0;
    }

    return pcapture_buffer;
}
/*****************************************************************************
 End of function  R_CAM_GetFrame
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: msleep_ov7xxx
 Description:   Software delay function.
 This function could be replaced by RTOS delay function
 Parameters:    wait - number of WAIT_TICK cycles to delay for.
 Return value:  None
 *****************************************************************************/
static void msleep_ov7xxx (uint32_t wait)
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
 End of function  msleep_ov7xxx
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: clean_buffer
 Description:   Fills frame buffer with 0x55.
 Parameters:    Pointer to frame buffer
 Return value:  None
 *****************************************************************************/
static void clean_buffer (video_buffer_t *Frame)
{
    uint32_t i;

    if (CAMERA_RES_VGA == g_camera_config.resolution)
    {
        for (i = 0; i < ((VGA_WIDTH * VGA_HEIGHT) * 2); i++)
        {
            ((uint8_t*) Frame)[i] = 0x55;
        }
    }

    if (CAMERA_RES_QVGA == g_camera_config.resolution)
    {
        for (i = 0; i < ((VGA_WIDTH * VGA_HEIGHT) / 2); i++)
        {
            ((uint8_t*) Frame)[i] = 0x55;
        }
    }
}
/*****************************************************************************
 End of function  clean_buffer
 ******************************************************************************/

/******************************************************************************
 * Function Name: delay_1ms
 * Description  : Software delay for 1ms
 * Arguments    : None
 * Return Value : None
 ******************************************************************************/
static void delay_1ms (void)
{
    uint32_t delay_count = 0u;

    while (DELAY_TIME_1MS >= delay_count)
    {
        delay_count++;
    }
}
/*****************************************************************************
 End of function  delay_1ms
 ******************************************************************************/

/* End of File */
