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
 * File Name    : r_graphics_drv_wrapper.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2
 * Description  : Graphics driver wrapper function
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
#include <string.h>

/* Default type definition header */
#include <r_bsp.h>            /* Device Driver common header */
#include "r_lcd_panel.h"
#include "r_graphics_drv_wrapper.h"

#ifdef INCLUDE_GFX_DEBUG
    #include "graphics_debug.h"
#endif /*INCLUDE_GFX_DEBUG */

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define STP91_BIT   (0x02u)
#define STBRQ25_BIT (0x20u)
#define STBAK25_BIT (0x20u)

/* If DebugPrint is not available provide a dummy function */
#ifndef GRAPHICS_DebugPrint
    #define GRAPHICS_DebugPrint(a,b,c) ((void)(a))
#endif /* INCLUDE_GFX_DEBUG */

#define VSYNC_1_2_FH_TIMING  (858u) /* Vsync signal 1/2fH phase timing */
#define VSYNC_1_4_FH_TIMING  (429u) /* Vsync signal 1/4fH phase timing */

#define BUFFER_NUM (2u)

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef struct
{
    vdc5_panel_clksel_t   panel_icksel; /* Panel clock select */
    vdc5_panel_clk_dcdr_t panel_dcdr; /* Panel clock frequency division ratio */
    uint16_t              res_fv; /* Free-running Vsync period setting */
    uint16_t              res_fh; /* Hsync period setting */
    vdc5_period_rect_t    res_f; /* Full-screen enable signal */
    uint16_t              tcon_half; /* TCON reference timing, 1/2fH timing */
    uint16_t              tcon_offset; /* TCON reference timing, offset Hsync signal timing */
    vdc5_lcd_outformat_t  out_format; /* Output format select */
    vdc5_edge_t           outcnt_lcd_edge;/* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
} lcd_dependent_param_t;

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/*! Conversion table from video-signal writing format into read signal format */
static const vdc5_gr_format_t gr_format_conv[VDC5_RES_MD_NUM] =
{ VDC5_GR_FORMAT_YCBCR422, /* YCbCr422 */
VDC5_GR_FORMAT_RGB565, /* RGB565 */
VDC5_GR_FORMAT_RGB888, /* RGB888 */
VDC5_GR_FORMAT_YCBCR444 /* YCbCr444 */
};

/*! Swap mode */
static const vdc5_wr_rd_swa_t gr_rdswa_tbl[VDC5_GR_FORMAT_NUM] =
{ VDC5_WR_RD_WRSWA_32_16BIT, /* RGB565 */
VDC5_WR_RD_WRSWA_32BIT, /* RGB888 */
VDC5_WR_RD_WRSWA_32_16BIT, /* ARGB1555 */
VDC5_WR_RD_WRSWA_32_16BIT, /* ARGB4444 */
VDC5_WR_RD_WRSWA_32BIT, /* ARGB8888 */
VDC5_WR_RD_WRSWA_32_16_8BIT, /* CLUT8 */
VDC5_WR_RD_WRSWA_32_16_8BIT, /* CLUT4 */
VDC5_WR_RD_WRSWA_32_16_8BIT, /* CLUT1 */
VDC5_WR_RD_WRSWA_32BIT, /* YCbCr422 */
VDC5_WR_RD_WRSWA_32BIT, /* YCbCr444 */
VDC5_WR_RD_WRSWA_32_16BIT, /* RGBA5551 */
VDC5_WR_RD_WRSWA_32BIT /* RGBA8888 */
};

static const lcd_dependent_param_t lcd_dependent_param =
{
LCD_CH0_PANEL_CLK, /* Panel clock select */
LCD_CH0_PANEL_CLK_DIV, /* Panel clock frequency division ratio */
(uint16_t) LCD_CH0_SIG_FV, /* Free-running Vsync period setting */
(uint16_t) LCD_CH0_SIG_FH, /* Hsync period setting */
{
/* Full-screen enable signal */
(uint16_t) LCD_CH0_DISP_VS, (uint16_t) LCD_CH0_DISP_VW, (uint16_t) LCD_CH0_DISP_HS, (uint16_t) LCD_CH0_DISP_HW },
        (uint16_t) LCD_CH0_TCON_HALF, /* TCON reference timing, 1/2fH timing */
        (uint16_t) LCD_CH0_TCON_OFFSET, /* TCON reference timing, offset Hsync signal timing */
        LCD_CH0_OUT_FORMAT, /* Output format select */
        LCD_CH0_OUT_EDGE /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
};

static void init_func (const uint32_t user_num);
static void quit_func (const uint32_t user_num);

/*****************************************************************************
 Function Name: R_GRPDRV_Init
 Description:   Graphics initialization processing
 Parameters:    ext_input_sig       : External input signal parameter
 Return value:  vdc5_error_t result of initialize attempt
 *****************************************************************************/
vdc5_error_t R_GRPDRV_Init (const vdc5_ext_in_sig_t * ext_input_sig)
{
    vdc5_error_t error;
    vdc5_init_t init;
    vdc5_input_t input;
    vdc5_sync_ctrl_t sync_ctrl;
    vdc5_output_t output;
    const lcd_dependent_param_t * param;

    param = &lcd_dependent_param;

    /* Initialization parameter */
    init.panel_icksel = param->panel_icksel; /* Panel clock select */
    init.panel_dcdr = param->panel_dcdr; /* Panel clock frequency division ratio */
    init.lvds = NULL; /* LVDS parameter */

    /* Initialize */
    error = R_VDC5_Initialize(VDC5_CHANNEL_0, &init, &init_func, (uint32_t) 0u);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }

    if (error == VDC5_OK)
    {
        if (NULL != ext_input_sig)
        {
            /* Input parameter */
            input.inp_sel = VDC5_INPUT_SEL_EXT; /* Input select */
            input.inp_fh50 = (uint16_t) VSYNC_1_2_FH_TIMING; /* Vsync signal 1/2fH phase timing */
            input.inp_fh25 = (uint16_t) VSYNC_1_4_FH_TIMING; /* Vsync signal 1/4fH phase timing */
            input.dly = NULL; /* Sync signal delay adjustment */
            input.ext_sig = ext_input_sig; /* External input signal */

            /* Video input */
            error = R_VDC5_VideoInput(VDC5_CHANNEL_0, &input);
            if (error != VDC5_OK)
            {
                GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
            }
        }
    }

    if (error == VDC5_OK)
    {
        /* Sync signal control */
        sync_ctrl.res_vs_sel = VDC5_ON; /* Vsync signal output select (free-running Vsync on/off control) */

        /* Sync signal output and full-screen enable signal select */
        sync_ctrl.res_vs_in_sel = VDC5_RES_VS_IN_SEL_SC0;
        sync_ctrl.res_fv = param->res_fv; /* Free-running Vsync period setting */
        sync_ctrl.res_fh = param->res_fh; /* Hsync period setting */
        sync_ctrl.res_vsdly = (uint16_t) 0u; /* Vsync signal delay control */

        /* Full-screen enable control */
        sync_ctrl.res_f.vs = param->res_f.vs;
        sync_ctrl.res_f.vw = param->res_f.vw;
        sync_ctrl.res_f.hs = param->res_f.hs;
        sync_ctrl.res_f.hw = param->res_f.hw;
        sync_ctrl.vsync_cpmpe = NULL; /* Vsync signal compensation */

        /* Sync control */
        error = R_VDC5_SyncControl(VDC5_CHANNEL_0, &sync_ctrl);
        if (error != VDC5_OK)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        }
    }

    if (error == VDC5_OK)
    {
        /* Output parameter */
        output.tcon_half = param->tcon_half; /* TCON reference timing, 1/2fH timing */
        output.tcon_offset = param->tcon_offset; /* TCON reference timing, offset Hsync signal timing */

        /* LCD TCON timing setting */
        R_LCD_SetLcdTconSettings(VDC5_CHANNEL_0, output.outctrl);
        output.outcnt_lcd_edge = param->outcnt_lcd_edge; /* Output phase control of LCD_DATA23 to LCD_DATA0 pin */
        output.out_endian_on = VDC5_OFF; /* Bit endian change on/off control */
        output.out_swap_on = VDC5_OFF; /* B/R signal swap on/off control */
        output.out_format = param->out_format; /* Output format select */
        output.out_frq_sel = VDC5_LCD_PARALLEL_CLKFRQ_1; /* Clock frequency control */
        output.out_dir_sel = VDC5_LCD_SERIAL_SCAN_FORWARD; /* Scan direction select */
        output.out_phase = VDC5_LCD_SERIAL_CLKPHASE_0; /* Clock phase adjustment */
        output.bg_color = (uint32_t) 0x00000000u; /* Background color in 24-bit RGB color format */

        /* Display output */
        error = R_VDC5_DisplayOutput(VDC5_CHANNEL_0, &output);
        if (error != VDC5_OK)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        }
    }
    return error;
}

