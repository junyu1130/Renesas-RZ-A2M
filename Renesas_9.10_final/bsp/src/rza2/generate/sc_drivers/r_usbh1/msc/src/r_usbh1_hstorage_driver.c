/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
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
* Copyright (C) 2014(2015) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usbh1_hstorage_driver.c
* Description  : USB Host Storage Driver
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usbh1_hmsc_if.h"
#include "r_usbh1_hatapi_define.h"
#include "r_usbh1_hmsc_local.h"


/******************************************************************************
External variables and functions
******************************************************************************/
uint16_t    g_usbh1_hmsc_strg_process;
uint16_t    g_usbh1_hmsc_strg_drive_search_seq;


/******************************************************************************
Private global variables and functions
******************************************************************************/
//static uint8_t  gs_usbh1_hmsc_data[512];
static uint8_t  gs_usbh1_hmsc_data[512] __attribute__ ((section ("USB_CLASS_1")));
static uint16_t gs_usbh1_hmsc_read_partition_retry_count = 0;

static void     usbh1_hmsc_strg_specified_path(st_usbh1_utr_t *p_mess);
static void     usbh1_hmsc_strg_check_result(st_usbh1_utr_t *p_mess);


/******************************************************************************
Function Name   : usbh1_hmsc_strg_drive_search_act
Description     : Storage drive search
Arguments       : st_usbh1_utr_t *mess   : Message
Return value    : none
******************************************************************************/
void usbh1_hmsc_strg_drive_search_act(st_usbh1_utr_t *p_mess)
{
    static uint16_t side;
    uint32_t        max_lun;

    switch(g_usbh1_hmsc_strg_drive_search_seq)
    {
        case USBH1_SEQ_0:     /* Get MAX_LUN */
            USBH1_PRINTF0("\n*** Drive search !\n");
            g_usbh1_hmsc_strg_process = USBH1_MSG_HMSC_STRG_DRIVE_SEARCH;
            R_USBH1_HmscRefDrvNo(&side, p_mess->keyword);
            R_USBH1_HmscGetMaxUnit(p_mess->keyword, gs_usbh1_hmsc_data, usbh1_hmsc_strg_check_result);
            g_usbh1_hmsc_strg_drive_search_seq++;
        break;

        case USBH1_SEQ_1:     /* Check result */
            if (USBH1_HMSC_OK != p_mess->result)
            {
                USBH1_PRINTF0("### GetMaxLUN error\n");
            }

            max_lun =  gs_usbh1_hmsc_data[0];
            if (0 != max_lun)
            {
                USBH1_PRINTF1("*** GetMaxLUN data error (%d), set unit number 0 !\n", max_lun);
                max_lun = 0;
            }
            else
            {
                USBH1_PRINTF1(" Unit number is %d\n", max_lun);
            }

            /*** Support LUN0 ONLY ***/
            max_lun = 0;

            USBH1_PRINTF0("\nPlease wait device ready\n");
            R_USBH1_HstdDelayXms(100);

            usbh1_hmsc_strg_specified_path(p_mess);
            g_usbh1_hmsc_strg_drive_search_seq++;
        break;

        case USBH1_SEQ_2:     /* Inquiry */
            R_USBH1_HmscInquiry(side, gs_usbh1_hmsc_data);
            g_usbh1_hmsc_strg_drive_search_seq++;
        break;

        case USBH1_SEQ_3:     /* Check result */
            if (USBH1_HMSC_OK != p_mess->result)
            {
                USBH1_PRINTF0("### Inquiry error\n");
            }
            usbh1_hmsc_strg_specified_path(p_mess);
            g_usbh1_hmsc_strg_drive_search_seq++;
        break;

        case USBH1_SEQ_4:     /* Read Format Capacity */
            R_USBH1_HmscReadFormatCapacity(side, gs_usbh1_hmsc_data);
            g_usbh1_hmsc_strg_drive_search_seq++;
        break;

        case USBH1_SEQ_5:     /* Read Capacity */
            R_USBH1_HmscReadCapacity(side, gs_usbh1_hmsc_data);
            g_usbh1_hmsc_strg_drive_search_seq++;
        break;

        case USBH1_SEQ_6:
            if (USBH1_HMSC_OK != p_mess->result)
            {
                /* TestUnitReady */
                R_USBH1_HmscTestUnit(side);
                g_usbh1_hmsc_strg_drive_search_seq++;
            }
            else
            {
                /* Pass TestUnitReady */
                g_usbh1_hmsc_strg_drive_search_seq = USBH1_SEQ_8;
                usbh1_hmsc_strg_specified_path(p_mess);
            }
        break;

        case USBH1_SEQ_7:
            if (USBH1_HMSC_OK != p_mess->result)
            {
                /* TestUnitReady (Retry) */
                R_USBH1_HmscTestUnit(side);
                g_usbh1_hmsc_strg_drive_search_seq = USBH1_SEQ_7;
            }
            else
            {
                /* Read Capacity (Retry) */
                R_USBH1_HmscReadCapacity(side, (uint8_t*)&gs_usbh1_hmsc_data);
                g_usbh1_hmsc_strg_drive_search_seq++;
            }
        break;

        case USBH1_SEQ_8:
            R_USBH1_HmscRead10(side, (uint8_t*)&gs_usbh1_hmsc_data, 0, 1, 512);
            g_usbh1_hmsc_strg_drive_search_seq++;
        break;

        case USBH1_SEQ_9:
            if (USBH1_HMSC_OK == p_mess->result)
            {
                g_usbh1_hmsc_strg_drive_search_seq = USBH1_SEQ_0;
                g_usbh1_hmsc_strg_process = USBH1_MSG_HMSC_STRG_DRIVE_SEARCH_END;
            }
            else
            {
                g_usbh1_hmsc_strg_drive_search_seq = USBH1_SEQ_8;
                gs_usbh1_hmsc_read_partition_retry_count++;
                if (10 == gs_usbh1_hmsc_read_partition_retry_count)
                {
                    g_usbh1_hmsc_strg_drive_search_seq = USBH1_SEQ_0;
                    g_usbh1_hmsc_strg_process = USBH1_MSG_HMSC_STRG_DRIVE_SEARCH_END;
                }
            }
            usbh1_hmsc_strg_specified_path(p_mess);
        break;

        default:
            g_usbh1_hmsc_strg_drive_search_seq = USBH1_SEQ_0;
            g_usbh1_hmsc_strg_process = USBH1_MSG_HMSC_STRG_DRIVE_SEARCH_END;
            usbh1_hmsc_strg_specified_path(p_mess);
        break;
    }
}   /* eof usbh1_hmsc_strg_drive_search_act() */

