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
* © 2012-2013 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name : r_jcu.c
* Version : 1.0
* Description : Basic driver for the JPEG Codec Unit (JCU)
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
* : 11.05.2016 1.00 First Release
******************************************************************************/



/******************************************************************************
Includes <System Includes> , “Project Includes”
******************************************************************************/
#include "r_jcu.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define  QUANTIZATION_TABLE_SIZE    (64u)
#define  HUFFMAN_TABLE_DC_SIZE      (28u)
#define  HUFFMAN_TABLE_AC_SIZE      (178u)


/******************************************************************************
Private global variables and functions
******************************************************************************/
/*[QuantizationTable_Y]*/
/* Quality is IJG75 calculated by "QuantizationTable_Generator.xls" */
/* ->SEC M1.10.1 Table data. */
static const uint8_t  QuantizationTable_Y[QUANTIZATION_TABLE_SIZE] =
{
    8, 6, 5, 8, 12, 20, 26, 31,
    6, 6, 7, 10, 13, 29, 30, 28,
    7, 7, 8, 12, 20, 29, 35, 28,
    7, 9, 11, 15, 26, 44, 40, 31,
    9, 11, 19, 28, 34, 55, 52, 39,
    12, 18, 28, 32, 41, 52, 57, 46,
    25, 32, 39, 44, 52, 61, 60, 51,
    36, 46, 48, 49, 56, 50, 52, 50
};
/* <-SEC M1.10.1 */

/*[QuantizationTable_C]*/
/* Quality is IJG75 calculated by "QuantizationTable_Generator.xls" */
/* ->SEC M1.10.1 Table data. */
static const uint8_t  QuantizationTable_C[QUANTIZATION_TABLE_SIZE] =
{
    9, 9, 12, 24, 50, 50, 50, 50,
    9, 11, 13, 33, 50, 50, 50, 50,
    12, 13, 28, 50, 50, 50, 50, 50,
    24, 33, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50
};
/* <-SEC M1.10.1 */

/*[HuffmanTable_Y_DC]*/
/* Example written in ITU-T T81 specification */
/* ->SEC M1.10.1 Table data. */
static const uint8_t  HuffmanTable_Y_DC[HUFFMAN_TABLE_DC_SIZE] =
{
    0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B
};
/* <-SEC M1.10.1 */


/*[HuffmanTable_C_DC]*/
/* Example written in ITU-T T81 specification */
/* ->SEC M1.10.1 Table data. */
static const uint8_t  HuffmanTable_C_DC[HUFFMAN_TABLE_DC_SIZE] =
{
    0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B
};
/* <-SEC M1.10.1 */


/*[HuffmanTable_Y_AC]*/
/* Example written in ITU-T T81 specification */
/* ->SEC M1.10.1 Table data. */
static const uint8_t  HuffmanTable_Y_AC[HUFFMAN_TABLE_AC_SIZE] =
{
    0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D,
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
    0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
    0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA
};
/* <-SEC M1.10.1 */


/*[HuffmanTable_C_AC]*/
/* Example written in ITU-T T81 specification */
/* ->SEC M1.10.1 Table data. */
static const uint8_t  HuffmanTable_C_AC[HUFFMAN_TABLE_AC_SIZE] =
{
    0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0,
    0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26,
    0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5,
    0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3,
    0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA,
    0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA
};
/* <-SEC M1.10.1 */


jcu_encode_t * currentEncode;
jcu_decode_t * currentDecode;


