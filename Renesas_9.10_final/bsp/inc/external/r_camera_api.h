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
 * File Name    : r_camera_api.h
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2
 * Description  : Header for r_camera_api.c
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.08.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

#ifndef R_CAMERA_API_H
    #define R_CAMERA_API_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/


    #define VGA_WIDTH              (640)
    #define VGA_HEIGHT             (480)

    #define TOTAL_LINE_WIDTH    (784)

    #define Y_SIZE                 (320*240)
    #define UV_SIZE             (Y_SIZE/2)

typedef struct
{
    uint8_t addr;
    uint8_t val;
} omni_register_t;

typedef struct
{
    uint16_t               width;
    uint16_t               height;
    uint8_t                com7_bit;
    uint16_t               hstart;        // Start/stop values for the camera.
    uint16_t               hstop;
    uint16_t               vstart;
    uint16_t               vstop;
    omni_register_t *psize_regs; // Regs to tweak, NULL if not change required
} image_size_t;

typedef struct
{
    uint8_t y[Y_SIZE];
    uint8_t uv[UV_SIZE];
} video_buffer_t;

typedef enum
{
    CAMERA_RES_QVGA, CAMERA_RES_VGA
} camera_res_t;

typedef enum
{
    CAMERA_TYPE_NONE, CAMERA_TYPE_AUTO, CAMERA_TYPE_OV7670, CAMERA_TYPE_OV7740
} camera_type_t;

typedef struct
{
    uint8_t       initialized;
    camera_type_t model;
    camera_res_t  resolution;
} camera_config_t;

extern camera_config_t g_camera_config;

/******************************************************************************
 Variable Externs
 ******************************************************************************/

/******************************************************************************
 Functions Prototypes
 ******************************************************************************/
void R_CAM_Init (void);
void R_CAM_StartCapture (void);
uint8_t* R_CAM_GetFrame (void);

#endif  /* R_CAMERA_API_H */

/* End of File */