/******************************************************************************
Function Name   : usbh1_hmsc_strg_specified_path
Description     : Next Process Selector
Arguments       : st_usbh1_utr_t *mess  : Message
Return value    : none
******************************************************************************/
static void usbh1_hmsc_strg_specified_path(st_usbh1_utr_t *p_mess)
{
    st_usbh1_utr_t    *p_blf;
    usbh1_er_t        err;

    /* Get mem pool blk */
    err = R_USBH1_PGET_BLK(USBH1_HSTRG_MPL, &p_blf);
    if (USBH1_OK == err)
    {
        p_blf->msginfo  = g_usbh1_hmsc_strg_process;
        p_blf->keyword  = p_mess->keyword;
        p_blf->result   = p_mess->result;

        /* Send message */
        err = R_USBH1_SND_MSG(USBH1_HSTRG_MBX, (usbh1_msg_t*)p_blf);
        if (USBH1_OK != err)
        {
            err = R_USBH1_REL_BLK(USBH1_HSTRG_MPL, p_blf);
            USBH1_PRINTF0("### SpecifiedPass function snd_msg error\n");
        }
    }
    else
    {
        USBH1_PRINTF0("### SpecifiedPass function pget_blk error\n");
    }
}   /* eof usbh1_hmsc_strg_specified_path() */

/******************************************************************************
Function Name   : usbh1_hmsc_strg_check_result
Description     : Hub class check result
Arguments       : st_usbh1_utr_t *mess    : Message
Return value    : none
******************************************************************************/
static void usbh1_hmsc_strg_check_result(st_usbh1_utr_t *p_mess)
{
    usbh1_hmsc_strg_specified_path(p_mess);
}   /* eof usbh1_hmsc_strg_check_result() */


/******************************************************************************
End  Of File
******************************************************************************/