/*****************************************************************************
 End of function R_GRPDRV_Init
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_GRPDRV_Term
 Description:   Graphics termination processing
 Parameters:    None
 Return value:  VDC5 driver error code
 *****************************************************************************/
vdc5_error_t R_GRPDRV_Term (void)
{
    vdc5_error_t error;

    /* Terminate */
    error = R_VDC5_Terminate(VDC5_CHANNEL_0, &quit_func, (uint32_t) 0u);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}

/*****************************************************************************
 End of function  R_GRPDRV_Term
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_GRPDRV_GraphicsCreateSurface
 Description:   Graphics surface creation processing
 Parameters:
 layer_id    : Layer ID
 framebuff   : Base address of the frame buffer
 fb_stride   : Line offset address of the frame buffer
 gr_format   : Format of the frame buffer read signal
 period_rect : Graphics display area
 Return value:  VDC5 driver error code
 *****************************************************************************/
vdc5_error_t R_GRPDRV_GraphicsCreateSurface (const vdc5_layer_id_t layer_id, void * const framebuff,
        const uint32_t fb_stride, const vdc5_gr_format_t gr_format, const vdc5_period_rect_t * const period_rect)
{
    vdc5_error_t error;
    vdc5_read_t read;

    /* Read data parameter */
    read.gr_ln_off_dir = VDC5_GR_LN_OFF_DIR_INC; /* Line offset address direction of the frame buffer */
    read.gr_flm_sel = VDC5_GR_FLM_SEL_FLM_NUM; /* Selects a frame buffer address setting signal */
    read.gr_imr_flm_inv = VDC5_OFF; /* Frame buffer number for distortion correction */
    read.gr_bst_md = VDC5_BST_MD_32BYTE; /* Frame buffer burst transfer mode */
    read.gr_base = framebuff; /* Frame buffer base address */
    read.gr_ln_off = fb_stride; /* Frame buffer line offset address */
    read.width_read_fb = NULL; /* Width of the image read from frame buffer */
    read.adj_sel = VDC5_OFF; /* Measures to decrease the influence
     by folding pixels/lines (on/off) */
    read.gr_format = gr_format; /* Format of the frame buffer read signal */
    read.gr_ycc_swap = VDC5_GR_YCCSWAP_CBY0CRY1; /* Controls swapping of data read from buffer
     in the YCbCr422 format */
    read.gr_rdswa = gr_rdswa_tbl[gr_format]; /* Frame buffer swap setting */

    /* Display area */
    read.gr_grc.vs = period_rect->vs;
    read.gr_grc.vw = period_rect->vw;
    read.gr_grc.hs = period_rect->hs;
    read.gr_grc.hw = period_rect->hw;

    /* Read data control */
    error = R_VDC5_ReadDataControl(VDC5_CHANNEL_0, layer_id, &read);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}

