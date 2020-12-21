/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED 'AS IS' AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Changed from original python code to C source code.
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : layer_shapes.h
* Version      : 1.00
* Description  : Initializations
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 16.06.2017 1.00     First Release
***********************************************************************************************************************/

#include "Typedef.h"
#ifndef LAYER_SHAPES_H_
#define LAYER_SHAPES_H_
 
TPrecision* dnn_compute(TPrecision*);
 
TPrecision conv_biased_1_0_pad[6724];
TPrecision conv_biased_1_0[25600];
TPrecision pool_2_0[1600];
TPrecision conv_biased_3_0_pad[1936];
TPrecision conv_biased_3_0[3200];
TPrecision pool_4_0[800];
TPrecision conv_biased_5_0_pad[1152];
TPrecision conv_biased_5_0[1600];
TPrecision Flatten_flatten_Reshape_0[400];
TPrecision fully_connected_BiasAdd_0[6];
 
struct shapes{
    TsInt conv_biased_1_0_shape[16];
    TsInt conv_biased_1_0_activ_shape;
    TsInt pool_2_0_shape[11];
    TsInt conv_biased_3_0_shape[16];
    TsInt conv_biased_3_0_activ_shape;
    TsInt pool_4_0_shape[11];
    TsInt conv_biased_5_0_shape[16];
    TsInt conv_biased_5_0_activ_shape;
    TsInt Flatten_flatten_Reshape_0_shape[11];
    TsInt fully_connected_BiasAdd_0_shape[4];
    TsInt fully_connected_BiasAdd_0_activ_shape;
};
 
struct shapes layer_shapes ={
    {1,1,80,80,4,1,3,3,80,80,1,1,1,1,1,1},
    25600,
    {1,4,80,80,20,20,2,2,4,4,0},
    {1,4,20,20,8,4,3,3,20,20,1,1,1,1,1,1},
    3200,
    {1,8,20,20,10,10,2,2,2,2,0},
    {1,8,10,10,16,8,3,3,10,10,1,1,1,1,1,1},
    1600,
    {1,16,10,10,5,5,2,2,2,2,0},
    {1,400,400,6},
    6
};
 
#endif