void r_jcu_set_quantization_table(jcu_table_no_t number, const uint8_t * table) {
    volatile uint8_t*  tableAtRegister;
    uint32_t       counter;
    switch (number)
    {
        case JCU_TABLE_NO_0:
            tableAtRegister = (volatile uint8_t *)&JCU.JCQTBL0;
        break;
        case JCU_TABLE_NO_1:
            tableAtRegister = (volatile uint8_t *)&JCU.JCQTBL1;
        break;
        case JCU_TABLE_NO_2:
            tableAtRegister = (volatile uint8_t *)&JCU.JCQTBL2;
        break;
        case JCU_TABLE_NO_3:
            tableAtRegister = (volatile uint8_t *)&JCU.JCQTBL3;
        break;
        default:
            /* NOT REACHED */
        return;
    } /* end switch */

    /* Update the quantization table data */
    for (counter = 0; counter < QUANTIZATION_TABLE_SIZE; counter++)
    {
        tableAtRegister[ counter ] = table[ counter ];
    } /* end for */
}
void r_jcu_set_huffman_table(jcu_table_no_t number, jcu_huff_t huff, const uint8_t * table) {
    volatile uint8_t*  tableAtRegister;
        int32_t       copyCount;
        int32_t       counter;

        /* Select the destination Huffman table and the size of copy data */
        if (number == JCU_TABLE_NO_0)
        {
            if (huff == JCU_HUFFMAN_DC)
            {
                tableAtRegister  = (volatile uint8_t *)&JCU.JCHTBD0;
                copyCount        = HUFFMAN_TABLE_DC_SIZE;
            }
            else
            {
                tableAtRegister  = (volatile uint8_t *)&JCU.JCHTBA0;
                copyCount        = HUFFMAN_TABLE_AC_SIZE;
            } /* end if */
        }
        else
        {
            if (huff == JCU_HUFFMAN_DC)
            {
                tableAtRegister  = (volatile uint8_t *)&JCU.JCHTBD1;
                copyCount        = HUFFMAN_TABLE_DC_SIZE;
            }
            else
            {
                tableAtRegister  = (volatile uint8_t *)&JCU.JCHTBA1;
                copyCount        = HUFFMAN_TABLE_AC_SIZE;
            } /* end if */
        }  /* end if */

        /* Update the Huffman table */
        for (counter = 0; counter < copyCount; counter++)
        {
            tableAtRegister[ counter ] = table[ counter ];
        } /* end for */
}


/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name: R_JCU_EncodeInitialize
* Description  : Sets the JCU to 'encode' mode and updates the
*                encode parameters
*                "params" is saved in "currentEncode" to be accessed in
*                R_JCU_EncodeStart
* Arguments    : jcu_encode_t * params: pointer to a set of parameters
*                                       for the encoding process
* Return Value : none
******************************************************************************/
void R_JCU_EncodeInitialize(jcu_encode_t * params) {
    currentEncode = params;
    volatile uint32_t val;

    // Reinitialize the JCU module
    CPG.SWRSTCR2 = 0x02u;
    val = CPG.SWRSTCR2;
    CPG.SWRSTCR2 = 0x00u;
    val = CPG.SWRSTCR2;

    // Turn off the JCU
    JCU.JCCMD  = 0x00;

    JCU.JCMOD  = 0x00 | 0x01;                            //    Compression mode, YCrCB422 format



    JCU.JCQTN  = 0x00 | 0x00 << 4 | 0x01 << 2 | 0x01;    //    QT tables : 0, 1, 1
    JCU.JCHTN  = 0x00 | 0x00 << 4 | 0x03 << 2 | 0x03;    //    HM tables : 0, 1, 1

    JCU.JCDRIU = 0x00;                                // No DRI and RST
    JCU.JCDRID = 0x00;

    /* Set the height and width according to the params    */
    JCU.JCVSZU = params->input.height >> 8;
    JCU.JCVSZD = params->input.height & 0x00FF;

    JCU.JCHSZU = params->input.width >> 8;
    JCU.JCHSZD = params->input.width & 0x00FF;

    /* Make sure the Quantization and Huffman tables are set    */
    r_jcu_set_quantization_table( JCU_TABLE_NO_0, QuantizationTable_Y );
    r_jcu_set_quantization_table( JCU_TABLE_NO_1, QuantizationTable_C );
    r_jcu_set_huffman_table( JCU_TABLE_NO_0, JCU_HUFFMAN_DC, HuffmanTable_Y_DC );
    r_jcu_set_huffman_table( JCU_TABLE_NO_1, JCU_HUFFMAN_DC, HuffmanTable_C_DC );
    r_jcu_set_huffman_table( JCU_TABLE_NO_0, JCU_HUFFMAN_AC, HuffmanTable_Y_AC );
    r_jcu_set_huffman_table( JCU_TABLE_NO_1, JCU_HUFFMAN_AC, HuffmanTable_C_AC );

    // IO Buffer Settings
    JCU.JIFECNT   = params->output.swap << 8 | params->input.swap;
    // Input buffer address
    JCU.JIFESA       = ((((uint32_t)params->input.addr)+7)&~7);
    // Output buffer address
    JCU.JIFEDA       = ((((uint32_t)params->output.addr)+7)&~7);
    // The input width is used to determine the width of a line
    JCU.JIFESOFST = params->input.width << 1;

    JCU.JIFESVSZ  = params->input.cbcr_range;
    JCU.JIFESHSZ  = params->output.cbcr_range;

    // No interrupt used in this driver
    JCU.JINTE1    = 0x00;
    JCU.JINTS1    = 0x00;

}