/******************************************************************************
 End  Of function R_GRPDRV_GraphicsCreateSurface
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_GRPDRV_VideoCreateSurface
 Description:   Video surface creation processing
 Parameters:
 framebuff   : Base address of the frame buffer
 fb_stride   : Line offset address of the frame buffer
 res_md      : Frame buffer video-signal writing format
 res         : Image area to be captured
 period_rect : Graphics display area
 Return value:  VDC5 driver error code
 *****************************************************************************/
vdc5_error_t R_GRPDRV_VideoCreateSurface (void * const framebuff, const uint32_t fb_stride, const vdc5_res_md_t res_md,
        const vdc5_period_rect_t * const res, const vdc5_period_rect_t * const period_rect)
{
    vdc5_error_t error;
    vdc5_write_t write;
    vdc5_scalingdown_rot_t * pscldw_rot;
    vdc5_read_t read;
    vdc5_width_read_fb_t w_read_fb;
    vdc5_gr_format_t gr_format_tmp;

    gr_format_tmp = gr_format_conv[res_md];

    /* Scaling-down and rotation parameter */
    pscldw_rot = &write.scalingdown_rot;

    /* Image area to be captured */
    pscldw_rot->res.vs = (uint16_t) ((uint32_t) res->vs - 1u);
    pscldw_rot->res.vw = res->vw;
    pscldw_rot->res.hs = res->hs;
    pscldw_rot->res.hw = res->hw;

    /* Display area */
    read.gr_grc.vs = period_rect->vs;
    read.gr_grc.vw = period_rect->vw;
    read.gr_grc.hs = period_rect->hs;
    read.gr_grc.hw = period_rect->hw;

    /* Width of the image output from scaling-down control block
     Width of the image read from frame buffer */
    if ((uint32_t) res->vw <= (uint32_t) read.gr_grc.vw)
    {
        w_read_fb.in_vw = res->vw;
    }
    else
    {
        w_read_fb.in_vw = read.gr_grc.vw;
    }
    if ((uint32_t) res->hw <= (uint32_t) read.gr_grc.hw)
    {
        w_read_fb.in_hw = res->hw;
    }
    else
    {
        w_read_fb.in_hw = read.gr_grc.hw;
    }

    /* Write data parameter */
    pscldw_rot->res_pfil_sel = VDC5_ON; /* Prefilter mode select for brightness signals (on/off) */
    pscldw_rot->res_out_vw = w_read_fb.in_vw; /* Number of valid lines in vertical direction
     output by scaling-down control block */
    pscldw_rot->res_out_hw = w_read_fb.in_hw; /* Number of valid horizontal pixels
     output by scaling-down control block */
    pscldw_rot->adj_sel = VDC5_ON; /* Measures to decrease the influence
     by lack of last-input line (on/off) */
    pscldw_rot->res_ds_wr_md = VDC5_WR_MD_NORMAL; /* Frame buffer writing mode */
    write.res_wrswa = VDC5_WR_RD_WRSWA_NON; /* Frame buffer swap setting */
    write.res_md = res_md; /* Frame buffer video-signal writing format */
    write.res_bst_md = VDC5_BST_MD_32BYTE; /* Transfer burst length for frame buffer */
    write.res_inter = VDC5_RES_INTER_INTERLACE; /* Field operating mode select */
    write.res_fs_rate = VDC5_RES_FS_RATE_PER1; /* Writing rate */
    write.res_fld_sel = VDC5_RES_FLD_SEL_TOP; /* Write field select */
    write.res_dth_on = VDC5_ON; /* Dither correction on/off */
    write.base = framebuff; /* Frame buffer base address */
    write.ln_off = fb_stride; /* Frame buffer line offset address [byte] */
    write.flm_num = (uint32_t) (BUFFER_NUM - 1u); /* Number of frames of buffer (res_flm_num + 1) */
    write.flm_off = fb_stride * (uint32_t) pscldw_rot->res_out_vw; /*Frame buffer frame offset address */
    write.btm_base = NULL; /* Frame buffer base address for bottom */

    /* Write data control */
    error = R_VDC5_WriteDataControl(VDC5_CHANNEL_0, VDC5_LAYER_ID_0_WR, &write);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    else
    { /* Read data parameter */
        read.gr_ln_off_dir = VDC5_GR_LN_OFF_DIR_INC; /* Line offset address direction of the frame buffer */
        read.gr_flm_sel = VDC5_GR_FLM_SEL_POINTER_BUFF; /* Selects a frame buffer address setting signal */
        read.gr_imr_flm_inv = VDC5_OFF; /* Frame buffer number for distortion correction */
        read.gr_bst_md = VDC5_BST_MD_32BYTE; /* Frame buffer burst transfer mode */
        read.gr_base = framebuff; /* Frame buffer base address */
        read.gr_ln_off = fb_stride; /* Frame buffer line offset address */
        read.width_read_fb = &w_read_fb; /* Width of the image read from frame buffer */
        read.adj_sel = VDC5_ON; /* Measures to decrease the influence
         by folding pixels/lines (ON/OFF) */
        read.gr_format = gr_format_tmp; /* Format of the frame buffer read signal */
        read.gr_ycc_swap = VDC5_GR_YCCSWAP_CBY0CRY1; /* Controls swapping of data read from buffer
         in the YCbCr422 format */
        read.gr_rdswa = VDC5_WR_RD_WRSWA_NON;

        /* Read data control */
        error = R_VDC5_ReadDataControl(VDC5_CHANNEL_0, VDC5_LAYER_ID_0_RD, &read);
        if (error != VDC5_OK)
        {
            GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
        }
    }
    return error;
}

