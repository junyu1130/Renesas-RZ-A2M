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
* File Name    : dnn_compute.c
* Version      : 1.00
* Description  : The function calls
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 16.06.2017 1.00     First Release
***********************************************************************************************************************/

 
#include "layer_shapes.h"
#include "layer_graph.h"
#include "weights.h"
 
TPrecision* dnn_compute(TPrecision* Placeholder_0)
{
  convolution(Placeholder_0,conv_biased_1_0_pad,conv_biased_1_0_weights,conv_biased_1_0_biases,conv_biased_1_0,layer_shapes.conv_biased_1_0_shape);
  relu(conv_biased_1_0,layer_shapes.conv_biased_1_0_activ_shape);
  pooling_without_pad(conv_biased_1_0,pool_2_0,layer_shapes.pool_2_0_shape);
  convolution(pool_2_0,conv_biased_3_0_pad,conv_biased_3_0_weights,conv_biased_3_0_biases,conv_biased_3_0,layer_shapes.conv_biased_3_0_shape);
  relu(conv_biased_3_0,layer_shapes.conv_biased_3_0_activ_shape);
  pooling_without_pad(conv_biased_3_0,pool_4_0,layer_shapes.pool_4_0_shape);
  convolution(pool_4_0,conv_biased_5_0_pad,conv_biased_5_0_weights,conv_biased_5_0_biases,conv_biased_5_0,layer_shapes.conv_biased_5_0_shape);
  relu(conv_biased_5_0,layer_shapes.conv_biased_5_0_activ_shape);
  pooling_without_pad(conv_biased_5_0,Flatten_flatten_Reshape_0,layer_shapes.Flatten_flatten_Reshape_0_shape);
   
  innerproduct(Flatten_flatten_Reshape_0,fully_connected_BiasAdd_0_weights,fully_connected_BiasAdd_0_biases,fully_connected_BiasAdd_0,layer_shapes.fully_connected_BiasAdd_0_shape);
  softmax(fully_connected_BiasAdd_0,layer_shapes.fully_connected_BiasAdd_0_activ_shape);
  return(fully_connected_BiasAdd_0);
}