/******************************************************************************
* Function Name: R_JCU_EncodeStart
* Description  : Launches an encoding process and waits until the process
*                is finished
*                The "output.size" of the "currentEncode" parameters is updated
*                at the end of the process
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_JCU_EncodeStart(void) {
    uint32_t val;

    uint8_t * addr = currentEncode->input.addr;
    uint32_t inc = currentEncode->input.width * 2 * 8;
    // Start the compression process
    JCU.JINTS1 = 0x00;
    JCU.JCCMD = 0x01;
    while ((JCU.JINTS1 & (1<<6))==0)
    {
        /* Actively wait until encode is finished    */
    }
    JCU.JINTS1 = 0x00;

    (*currentEncode).output.size = 0;
    (*currentEncode).output.size = (((uint32_t)JCU.JCDTCU) << 16);
    (*currentEncode).output.size |= (((uint32_t)JCU.JCDTCM) << 8);
    (*currentEncode).output.size |= (((uint32_t)JCU.JCDTCD));

}

/******************************************************************************
* Function Name: R_JCU_DecodeInitialize
* Description  : Sets the JCU to 'decode' mode and updates the
*                decode parameters
*                "params" is saved in "currentDecode" to be accessed in
*                R_JCU_DecodeStart
* Arguments    : jcu_decode_t * params: pointer to a set of parameters
*                                       for the decoding process
* Return Value : none
******************************************************************************/
void R_JCU_DecodeInitialize(jcu_decode_t * params) {
    currentDecode = params;
    // JPEG CORE SETTINGS
    volatile uint32_t val;

    CPG.SWRSTCR2 = 0x02u;
    val = CPG.SWRSTCR2;
    CPG.SWRSTCR2 = 0x00u;
    val = CPG.SWRSTCR2;

    JCU.JCCMD = 0x00;

    JCU.JCMOD = 0x08;                            //    Decompression mode


    JCU.JIFDCNT = 0x00
                        | params->output.vertical_scale << 28
                        | params->output.horizontal_scale << 26
                        | params->output.format << 24
                        | params->input.swap << 8
                        | params->output.swap;

    JCU.JIFDSA = ((((uint32_t)params->input.addr)+7)&~7);


    JCU.JIFDDOFST = params->output.stride << 1;
    JCU.JIFDDA = ((((uint32_t)params->output.addr)+7)&~7);


    JCU.JINTE0 = 0x00 | 1 << 7 | 1 << 6 | 1 << 5;
    JCU.JINTE1 = 0x00 | 1 << 2;
    JCU.JINTS0 = 0x00;
    JCU.JINTS1 = 0x00;

    params->output.error = JCU_NO_ERROR;
}

/******************************************************************************
* Function Name: R_JCU_DecodeStart
* Description  : Launches a decoding process and waits until the process
*                is finished
*                The "output.error" of the "currentDecode" parameters tells
*                whether the decoding was successful or not.
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_JCU_DecodeStart(void) {
    uint32_t val;
    JCU.JINTS1 = 0x00;
    JCU.JINTS0 = 0x00;
    JCU.JCCMD = 0x01;

    currentDecode->output.error = JCU_NO_ERROR;
    while ( ((JCU.JINTS1 & 1<<2) == 0) &&
            (currentDecode->output.error == JCU_NO_ERROR))
    {
        if ((JCU.JINTS0 & 1<<5) != 0)
        {
            currentDecode->output.error = JCU_DECOMP_ERROR;
        }
    }

    JCU.JINTS0 = 0x00;
    JCU.JINTS1 = 0x00;
    JCU.JCCMD = 0x00;
}
