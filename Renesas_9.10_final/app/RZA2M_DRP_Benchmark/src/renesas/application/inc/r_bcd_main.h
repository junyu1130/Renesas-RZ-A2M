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
#ifndef R_BCD_MAIN_H
#define R_BCD_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdbool.h>

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
	IPP_FPS,
	IPP_BAYER_2_GRAY_TOTAL,
	IPP_BAYER_2_GRAY_DL,
	IPP_BAYER_2_GRAY_EXE,

	IPP_BINARIZATION_FIXED_TOTAL,
	IPP_BINARIZATION_FIXED_DL,
	IPP_BINARIZATION_FIXED_EXE,

	IPP_RESIZE_BILINEAR_TOTAL,
	IPP_RESIZE_BILINEAR_DL,
	IPP_RESIZE_BILINEAR_EXE,

	IPP_CAMERA_FRAMERATE,
	//IPP_BINARIZATION_CV,
	IPP_BINARIZATION_Ostu,
	IPP_CPU_COPY2RESULT_MEM,
	IPP_CPU_INFERENCE,
	IPP_TOTAL,
	IPP_INVALID,
	IPP_Test_1,
	IPP_Test_2,
	IPP_Test_3,
	IPP_Test_4
}R_IMG_PROCESS_PROFILING;


typedef struct
{
	uint16_t x;
	uint16_t y;
}R_POINT;


/******************************************************************************
Macro definitions
******************************************************************************/


#define use_bcd (0)
#define use_deeplearning (1)
#define use_auto_exposure (0)
#define use_send_data (0)
#define use_NRF (1)

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
void sample_main(void);

void R_BCD_MainBinarization2(uint32_t in_adr, uint32_t out_adr, uint32_t width, uint32_t height);
bool R_BCD_MainReedsolomon(int8_t * codewordBytes, uint8_t numCodewords, uint8_t numECCodewords);
void R_BCD_MainSetReedsolomonTime(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif  /* R_BCD_MAIN_H */

/* End of File */
