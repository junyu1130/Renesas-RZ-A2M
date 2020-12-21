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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "r_typedefs.h"
#include "r_bsp.h"
#include "perform.h"
#include "main.h"
#include "r_bcd_camera.h"
#include "r_bcd_lcd.h"
#include "r_drp.h"

/**********************************************************************************************************************
Imported global variables and functions (from other files)
**********************************************************************************************************************/
extern uint8_t g_drp_lib_bayer2grayscale[];
extern uint8_t g_drp_lib_binarization_fixed[];
extern uint8_t g_drp_lib_resize_bilinear[];
extern uint8_t g_drp_lib_resize_nearest[];

static uint8_t drp_lib_id[R_DK2_TILE_NUM] = {0};
static volatile uint8_t drp_lib_status[R_DK2_TILE_NUM] = {DRP_NOT_FINISH};

static r_drp_bayer2grayscale_t 			param_b2g[R_DK2_TILE_NUM] __attribute__ ((section("Uncache_IRAM")));
static r_drp_binarization_fixed_t		param_binarization_fixed[R_DK2_TILE_NUM] __attribute__ ((section("Uncache_IRAM")));
static r_drp_resize_bilinear_t			param_resize_bilinear  __attribute__ ((section("Uncache_IRAM")));
static r_drp_resize_nearest_t			param_resize_nearest  __attribute__ ((section("Uncache_IRAM")));
static void cb_drp_finish(uint8_t id);


/**********************************************************************************************************************
* Function Name: cb_drp_finish
* Description  : This function is a callback function called from the
*              : DRP driver at the finish of the DRP library processing.
* Arguments    : id
*              :   The ID of the DRP library that finished processing.
* Return Value : -
**********************************************************************************************************************/
static void cb_drp_finish(uint8_t id)
{
    uint32_t tile_no;

    /* Change the operation state of the DRP library notified by the argument to finish */
    for (tile_no = 0; tile_no < R_DK2_TILE_NUM; tile_no++)
    {
        if (drp_lib_id[tile_no] == id)
        {
            drp_lib_status[tile_no] = DRP_FINISH;
            break;
        }
    }

    return;
}


int32_t r_drp_bayer2grayscale(uint32_t src, uint32_t dst, uint16_t width, uint16_t height)
{
	int32_t ret_val;
	uint32_t tile_no;

    /******************************/
    /* Load DRP Library           */
    /*        +-----------------+ */
    /* tile 0 | Bayer2Grayscale | */
    /*        +-----------------+ */
    /* tile 1 | Bayer2Grayscale | */
    /*        +-----------------+ */
    /* tile 2 | Bayer2Grayscale | */
    /*        +-----------------+ */
    /* tile 3 | Bayer2Grayscale | */
    /*        +-----------------+ */
    /* tile 4 | Bayer2Grayscale | */
    /*        +-----------------+ */
    /* tile 5 | Bayer2Grayscale | */
    /*        +-----------------+ */
    /******************************/

	ret_val = R_DK2_Load(&g_drp_lib_bayer2grayscale[0],
	                            R_DK2_TILE_0 | R_DK2_TILE_1 | R_DK2_TILE_2 | R_DK2_TILE_3 | R_DK2_TILE_4 | R_DK2_TILE_5,
	                            R_DK2_TILE_PATTERN_1_1_1_1_1_1, NULL, &cb_drp_finish, &drp_lib_id[0]);
	DRP_DRV_ASSERT(ret_val);

	ret_val = R_DK2_Activate(drp_lib_id[TILE_0] | drp_lib_id[TILE_1] | drp_lib_id[TILE_2] | drp_lib_id[TILE_3]
	                         | drp_lib_id[TILE_4] | drp_lib_id[TILE_5], 0);
	DRP_DRV_ASSERT(ret_val);

	for (tile_no = 0; tile_no < R_DK2_TILE_NUM; tile_no++)
	{
		/* Set the address of buffer to be read/write by DRP */
		param_b2g[tile_no].src = src + ((R_BCD_CAMERA_WIDTH * (R_BCD_CAMERA_HEIGHT / R_DK2_TILE_NUM)) * tile_no);
		param_b2g[tile_no].dst = dst + ((R_BCD_CAMERA_WIDTH * (R_BCD_CAMERA_HEIGHT / R_DK2_TILE_NUM)) * tile_no);

		/* Set Image size */
		param_b2g[tile_no].width = R_BCD_CAMERA_WIDTH;
		param_b2g[tile_no].height = R_BCD_CAMERA_HEIGHT / R_DK2_TILE_NUM;

		/* Set whether to perform top or bottom edge border processing. */
		param_b2g[tile_no].top = (tile_no == TILE_0) ? 1 : 0;
		param_b2g[tile_no].bottom = (tile_no == TILE_5) ? 1 : 0;

		/* Initialize variables to be used in termination judgment of the DRP library */
		drp_lib_status[tile_no] = DRP_NOT_FINISH;

		/*********************/
		/* Start DRP Library */
		/*********************/
		ret_val = R_DK2_Start(drp_lib_id[tile_no], (void *)&param_b2g[tile_no], sizeof(r_drp_bayer2grayscale_t));
		DRP_DRV_ASSERT(ret_val);
	}

	/***************************************/
	/* Wait until DRP processing is finish */
	/***************************************/
	for (tile_no = 0; tile_no < R_DK2_TILE_NUM; tile_no++)
	{
		while (drp_lib_status[tile_no] == DRP_NOT_FINISH)
		{
			/* DO NOTHING */
		}
	}

	ret_val = R_DK2_Unload(drp_lib_id[TILE_0] | drp_lib_id[TILE_1] | drp_lib_id[TILE_2] | drp_lib_id[TILE_3]
	                       | drp_lib_id[TILE_4] | drp_lib_id[TILE_5], &drp_lib_id[0]);
	DRP_DRV_ASSERT(ret_val);

	return ret_val;
}


