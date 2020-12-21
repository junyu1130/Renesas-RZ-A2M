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
/*******************************************************************************
* File: jcu_user.c
*    JPEG Codec Unit (JCU)  driver. User defined funcions.
*
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\24_RZA2_en $
* - $Rev: $
* - $Date: 2018-11-26T19:24:28+09:00 $
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stdio.h>
#include  "r_typedefs.h"
#include  "iodefine.h"
#include  "iobitmask.h"
#include  "r_register_bit.h"
#include  "rz_co.h"
#include  "r_jcu.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Defines: Macros in this file scope. Global static defines (GS)
************************************************************************/
#define  GS_N  "\r\n"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/


/**************************************************************************//**
* Function Name: [R_JCU_PrintRegisters]
* @brief
* @retval        None
******************************************************************************/
void  R_JCU_PrintRegisters()
{
	volatile uint8_t*  array;
	const uint8_t   no_shift = IOREG_NONSHIFT_ACCESS;
	const uint32_t  no_mask  = IOREG_NONMASK_ACCESS;

	#ifdef __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wformat="
	#endif

	printf( ""GS_N );
	printf( "JCU Registers"GS_N );
	printf( "JCMOD  = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCMOD  ) );
	printf( "JCCMD  = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCCMD  ) );
	printf( "JCQTN  = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCQTN  ) );
	printf( "JCHTN  = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCHTN  ) );
	printf( "JCDRIU = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCDRIU ) );
	printf( "JCDRID = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCDRID ) );
	printf( "JCVSZU = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCVSZU ) );
	printf( "JCVSZD = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCVSZD ) );
	printf( "JCHSZU = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCHSZU ) );
	printf( "JCHSZD = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCHSZD ) );
	printf( "JCDTCU = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCDTCU ) );
	printf( "JCDTCM = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCDTCM ) );
	printf( "JCDTCD = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCDTCD ) );
	printf( "JINTE0 = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JINTE0 ) );
	printf( "JINTS0 = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JINTS0 ) );
	printf( "JCDERR = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCDERR ) );
	printf( "JCRST  = 0x%02X"GS_N,  R_REGISTER_READ8( &JCU.JCRST  ) );
	printf( "JIFECNT   = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFECNT   ) );
	printf( "JIFESA    = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFESA    ) );
	printf( "JIFESOFST = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFESOFST ) );
	printf( "JIFEDA    = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFEDA    ) );
	printf( "JIFESLC   = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFESLC   ) );
	printf( "JIFEDDC   = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFEDDC   ) );
	printf( "JIFDCNT   = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFDCNT   ) );
	printf( "JIFDSA    = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFDSA    ) );
	printf( "JIFDDOFST = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFDDOFST ) );
	printf( "JIFDDA    = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFDDA    ) );
	printf( "JIFDSDC   = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFDSDC   ) );
	printf( "JIFDDLC   = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFDDLC   ) );
	printf( "JIFDADT   = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFDADT   ) );
	printf( "JINTE1    = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JINTE1    ) );
	printf( "JINTS1    = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JINTS1    ) );
	printf( "JIFESVSZ  = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFESVSZ  ) );
	printf( "JIFESHSZ  = 0x%08X"GS_N,  R_REGISTER_READ32( &JCU.JIFESHSZ  ) );

	array = &JCU.JCQTBL0;
	printf( "JCQTBL0[0] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[0],  no_shift,  no_mask ) );
	printf( "JCQTBL0[1] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[1],  no_shift,  no_mask ) );
	printf( "JCQTBL0[2] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[2],  no_shift,  no_mask ) );
	printf( "        :"GS_N );

	array = &JCU.JCQTBL1;
	printf( "JCQTBL1[0] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[0],  no_shift,  no_mask ) );
	printf( "JCQTBL1[1] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[1],  no_shift,  no_mask ) );
	printf( "        :"GS_N );

	array = &JCU.JCQTBL2;
	printf( "JCQTBL2[0] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[0],  no_shift,  no_mask ) );
	printf( "        :"GS_N );

	array = &JCU.JCQTBL3;
	printf( "JCQTBL3[0] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[0],  no_shift,  no_mask ) );
	printf( "        :"GS_N );

	array = &JCU.JCHTBD0;
	printf( "JCHTBD0[0] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[0],  no_shift,  no_mask ) );
	printf( "        :"GS_N );

	array = &JCU.JCHTBA0;
	printf( "JCHTBA0[0] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[0],  no_shift,  no_mask ) );
	printf( "        :"GS_N );

	array = &JCU.JCHTBD1;
	printf( "JCHTBD1[0] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[0],  no_shift,  no_mask ) );
	printf( "        :"GS_N );

	array = &JCU.JCHTBA1;
	printf( "JCHTBA1[0] = 0x%02X"GS_N,  RZA_IO_RegRead_8( &array[0],  no_shift,  no_mask ) );
	printf( "        :"GS_N );

	#ifdef __GNUC__
	#pragma GCC diagnostic pop
	#endif
}

