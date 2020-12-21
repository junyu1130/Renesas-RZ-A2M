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
* Copyright (C) 2012 - 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File: r_jcu.h
*    JPEG Codec Unit (JCU) driver's API. Main Header.
*
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\24_RZA2_en $
* - $Rev: $
* - $Date: 2018-11-26T19:24:28+09:00 $
******************************************************************************/

#ifndef  R_JCU_API_H
#define  R_JCU_API_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stddef.h>
#include  "r_typedefs.h"
#include  "r_jcu_typedef.h"
#include  "rz_co_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Struct & Enum definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
jcu_errorcode_t  R_JCU_Initialize( jcu_config_t*  in_out_Config );
#ifdef  JCU_CALLBACK_ARGUMENT
jcu_errorcode_t  R_JCU_TerminateAsync( r_co_function_t  in_OnFinalized,  volatile void*  in_OnFinalizedArgument );
#endif
jcu_errorcode_t  R_JCU_SelectCodec( const jcu_codec_t  in_Codec );
#ifdef  JCU_CALLBACK_ARGUMENT
jcu_errorcode_t  R_JCU_StartAsync( r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument );
jcu_errorcode_t  R_JCU_GetAsyncError(void);
jcu_errorcode_t  R_JCU_ContinueAsync( const jcu_continue_type_t  in_Type,  r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument );
#endif
jcu_errorcode_t  R_JCU_SetDecodeParam( const jcu_decode_param_t*const  in_Decode,  const jcu_buffer_param_t*const  in_Buffer );
jcu_errorcode_t  R_JCU_SetPauseForImageInfo( const bool_t  in_IsPause );
jcu_errorcode_t  R_JCU_GetImageInfo( jcu_image_info_t* const  out_Buffer );
jcu_errorcode_t  R_JCU_SetErrorFilter( jcu_int_detail_errors_t  in_Filter );
jcu_errorcode_t  R_JCU_SetQuantizationTable( const jcu_table_no_t  in_TableNum,  const uint8_t* const  in_Table );
jcu_errorcode_t  R_JCU_SetHuffmanTable( const jcu_table_no_t  in_TableNum,  const jcu_huff_t  in_Type,  const uint8_t* const  in_Table );
jcu_errorcode_t  R_JCU_SetEncodeParam( const jcu_encode_param_t* const  in_Encode,  const jcu_buffer_param_t* const  in_Buffer );
jcu_errorcode_t  R_JCU_GetEncodedSize( size_t* const  out_Size );

errnum_t  R_JCU_OnInterrupting(void);

void  R_JCU_PrintRegisters(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* #define  R_JCU_API_H */