/******************************************************************************
 /* Function Name: r_drp_binarization_fixed
 * Description  : This function converts from RGB to Value of HSV on DRP
 * Arguments    : input_address
 * 					Address of input image
 * 				 output_address
 * 				 	Address of output image
 * 				 input_width
 * 				 	Horizontal size of input data image
 * 				 input_height
 * 				 	Vertical size of input data image
 * 				 threshold
 * 				    Threshold of Binarization
 * Return Value : -
 ******************************************************************************/
int32_t r_drp_binarization_fixed(uint32_t input_address, uint32_t output_address, uint16_t input_width, uint16_t input_height, uint8_t threshold)
{
	int32_t ret_val;
	uint32_t tile_no;

	PerformSetStartTime(IPP_RESIZE_BILINEAR_DL);
	/******************************/
	/* Load DRP Library           */
	/*        +-----------------+ */
	/* tile 0 | Binarization    | */
	/*        +-----------------+ */
	/* tile 1 | Binarization    | */
	/*        +-----------------+ */
	/* tile 2 | Binarization    | */
	/*        +-----------------+ */
	/* tile 3 | Binarization    | */
	/*        +-----------------+ */
	/* tile 4 | Binarization    | */
	/*        +-----------------+ */
	/* tile 5 | Binarization    | */
	/*        +-----------------+ */
	/******************************/
	ret_val = R_DK2_Load(&g_drp_lib_binarization_fixed[0],
			R_DK2_TILE_0 | R_DK2_TILE_1 | R_DK2_TILE_2 | R_DK2_TILE_3 | R_DK2_TILE_4 | R_DK2_TILE_5,
			R_DK2_TILE_PATTERN_1_1_1_1_1_1, NULL, &cb_drp_finish, &drp_lib_id[0]);
	//DRP_DRV_ASSERT(ret_val);

	/************************/
	/* Activate DRP Library */
	/************************/
	ret_val = R_DK2_Activate(drp_lib_id[TILE_0] | drp_lib_id[TILE_1] | drp_lib_id[TILE_2] | drp_lib_id[TILE_3] | drp_lib_id[TILE_4] | drp_lib_id[TILE_5], 0);
	//DRP_DRV_ASSERT(ret_val);

	/* Set end time of process(DL)*/
	PerformSetEndTime(IPP_RESIZE_BILINEAR_DL);
	/* Set start time of process*/
	PerformSetStartTime(IPP_BINARIZATION_FIXED_EXE);

	/***************************************/
	/* Set R_DK2_Start function parameters */
	/***************************************/
	for (tile_no = 0; tile_no < R_DK2_TILE_NUM; tile_no++)
	{
		/* Set the address of buffer to be read/write by DRP */
		param_binarization_fixed[tile_no].src = input_address  + (input_width * (input_height / R_DK2_TILE_NUM)) * tile_no;
		param_binarization_fixed[tile_no].dst = output_address + (input_width * (input_height / R_DK2_TILE_NUM)) * tile_no;

		/* Set Image size */
		param_binarization_fixed[tile_no].width  = (uint16_t)input_width;
		param_binarization_fixed[tile_no].height = (uint16_t)input_height / R_DK2_TILE_NUM;

		param_binarization_fixed[tile_no].threshold = threshold;

		/* Initialize variables to be used in termination judgment of the DRP application */
		drp_lib_status[tile_no] = DRP_NOT_FINISH;

		/*********************/
		/* Start DRP Library */
		/*********************/
		ret_val = R_DK2_Start(drp_lib_id[tile_no], (void *)&param_binarization_fixed[tile_no], sizeof(r_drp_binarization_fixed_t));
		//DRP_DRV_ASSERT(ret_val);
	}

	/***************************************/
	/* Wait until DRP processing is finish */
	/***************************************/
	for (tile_no = 0; tile_no < R_DK2_TILE_NUM; tile_no++)
	{
		while (drp_lib_status[tile_no] == DRP_NOT_FINISH);
	}

	/* Set end time of process */
	PerformSetEndTime(IPP_BINARIZATION_FIXED_EXE);

	/***************************************/
	/* Unload DRP Library                  */
	/***************************************/
	ret_val = R_DK2_Unload(drp_lib_id[TILE_0] | drp_lib_id[TILE_1] | drp_lib_id[TILE_2]
						| drp_lib_id[TILE_3] | drp_lib_id[TILE_4] | drp_lib_id[TILE_5], &drp_lib_id[0]);
	//DRP_DRV_ASSERT(ret_val);

	return ret_val;
}