/******************************************************************************
 End Of Function R_GRPDRV_VideoCreateSurface
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_GRPDRV_DestroySurfaces
 Description:   Surface destruction processing
 Parameters:    None
 Return value:  VDC5 driver error code
 *****************************************************************************/
vdc5_error_t R_GRPDRV_DestroySurfaces (void)
{
    vdc5_error_t error;

    /* Release data control */
    error = R_VDC5_ReleaseDataControl(VDC5_CHANNEL_0, VDC5_LAYER_ID_ALL);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}

/******************************************************************************
 End  Of Function R_GRPDRV_DestroySurfaces
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_GRPDRV_StartSurfaces
 Description:   Surface start processing
 Parameters:    gr_disp_sel     : Graphics display mode
 Return value:  VDC5 driver error code
 *****************************************************************************/
vdc5_error_t R_GRPDRV_StartSurfaces (const vdc5_gr_disp_sel_t * const gr_disp_sel)
{
    vdc5_error_t error;
    vdc5_start_t start;

    start.gr_disp_sel = gr_disp_sel;

    /* Start process */
    error = R_VDC5_StartProcess(VDC5_CHANNEL_0, VDC5_LAYER_ID_ALL, &start);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}

/******************************************************************************
 End  Of Function R_GRPDRV_StartSurfaces
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_GRPDRV_StopSurfaces
 Description:   Surface stop processing
 Parameters:    gr_disp_sel     : Graphics display mode
 Return value:  VDC5 driver error code
 *****************************************************************************/
