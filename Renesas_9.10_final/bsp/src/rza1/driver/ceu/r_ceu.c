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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         r_ceu.c
* @version      0.04
* $Rev: 1739 $
* $Date:: 2016-08-01 17:51:18 +0100#$
* @brief        CEU driver API functions
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "r_ceu_interrupt.h"
#include "r_ceu_user_config.h"
#include "iodefine.h"
#include "gpio_iobitmask.h"
#include "rza_io_regrw.h"
#include "r_ceu.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define STBCR6_INIT_VALUE      (0xBFu)
#define SWRSTCR3_INIT_VALUE    (0xFDu)
#define CAPSR_INIT_VALUE       (0x00010000u)        //(0x00010000u)
#define CEIER_INIT_VALUE       (0x0000000u)
#define CETCR_INIT_VALUE       (0x00000000u)        //(0x00000000u)
#define CRCNTR_INIT_VALUE      (0x0000u)            //(0x0000u)
#define CAPCR_INIT_VALUE       (0x00000000u)        //(0x00000000u)(0x00010000)
#define CMCYR_INIT_VALUE       (0x00000000u)        //(0x00000000u)(0x00780138u)(0x00780620u)
#define CAMOR_A_INIT_VALUE     (0x00000000u)
#define CAPWR_A_INIT_VALUE     (0x00000000u)        //(0x00000000u)(0x00780138u)(0x00780620u)
#define CBDSR_A_INIT_VALUE     (0x00000000u)
#define CAIFR_INIT_VALUE       (0x00000000u)
#define CDWDR_A_INIT_VALUE     (0x00000000u)       //(0x00000000u)(0x00000138u)(0x00000620u)
#define CFLCR_A_INIT_VALUE     (0x00000000u)
#define CFSZR_A_INIT_VALUE     (0x00000000u)       //(0x00000000u)(0x00780620u)
#define CLFCR_A_INIT_VALUE     (0x00000000u)
#define CFWCR_INIT_VALUE       (0x00000000u)

#define CDAYR2_A_INIT_VALUE    (0x00000000u)       //(0x00000000u)(0x20600000u)
#define CDACR2_A_INIT_VALUE    (0x00000000u)       //(0x00000000u)(0x20612D00u)
#define CDBYR2_A_INIT_VALUE    (0x00000000u)       //(0x00000000u)(0x20625A00u)
#define CDBCR2_A_INIT_VALUE    (0x00000000u)       //(0x00000000u)(0x20638700u)

#define CDACR_A_INIT_VALUE     (0x00000000u)       //(0x00000000u)
#define CDBYR_A_INIT_VALUE     (0x00000000u)       //(0x00000000u)
#define CDBCR_A_INIT_VALUE     (0x00000000u)       //(0x00000000u)

#define CSTSR_CPTON_BIT_STATUS (0x00000001u)
#define CAPSR_CPKIL_BIT_STATUS (0x00010000u)


#define CAPSR_CE_BIT_SET       (0x00000001u)
#define CAPSR_CE_BIT_STATUS    (0x00000001u)

#define BIT_SHIFT_20 (20u)
#define BIT_SHIFT_2  (2u)
#define BIT_SHIFT_1  (1u)
#define BIT_SHIFT_0  (0u)

#define BOUNDARY_CHECK (0x00000003u)

#define    CDOCR_A_INIT_VALUE 0x00000007
#define    CAMCR_INIT_VALUE 0x00000220

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
ceu_config_t R_CEU_Config;

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
 * Function Name: R_CEU_ModuleInit
 * @brief       CEU module pre-initialization.
 *
 *              Description:<br>
 *              In this function, the operations are performed as below.<br>
 *              - The specified user-defined function is executed.<br>
 *              - Clock supply to CEU is enabled.<br>
 * @param[in]   init_func Pointer to the user-defined function<br>
 * @retval      none.
 *****************************************************************************/