/******************************************************************************
 /* Function Name: r_drp_resize_bilinear
 * Description  : This function resize 8bit gray scale image
 * Arguments    : input_address
 * 					Address of input image
 * 				 output_address
 * 				 	Address of output image
 * 				 input_width
 * 				 	Horizontal size of input data image
 * 				 input_height
 * 				 	Vertical size of input data image
 * 				 output_width
 * 				 	Horizontal size of output data image
 * 				 output_height
 * 				 	Vertical size of output data image
 * Return Value : -
 ******************************************************************************/
int32_t r_drp_resize_bilinear(uint32_t input_address, uint32_t output_address, uint16_t input_width, uint16_t input_height, uint16_t output_width, uint16_t output_height)
{
	int32_t ret_val;

	PerformSetStartTime(IPP_BINARIZATION_FIXED_DL);
	/******************************/
	/* Load DRP Library           */
	/*        +-----------------+ */
	/* tile 0 | resize bilinear | */
	/*        +                 + */
	/* tile 1 |                 | */
	/*        +                 + */
	/* tile 2 |                 | */
	/*        +                 + */
	/* tile 3 |                 | */
	/*        +                 + */
	/* tile 4 |                 | */
	/*        +                 + */
	/* tile 5 |                 | */
	/*        +-----------------+ */
	/******************************/
	ret_val = R_DK2_Load(&g_drp_lib_resize_bilinear[0],
			R_DK2_TILE_0 ,	R_DK2_TILE_PATTERN_6, NULL, &cb_drp_finish, &drp_lib_id[0]);
	//DRP_DRV_ASSERT(ret_val);

	/************************/
	/* Activate DRP Library */
	/************************/
	ret_val = R_DK2_Activate(drp_lib_id[TILE_0], 0);
	//DRP_DRV_ASSERT(ret_val);

	/* Set end time of process(DL)*/
	PerformSetEndTime(IPP_BINARIZATION_FIXED_DL);
	/* Set start time of process*/
	PerformSetStartTime(IPP_RESIZE_BILINEAR_EXE);

	/***************************************/
	/* Set R_DK2_Start function parameters */
	/***************************************/
	/* Set the address of buffer to be read/write by DRP */
	param_resize_bilinear.src = input_address;
	param_resize_bilinear.dst = output_address;

	/* Set Image size */
	param_resize_bilinear.src_width  = (uint16_t)input_width;
	param_resize_bilinear.src_height = (uint16_t)input_height;

	param_resize_bilinear.dst_width = output_width;
	param_resize_bilinear.dst_height = output_height;

	/* Initialize variables to be used in termination judgment of the DRP application */
	drp_lib_status[0] = DRP_NOT_FINISH;

	/*********************/
	/* Start DRP Library */
	/*********************/
	ret_val = R_DK2_Start(drp_lib_id[0], (void *)&param_resize_bilinear, sizeof(r_drp_resize_bilinear_t));
	//DRP_DRV_ASSERT(ret_val);

	/***************************************/
	/* Wait until DRP processing is finish */
	/***************************************/
	while (drp_lib_status[0] == DRP_NOT_FINISH);

	/* Set end time of process */
	PerformSetEndTime(IPP_RESIZE_BILINEAR_EXE);

	/***************************************/
	/* Unload DRP Library                  */
	/***************************************/
	ret_val = R_DK2_Unload(drp_lib_id[TILE_0], &drp_lib_id[0]);
	//DRP_DRV_ASSERT(ret_val);

	return ret_val;
}


