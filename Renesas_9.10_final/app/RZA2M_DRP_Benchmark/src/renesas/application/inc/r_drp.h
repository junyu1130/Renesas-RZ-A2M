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

#ifndef RENESAS_APPLICATION_INC_R_DRP_H_
#define RENESAS_APPLICATION_INC_R_DRP_H_

#define TILE_0            (0)
#define TILE_1            (1)
#define TILE_2            (2)
#define TILE_3            (3)
#define TILE_4            (4)
#define TILE_5            (5)

#define DRP_NOT_FINISH    (0)
#define DRP_FINISH        (1)
#define DRP_DRV_ASSERT(x) if ((x) != 0) while(1)

int32_t r_drp_bayer2grayscale(uint32_t src, uint32_t dst, uint16_t width, uint16_t height);
int32_t r_drp_binarization_fixed(uint32_t input_address, uint32_t output_address, uint16_t input_width, uint16_t input_height, uint8_t threshold);
int32_t r_drp_resize_bilinear(uint32_t input_address, uint32_t output_address, uint16_t input_width, uint16_t input_height, uint16_t output_width, uint16_t output_height);

#endif /* RENESAS_APPLICATION_INC_R_DRP_H_ */