void R_CEU_ModuleInit( void (* const init_func)(void) )
{
    uint32_t reg_data;

    /* call back function */
    if ( init_func != NULL )
    {
        init_func();
    }

    /* CEU Clock is enabled
     * bit6 MSTP66 - Controls clock supply to CEU   : set 0(CEU operates)
     */
    reg_data   = (uint32_t)CPG.STBCR6;
    /*reg_data   = RZA_IO_RegRead_32((uint32_t *)&CPG.STBCR6,
                                     CPG_STBCR6_MSTP66_SHIFT,
                                     CPG_STBCR6_MSTP66);*/
    reg_data   &= STBCR6_INIT_VALUE;
    CPG.STBCR6 = (uint8_t)reg_data;

    return;
}   /* End of function R_CEU_ModuleInit() */



/**************************************************************************//**
 * Function Name: R_CEU_SetMemoryAddress
 * @brief       The function which sets the address of a capture data buffer.
 *
 *              Description:<br>
 *              The address of a capture data buffer is set.<br>
 *              The buffers must be 4 byte boundaries.<br>
 * @param[in]   addr The address of a capture data buffer. The buffers must be 4 byte boundaries.<br>
 * @retval      Error code.
 *****************************************************************************/
ceu_error_t R_CEU_SetMemoryAddress( const uint32_t addr )
{
    ceu_error_t ceu_error;

    /* boundary check */
    if ( (addr & BOUNDARY_CHECK) != 0u )
    {
        ceu_error = CEU_ERROR_PARAM;     /* Not 4Byte boundary */
    }
    else
    {
        CEU.CDAYR_A = addr;
        CEU.CDACR_A = CDACR_A_INIT_VALUE;       /* not used in data fetch. */
        CEU.CDBYR_A = CDBYR_A_INIT_VALUE;       /* not used in data fetch. */
        CEU.CDBCR_A = CDBCR_A_INIT_VALUE;       /* not used in data fetch. */
        ceu_error = CEU_ERROR_OK;
    }

    return ceu_error;

}   /* End of function R_CEU_SetMemoryAddress() */

/**************************************************************************//**
 * Function Name: R_CEU_CallbackISR
 * @brief       CEU driver set interrupt callback function.
 *
 *              Description:<br>
 *              Setting interrupt callback function.<br>
 * @param[in]   callback Pointer to the user-defined function.<br>
 * @retval      none.
 *****************************************************************************/
void R_CEU_CallbackISR( void (*const callback)(uint32_t) )
{
    if( callback != NULL ){
        CEU_Int_SetInterrupt( callback );
    }

    return;

}   /* End of function R_CEU_CallbackISR() */

/**************************************************************************//**
 * Function Name: R_CEU_CaptureStart
 * @brief       The Function which start capture.
 *
 *              Description:<br>
 *              This function start CEU capture.<br>
 * @retval      none.
 *****************************************************************************/
void R_CEU_CaptureStart( void )
{
    /* start capture */
    /* Capture Start Register (CAPSR)
     * bit0 CE - Capture Enable : set b'1 (Starts capturing)
     * other bit is not change.
     */

     //seb
    //CEU.CETCR = 0x00;

    CEU.CAPSR |= CAPSR_CE_BIT_SET;

    return;

}   /* End of function R_CEU_CaptureStart() */