vdc5_error_t R_GRPDRV_StopSurfaces (void)
{
    vdc5_error_t error;

    /* Stop process */
    error = R_VDC5_StopProcess(VDC5_CHANNEL_0, VDC5_LAYER_ID_ALL);
    if (error != VDC5_OK)
    {
        GRAPHICS_DebugPrint(__FILE__, __LINE__, (int32_t)error);
    }
    return error;
}

/******************************************************************************
 End  Of Function R_GRPDRV_StopSurfaces
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: init_func
 Description:   User-defined function within R_VDC5_Initialize
 Parameters:    user_num - unused
 Return value:  None
 *****************************************************************************/
static void init_func (const uint32_t user_num)
{
    uint32_t reg_data;
    volatile uint8_t dummy_read;

    /* Standby control register 9 (STBCR9)
     b1      ------0-;  MSTP91 : 0 : Video display controller channel 0 enable */
    reg_data = (uint32_t) CPG.STBCR9 & (uint32_t) ~STP91_BIT;
    CPG.STBCR9 = (uint8_t) reg_data;

    /* In order to reflect the change, a dummy read should be done. */
    dummy_read = CPG.STBCR9;

    /* Standby Request Register 2 (STBREQ2)
     b5      --0-----;  STBRQ25 : The standby request to VDC5 channel 0 is invalid. */
    reg_data = (uint32_t) CPG.STBREQ2 & (uint32_t) ~STBRQ25_BIT;
    CPG.STBREQ2 = (uint8_t) reg_data;

    /* Standby Acknowledge Register 2 (STBACK2)
     b5      --*-----;  STBAK25 : Standby acknowledgement from VDC5 channel 0. */
    while (((uint32_t) CPG.STBACK2 & (uint32_t) STBAK25_BIT) != 0u)
    {
        /* Wait for the STBAK25 to be cleared to 0. */
    }

    R_LCD_SetLcdPanel(VDC5_CHANNEL_0);
}

/*****************************************************************************
 End of function init_func
 ******************************************************************************/

/*****************************************************************************
 Function Name: quit_func
 Description:   User-defined function within R_VDC5_Terminate
 Parameters:    user_num - unused
 Return value:  None
 *****************************************************************************/
static void quit_func (const uint32_t user_num)
{
    uint32_t reg_data;
    volatile uint8_t dummy_read;

    /* Standby Request Register 2 (STBREQ2)
     b5      --1-----;  STBRQ25 : The standby request to VDC5 channel 0 is valid. */
    reg_data = (uint32_t) CPG.STBREQ2 | (uint32_t) STBRQ25_BIT;
    CPG.STBREQ2 = (uint8_t) reg_data;

    /* Standby Acknowledge Register 2 (STBACK2)
     b5      --*-----;  STBAK25 : Standby acknowledgement from VDC5 channel 0. */
    while (((uint32_t) CPG.STBACK2 & (uint32_t) STBAK25_BIT) == 0u)
    {
        /* Wait for the STBAK25 to be set to 1. */
    }

    /* Standby control register 9 (STBCR9)
     b1      ------1-;  MSTP91 : 1 : Video display controller channel 0 & LVDS disable */
    reg_data = (uint32_t) CPG.STBCR9 | (uint32_t) STP91_BIT;
    CPG.STBCR9 = (uint8_t) reg_data;

    /* In order to reflect the change, a dummy read should be done. */
    dummy_read = CPG.STBCR9;
} /* End of function quitt_func() */

/*****************************************************************************
 End of function  quit_func
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