int32_t r_drp_resize_nearest(uint32_t input_address, uint32_t output_address, uint16_t input_width, uint16_t input_height, uint16_t output_width, uint16_t output_height)
{
	int32_t ret_val;

	PerformSetStartTime(IPP_BINARIZATION_FIXED_DL);
	/******************************/
	/* Load DRP Library           */
	/*        +-----------------+ */
	/* tile 0 | resize nearest | */
	/*        +                 + */
	/* tile 1 |                 | */
	/*        +                 + */
	/* tile 2 |                 | */
	/*        +                 + */
	/* tile 3 |                 | */
	/*        +                 + */
	/* tile 4 |                 | */
	/*        +                 + */
	/* tile 5 |                 | */
	/*        +-----------------+ */
	/******************************/
	ret_val = R_DK2_Load(&g_drp_lib_resize_nearest[0],
			R_DK2_TILE_0 ,	R_DK2_TILE_PATTERN_6, NULL, &cb_drp_finish, &drp_lib_id[0]);
	//DRP_DRV_ASSERT(ret_val);

	/************************/
	/* Activate DRP Library */
	/************************/
	ret_val = R_DK2_Activate(drp_lib_id[TILE_0], 0);
	//DRP_DRV_ASSERT(ret_val);

	/* Set end time of process(DL)*/
	PerformSetEndTime(IPP_BINARIZATION_FIXED_DL);
	/* Set start time of process*/
	PerformSetStartTime(IPP_RESIZE_BILINEAR_EXE);

	/***************************************/
	/* Set R_DK2_Start function parameters */
	/***************************************/
	/* Set the address of buffer to be read/write by DRP */
	param_resize_nearest.src = input_address;
	param_resize_nearest.dst = output_address;

	/* Set Image size */
	param_resize_nearest.src_width  = (uint16_t)input_width;
	param_resize_nearest.src_height = (uint16_t)input_height;

	param_resize_nearest.dst_width = output_width;
	param_resize_nearest.dst_height = output_height;

	/* Initialize variables to be used in termination judgment of the DRP application */
	drp_lib_status[0] = DRP_NOT_FINISH;

	/*********************/
	/* Start DRP Library */
	/*********************/
	ret_val = R_DK2_Start(drp_lib_id[0], (void *)&param_resize_nearest, sizeof(r_drp_resize_nearest_t));
	//DRP_DRV_ASSERT(ret_val);

	/***************************************/
	/* Wait until DRP processing is finish */
	/***************************************/
	while (drp_lib_status[0] == DRP_NOT_FINISH);

	/* Set end time of process */
	PerformSetEndTime(IPP_RESIZE_BILINEAR_EXE);

	/***************************************/
	/* Unload DRP Library                  */
	/***************************************/
	ret_val = R_DK2_Unload(drp_lib_id[TILE_0], &drp_lib_id[0]);
	//DRP_DRV_ASSERT(ret_val);

	return ret_val;
}