/**************************************************************************//**
* Function Name: CEU_YUVImageCaptureInit
* @brief       The Function initialise the CEU registers for image capture.
*
*              Description:
*
* @retval      none.
*****************************************************************************/
void CEU_ImageCaptureInit(void)
{
/* software reset
* bit16 CPKIL - Write 1 to this bit to perform a software reset of capturing.  : set 1(Software reset of capturing)
* bit0 CE - capture enable bit    : set 0 (Stops capturing)
*/
CEU.CAPSR = CAPSR_INIT_VALUE;

/* wait software reset complete
* Complete : CSTSR.CPTON = b'0 & CAPSR.CPKIL=b'0
* [CSTSR]
* bit0 CPTON - Indicates that the CEU is operating. : 0=Halted, 1=operating
* [CAPSR]
* bit16 CPKIL - Write 1 to this bit to perform a software reset of capturing.  : 0=Normal, 1=Software reset
*/
while ( 0u != (CEU.CSTSR & CSTSR_CPTON_BIT_STATUS))
    {
        /* wait state :"Halted" */
    }

while ( 0u != (CEU.CAPSR & CAPSR_CPKIL_BIT_STATUS))
    {
        /* wait state :"Normal" */
    }
//Return to normal state
CEU.CAPSR = 0;
/* CEU.CETCR = 0; */
CEU.CETCR = 0x00000000;

/*
CEU.CAPCR.FDRP = 0; //No frame drop
CEU.CAPCR.MTCM = 0; //Y data and C data are transferred in 32-byte units
CEU.CAPCR.CTNCP = 0; //Single frame capture*/
CEU.CAPCR = 0x00000000;

/*
CEU.CAMCR.FLDPOL = 0; //FLD signal is high-active, the field is detected as the top field
CEU.CAMCR.DTIF = 0; //Data input to 8-bit digital image input pins is captured
CEU.CAMCR.DTARY= 0; //10: Image input data is fetched in the order of Y0, Cb0, Y1, and Cr0
CEU.CAMCR.JPG = 0; //Image capture mode
CEU.CAMCR.VDPOL = 0; //(VD)  is high-active
CEU.CAMCR.HDPOL = 0;  //(HD)  is high-active*/
CEU.CAMCR = CAMCR_INIT_VALUE;


/*CEU.CMCYR.VCYL = HEIGHT; //Set the buffer limit for storing
  CEU.CMCYR.HCYL = STRIDE; //Horizontal Cycle Count*/
CEU.CMCYR= ((R_CEU_Config.USEFUL_HEIGHT<<16)&0x3FFF0000) + (((R_CEU_Config.LINE_WIDTH)*2)&0x3FFF);//((WIDTH_CEU*2)&0x3FFF) ;

/*CEU.CAMOR_A.VOFST = CAMERA_VOFFSET(0); //Set camera input offset
 CEU.CAMOR_A.HOFST = CAMERA_HOFFSET(0); //Set camera input offset*/
CEU.CAMOR_A = CAMOR_A_INIT;

/*CEU.CAPWR.VWDTH = HEIGHT; //Set the number of lines to capture
 CEU.CAPWR.HWDTH = STRIDE; //Set the number of cycles to capture (1 pixel = 2 bytes/cycles)*/
CEU.CAPWR_A = (((R_CEU_Config.USEFUL_HEIGHT)<<16)&0x0FFC0000) + (((R_CEU_Config.LINE_WIDTH)*2)&0x1FF8); //((WIDTH_CEU*2)&0x1FFE);

/*CEU.CAIFR.IFS = 0; //Progressive mode
CEU.CAIFR.CIM = 0; //Clear this bit to 0 when the input mode for image capture is progressive
CEU.CAIFR.FCI = 0; //Capture starts from the VD input immediately after the CEU activation*/
CEU.CAIFR = 0x00000000; // Both-field capture (interleaved if switch planes)

/*CEU.CRCNTR.RVS = 1; //b'4=1 Switches the register plane every 1 VD
  CEU.CRCNTR.RS = 0;  //b'1=0 Uses plane A of the register
  CEU.CRCNTR.RC = 0;  //b'0=1 Switches the register plane in synchronization with VD*/
CEU.CRCNTR = 0x00000010;//Switches the register plane every 1 VD


/*CEU.CRCMPR.RA = 1; //b'0 specify plane B of the register using this bit*/
CEU.CRCMPR = 0x00000001;

/*=========================================MODIFIED PART ! ! ! !=============================*/

/*scale down 3/4*/
CEU.CFLCR_A = 0x00000000;

/*CEU.CFSZR.VFCLP = HEIGHT; //Set the number of lines to capture
  CEU.CFSZR.HFCLP = STRIDE; //Set the number of pixels per line to capture*/
CEU.CFSZR_A = 0x00b400f0;//((WIDTH_CEU*2)&0x0FFC);

/*==============================================================================================*/

/*=========================================ORIGINAL PART ! ! ! !=============================*/
//CEU.CFLCR_A = 0;//No scale down used*/

/*CEU.CFSZR.VFCLP = HEIGHT; //Set the number of lines to capture
  CEU.CFSZR.HFCLP = STRIDE; //Set the number of pixels per line to capture*/
//CEU.CFSZR_A = ((USEFUL_HEIGHT<<16)&0x0FFC0000) + ((USEFUL_WIDTH)&0x0FFC);//((WIDTH_CEU*2)&0x0FFC);
/*===================================================================================================*/

/*CEU.CDWDR_A.CHDW = CEU.CAPWR_A.HWDTH; /:In 8 bit interface mode*/
CEU.CDWDR_A = R_CEU_Config.USEFUL_WIDTH & 0x1FFC; //CEU.CAPWR_A&0x1FFC;

/*
CEU.CBDSR = CAMERA_VERTICAL; //Number of lines for output to the memory in a bundle write */
CEU.CBDSR_A = 0; //Bundle write is not used so any value is fine here

/*CEU.CLFCR_A.LPF = 0;// Low-pass filter not used*/
CEU.CLFCR_A = 0x00000000;

/*CEU.CFWCR = 0; //Firewall is not activated (upper limit of the write addresses)*/
CEU.CFWCR = 0x00000000;

/*CEU.CDOCR_A.CBE = 0;//b'16 No bundle write (image is not splited into 2 buffers)
  CEU.CDOCR_A.CDS = 0;//b'4 Outputs in the YCbCr 420 format to the memory
  CEU.CDOCR_A.COLS =1;//b'2 Data is swapped in 32-bit units?????????????
  CEU.CDOCR_A.COWS =1;//b'1 Data is swapped in 16-bit units?????????????
  CEU.CDOCR_A.COBS =1;//b'0 Data is swapped in 8-bit units ?????????????*/
CEU.CDOCR_A = CDOCR_A_INIT_VALUE;

CEU.CEIER = 0x00000000;

//Check below status registers
CEU.CETCR = 0X00000000;
CEU.CSTSR = 0X00000000;

CEU.CDSSR = 0X00000000;//Not used in image capture mode


}

/**************************************************************************//**
* Function Name: CEU_CaptureOneFrame
* @brief       The Function initialise the CEU registers for YUV image capture.
*
*              Description:
*
 * @param[in]  Frame pointer to the captured buffer
 * @return success or error (CEU busy).
*****************************************************************************/

ceu_error_t CEU_CaptureOneFrame(void *Frame)
{
    ceu_error_t ceu_error;

//Start new capture if CSTSR.CPTON == CAPSR.CE == 0 (=>no capture request and none in progress)
if((((CEU.CSTSR) & 0x00000001)==0)&&((((CEU.CAPSR) & 0X00000001)==0)))
    {
    CEU.CDAYR_A = (uint32_t) &(((uint16_t*)Frame)[0]);

    if(R_CEU_Config.MODE == R_CEU_YUV_PLANAR) {
        CEU.CDACR_A = (uint32_t) &(((uint16_t*)Frame)[R_CEU_Config.USEFUL_HEIGHT*R_CEU_Config.USEFUL_WIDTH]);
    }


    CEU.CAPSR = 0X00000001; //CAPSR.CE =1 => Start capture on next VD
    ceu_error = CEU_ERROR_OK;
    }
else
    ceu_error = CEU_ERROR_PARAM;

return (ceu_error);
}

