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
* http://www.renesas.com/disclaimer*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name : r_bsp.h
* $Rev: 29 $
* $Date:: 2018-03-13 $
* Description : bsp header file
******************************************************************************/

#ifndef INC_R_BSP_H_
#define INC_R_BSP_H_

#ifdef __cplusplus
	extern "C" {
#endif


#include "mcu_board_select.h"
#include "r_typedefs.h"

#if defined (_DEVICE_RZA1_)
#include "../src/rza1/generate/system/inc/iodefine.h"
#include "../src/rza1/generate/system/inc/iobitmask.h"
#include "../src/rza1/generate/system/inc/dev_drv.h"
#include "../src/rza1/generate/system/inc/rza_io_regrw.h"
#include "../src/rza1/generate/system/inc/irq.h"

#include "../src/rza1/driver/adc/r_adc.h"
#include "../src/rza1/driver/intc/inc/devdrv_intc.h"
#include "../src/rza1/driver/intc/inc/intc_handler.h"
#include "../src/rza1/driver/ostm/inc/devdrv_ostm.h"
#include "../src/rza1/driver/scif_uart/inc/devdrv_scif_uart.h"
#include "../src/rza1/driver/ceu/r_ceu_interrupt.h"
#include "../src/rza1/driver/ceu/r_ceu_user_config.h"
#include "../src/rza1/driver/ceu/r_ceu.h"
#include "../src/rza1/driver/jcu/r_jcu.h"
#include "../src/rza1/driver/mtu2/r_mtu2.h"
#include "../src/rza1/driver/riic/inc/r_riic_header.h"
#include "../src/rza1/driver/riic/inc/r_riic_stream_it.h"
#include "../src/rza1/driver/riic/inc/riic_userdef.h"
#include "../src/rza1/driver/vdc5/inc/r_vdc5.h"
#include "../src/rza1/driver/vdc5/inc/r_vdc5_user.h"

#include "./external/r_touch_capacitive.h"
#include "./external/r_touch_driver.h"
#include "./external/r_camera_auto_detect.h"
#include "./external/r_camera_api.h"
#include "./external/r_camera_ceu.h"
#include "./external/r_camera_ov7670.h"
#include "./external/r_camera_ov7740.h"
#include "./external/r_graphics_drv_wrapper.h"
#include "./external/r_lcd_panel.h"
#include "./external/r_led_drive.h"
#include "./external/r_switch_driver.h"
#include "./external/r_touch_capacitive.h"
#include "./external/r_touch_driver.h"
#elif defined (_DEVICE_RZA2_)
#include "../src/rza2/generate/system/inc/iodefine.h"
#include "../src/rza2/generate/system/inc/iobitmask.h"
#include "../src/rza2/generate/system/inc/driver.h"
#include "../src/rza2/generate/system/inc/devlink.h"
#include "../src/rza2/generate/system/inc/r_devlink_wrapper.h"
#include "../src/rza2/generate/system/inc/rza_io_regrw.h"
#include "../src/rza2/generate/drivers/r_cache/inc/r_cache_l1_rza2m_asm.h"
#include "../src/rza2/generate/drivers/r_cache/inc/r_cache_lld_rza2m.h"
#include "../src/rza2/generate/drivers/r_cpg/inc/r_cpg_drv_api.h"
#include "../src/rza2/generate/drivers/r_cpg/inc/r_cpg_drv_sc_cfg.h"
#include "../src/rza2/generate/drivers/r_cpg/inc/r_cpg_hld_api.h"
#include "../src/rza2/generate/drivers/r_gpio/inc/r_gpio_drv_api.h"
#include "../src/rza2/generate/drivers/r_gpio/inc/r_gpio_drv_sc_cfg.h"
#include "../src/rza2/generate/drivers/r_intc/inc/r_intc_drv_api.h"
#include "../src/rza2/generate/drivers/r_mmu/inc/r_mmu_drv_api.h"
#include "../src/rza2/generate/drivers/r_mmu/inc/r_mmu_drv_sc_cfg.h"
#include "../src/rza2/generate/drivers/r_stb/inc/r_stb_drv_api.h"
#include "../src/rza2/generate/drivers/r_bsc/inc/r_bsc_drv_api.h"
#include "../src/rza2/generate/drivers/r_hyper/inc/r_hyper_drv_api.h"
#include "../src/rza2/generate/sc_drivers/r_ceu/inc/r_ceu.h"
#include "../src/rza2/generate/sc_drivers/r_ceu/inc/r_ceu_user.h"
#include "../src/rza2/generate/sc_drivers/r_drp/inc/r_dk2_if.h"
#include "../src/rza2/generate/sc_drivers/r_jcu/inc/r_jcu.h"
#include "../src/rza2/generate/sc_drivers/r_mipi/inc/r_mipi_api.h"
#include "../src/rza2/generate/sc_drivers/r_mipi/inc/r_mipi_user.h"
#include "../src/rza2/generate/sc_drivers/r_riic/inc/r_riic_drv_api.h"
#include "../src/rza2/generate/sc_drivers/r_rvapi/inc/r_rvapi_ceu.h"
#include "../src/rza2/generate/sc_drivers/r_rvapi/inc/r_rvapi_mipi.h"
#include "../src/rza2/generate/sc_drivers/r_rvapi/inc/r_rvapi_vdc.h"
#include "../src/rza2/generate/sc_drivers/r_rvapi/inc/r_rvapi_spea.h"
#include "../src/rza2/generate/sc_drivers/r_scifa/inc/r_scifa_drv_api.h"
#include "../src/rza2/generate/sc_drivers/r_scifa/inc/r_scifa_drv_sc_cfg.h"
#include "../src/rza2/generate/sc_drivers/r_scifa/inc/r_scifa_lld_rza2m_api.h"
#include "../src/rza2/generate/sc_drivers/r_vdc/inc/r_vdc.h"
#include "../src/rza2/generate/sc_drivers/r_vdc/inc/r_vdc_user.h"
#include "../src/rza2/generate/sc_drivers/r_vdc/inc/r_spea.h"
#include "../src/rza2/generate/sc_drivers/r_vdc/inc/r_spea_user.h"
#include "../src/rza2/generate/sc_drivers/r_sdhi_simplified/inc/r_sd_cfg.h"
#include "../src/rza2/generate/sc_drivers/r_sdhi_simplified/inc/r_sdhi_simplified_drv_sc_cfg.h"
#include "../src/rza2/generate/sc_drivers/r_sdhi_simplified/inc/r_sdif.h"

#include "../lib/drp/r_drp_argb2grayscale.h"
#include "../lib/drp/r_drp_bayer2grayscale.h"
#include "../lib/drp/r_drp_binarization_adaptive.h"
#include "../lib/drp/r_drp_binarization_adaptive_bit.h"
#include "../lib/drp/r_drp_binarization_fixed.h"
#include "../lib/drp/r_drp_canny_calculate.h"
#include "../lib/drp/r_drp_canny_hysterisis.h"
#include "../lib/drp/r_drp_circle_fitting.h"
#include "../lib/drp/r_drp_corner_harris.h"
#include "../lib/drp/r_drp_cropping.h"
#include "../lib/drp/r_drp_dilate.h"
#include "../lib/drp/r_drp_erode.h"
#include "../lib/drp/r_drp_gamma_correction.h"
#include "../lib/drp/r_drp_gaussian_blur.h"
#include "../lib/drp/r_drp_histogram.h"
#include "../lib/drp/r_drp_median_blur.h"
#include "../lib/drp/r_drp_prewitt.h"
#include "../lib/drp/r_drp_reed_solomon.h"
#include "../lib/drp/r_drp_resize_bilinear.h"
#include "../lib/drp/r_drp_resize_bilinear_fixed.h"
#include "../lib/drp/r_drp_resize_nearest.h"
#include "../lib/drp/r_drp_sobel.h"
#include "../lib/drp/r_drp_unsharp_masking.h"
#include "../lib/drp/r_drp_absdiff.h"
#include "../lib/drp/r_drp_moments_lite.h"
#include "../lib/drp/r_drp_bayer2hsv_inrange.h"
#include "../lib/drp/r_drp_bitwiseand.h"
#include "../lib/drp/r_drp_simple_isp_gesture.h"
#include "../lib/drp/r_drp_bayer2rgb_thinning.h"
#include "../lib/drp/r_drp_shading_wb_corr.h"
#include "../lib/drp/r_drp_rgb2value.h"
#include "../lib/drp/r_drp_opening_closing.h"
#include "../lib/drp/r_drp_affine.h"
#include "../lib/drp/r_drp_rgb_gain_bias.h"
#include "../lib/drp/r_drp_rgb2gray.h"
#include "../lib/drp/r_drp_simple_isp_face_detection.h"
#include "../lib/drp/r_drp_simple_isp_2_tiles.h"
#include "../lib/drp/r_drp_bayer2gray_thinning.h"
#include "../lib/drp/r_drp_find_contours.h"
#include "../lib/drp/r_drp_bayer2gray_cropping.h"
#include "../lib/drp/r_drp_keystone_correction.h"
#include "../lib/drp/r_drp_elbp.h"
#include "../lib/drp/r_drp_histogram_spatial.h"
#include "../lib/drp/r_drp_histogram_compare.h"

#endif //#if defined (TARGET_RZA1)


#include "trace.h"

#include "../os/inc/os_port.h"

#if defined (_DEVICE_RZA2_)
#include "../src/rza2/generate/os_abstraction/inc/r_task_priority.h"
#include "../src/rza2/generate/os_abstraction/inc/r_os_abstraction_api.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* INC_R_BSP_H_ */
