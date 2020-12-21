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
 * File Name    : r_graphics_drv_wrapper.h
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2
 * Description  : Header file for r_graphics_drv_wrapper.c
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.08.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

#ifndef GRAPHICS_DRV_WRAPPER_H
    #define GRAPHICS_DRV_WRAPPER_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
    #include    <stdlib.h>

/* Default  type definition header */
#include "r_typedefs.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
vdc5_error_t R_GRPDRV_Init (const vdc5_ext_in_sig_t * ext_input_sig);
vdc5_error_t R_GRPDRV_Term (void);
vdc5_error_t R_GRPDRV_GraphicsCreateSurface (const vdc5_layer_id_t layer_id, void * const framebuff,
        const uint32_t fb_stride, const vdc5_gr_format_t gr_format, const vdc5_period_rect_t * const period_rect);
vdc5_error_t R_GRPDRV_VideoCreateSurface (void * const framebuff, const uint32_t fb_stride, const vdc5_res_md_t res_md,
        const vdc5_period_rect_t * const res, const vdc5_period_rect_t * const period_rect);
vdc5_error_t R_GRPDRV_DestroySurfaces (void);
vdc5_error_t R_GRPDRV_StartSurfaces (const vdc5_gr_disp_sel_t * const gr_disp_sel);
vdc5_error_t R_GRPDRV_StopSurfaces (void);
vdc5_error_t GRPDRV_L_R_GRPDRV_StopSurfacesStopSurfaces (void);

#endif  /* GRAPHICS_DRV_WRAPPER_H */
