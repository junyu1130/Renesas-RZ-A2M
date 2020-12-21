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
* File Name    : r_usbh1_hmsc_driver.c
* Description  : USB Host MSC BOT driver
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usbh1_hmsc_if.h"
#include "r_usbh1_hmsc_local.h"


/******************************************************************************
External variables and functions
******************************************************************************/
st_usbh1_hmsc_drive_management_t  g_usbh1_hmsc_drvno_tbl[USBH1_MAXSTRAGE];    /* Drive no. management table */
uint16_t                        g_usbh1_hmsc_in_pipe[USBH1_MAXSTRAGE];      /* Pipenumber */
uint16_t                        g_usbh1_hmsc_out_pipe[USBH1_MAXSTRAGE];     /* Pipenumber */
//st_usbh1_hmsc_cbw_t               g_usbh1_hmsc_cbw[USBH1_MAXSTRAGE];          /* CBW headder */
//st_usbh1_hmsc_csw_t               g_usbh1_hmsc_csw[USBH1_MAXSTRAGE];          /* CSW headder */
uint32_t                        g_usbh1_hmsc_cbw_tag_no[USBH1_MAXSTRAGE];   /* CBW tag number */
uint8_t                         g_usbh1_hmsc_sub_class[USBH1_MAXSTRAGE];
uint8_t                         g_usbh1_hmsc_tmp_sub_class;
//uint8_t                         *gp_usbh1_hmsc_device_table;
//uint8_t                         *gp_usbh1_hmsc_config_table;
//uint8_t                         *gp_usbh1_hmsc_interface_table;
uint16_t                        g_usbh1_hmsc_devaddr;
uint16_t                        g_usbh1_hmsc_init_seq = USBH1_SEQ_0;
st_usbh1_utr_t                    g_usbh1_hmsc_class_control;


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */
//static uint8_t          gs_usbh1_hmsc_class_data[USBH1_HMSC_CLSDATASIZE];
static st_usbh1_utr_t     gs_usbh1_hmsc_trans_data[USBH1_MAXSTRAGE];          /* Send data transfer message */
static st_usbh1_utr_t     gs_usbh1_hmsc_receive_data[USBH1_MAXSTRAGE];        /* Receive data transfer message */
static uint32_t         gs_usbh1_hmsc_trans_size;
static uint8_t          *gsp_usbh1_hmsc_buff;
static uint16_t         gs_usbh1_hmsc_process;
static uint16_t         gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;
static uint16_t         gs_usbh1_hmsc_stall_err_seq = USBH1_SEQ_0;
static uint16_t         gs_usbh1_hmsc_csw_err_roop = USBH1_FALSE;

/* functions */
static uint16_t         usbh1_hmsc_data_act(st_usbh1_utr_t *p_mess);
static void             usbh1_hmsc_stall_err(st_usbh1_utr_t *p_mess);
static void             usbh1_hmsc_specified_path(st_usbh1_utr_t *p_mess);
static void             usbh1_hmsc_check_result(st_usbh1_utr_t *p_mess);
static void             usbh1_hmsc_command_result(uint16_t result);
static uint16_t         usbh1_hmsc_get_string_desc(uint16_t addr, uint16_t string, usbh1_utr_cb_t complete);
static uint16_t         usbh1_hmsc_send_cbw(uint16_t side);
static uint16_t         usbh1_hmsc_send_cbw_check(uint16_t side, uint16_t hmsc_retval);
static uint16_t         usbh1_hmsc_send_data(uint16_t side, uint8_t *p_buff, uint32_t size);
static uint16_t         usbh1_hmsc_send_data_check(uint16_t side, uint16_t hmsc_retval);
static uint16_t         usbh1_hmsc_get_data(uint16_t side, uint8_t *p_buff, uint32_t size);
static uint16_t         usbh1_hmsc_get_data_check(uint16_t side, uint16_t hmsc_retval);
static uint16_t         usbh1_hmsc_get_csw(uint16_t side);
static uint16_t         usbh1_hmsc_get_csw_check(uint16_t side, uint16_t hmsc_retval);
static usbh1_er_t         usbh1_hmsc_clear_stall(uint16_t pipe, usbh1_utr_cb_t complete);
static uint16_t         usbh1_hmsc_std_req_check(uint16_t result);


st_usbh1_hmsc_cbw_t     g_usbh1_hmsc_cbw[USBH1_MAXSTRAGE] __attribute__ ((section ("USB_CLASS_1")));    /* CBW headder */
st_usbh1_hmsc_csw_t     g_usbh1_hmsc_csw[USBH1_MAXSTRAGE] __attribute__ ((section ("USB_CLASS_1")));    /* CSW headder */
uint8_t                 *gp_usbh1_hmsc_device_table __attribute__ ((section ("USB_CLASS_1")));
uint8_t                 *gp_usbh1_hmsc_config_table __attribute__ ((section ("USB_CLASS_1")));
uint8_t                 *gp_usbh1_hmsc_interface_table __attribute__ ((section ("USB_CLASS_1")));
static uint8_t          *gsp_usbh1_hmsc_buff __attribute__ ((section ("USB_CLASS_1")));
static uint8_t          gs_usbh1_hmsc_class_data[USBH1_HMSC_CLSDATASIZE] __attribute__ ((section ("USB_CLASS_1")));


/******************************************************************************
Renesas Abstracted HMSC Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : usbh1_hmsc_task
Description     : USB HMSC Task
Arguments       : none
Return value    : none
******************************************************************************/
void usbh1_hmsc_task(void)
{
    st_usbh1_utr_t    *mess;
    usbh1_er_t        err;

    /* Receive message */
    err = R_USBH1_RCV_MSG(USBH1_HMSC_MBX, (usbh1_msg_t**)&mess);
    if(USBH1_OK != err)
    {
        return;
    }

    switch(mess->msginfo)
    {
        case USBH1_MSG_CLS_INIT:
            usbh1_hmsc_class_check(mess);
        break;

        case USBH1_MSG_HMSC_NO_DATA:
        case USBH1_MSG_HMSC_DATA_IN:
        case USBH1_MSG_HMSC_DATA_OUT:
            usbh1_hmsc_data_act(mess);
        break;

        case USBH1_MSG_HMSC_CBW_ERR:
        case USBH1_MSG_HMSC_CSW_PHASE_ERR:
            usbh1_hmsc_stall_err(mess);
        break;

        default:
        break;
    }

    err = R_USBH1_REL_BLK(USBH1_HMSC_MPL,mess);
    if(USBH1_OK != err)
    {
        USBH1_PRINTF0("### USB hmsc Task rel_blk error\n");
    }
}   /* eof usbh1_hmsc_task() */

/******************************************************************************
Function Name   : usbh1_hmsc_classcheck
Description     : check class HMSC SEQ process message
Arguments       : void
Return value    : none
******************************************************************************/
void usbh1_hmsc_classcheck(void)
{
    st_usbh1_utr_t    *p_blf;
    usbh1_er_t        err;

    /* Get mem pool blk */
    err = R_USBH1_PGET_BLK(USBH1_HMSC_MPL, &p_blf);
    if (USBH1_OK == err)
    {
        p_blf->msginfo = USBH1_MSG_CLS_INIT;
        p_blf->result = USBH1_CTRL_END;

        /* Send message */
        err = R_USBH1_SND_MSG(USBH1_HMSC_MBX, (usbh1_msg_t*)p_blf);
        if (USBH1_OK != err)
        {
            err = R_USBH1_REL_BLK(USBH1_HMSC_MPL,p_blf);
            USBH1_PRINTF0("### ClassCheck function snd_msg error\n");
        }
    }
    else
    {
        USBH1_PRINTF0("### ClassCheck function pget_blk error\n");
    }
}    /* eof r_usbh1_hmsc_classcheck() */

/******************************************************************************
Function Name   : usbh1_hmsc_class_check
Description     : check class
Arguments       : st_usbh1_utr_t *mess : message
Return value    : none
******************************************************************************/
void usbh1_hmsc_class_check(st_usbh1_utr_t *p_mess)
{
    uint16_t    retval;
    uint16_t    result;

    result = USBH1_OK;
    switch(g_usbh1_hmsc_init_seq)
    {
        case USBH1_SEQ_0:
            /* Descriptor check */

            /* Check Interface Descriptor (deviceclass) */
            if (USBH1_IFCLS_MAS != gp_usbh1_hmsc_interface_table[USBH1_IF_B_INTERFACECLASS])
            {
                USBH1_PRINTF1("### Interface deviceclass is %x , not support.\n", gp_usbh1_hmsc_interface_table[5]);
                result = USBH1_ERROR;
            }

            /* Check Interface Descriptor (subclass) */
            g_usbh1_hmsc_tmp_sub_class = gp_usbh1_hmsc_interface_table[USBH1_IF_B_INTERFACESUBCLASS];
            if (USBH1_ATAPI == g_usbh1_hmsc_tmp_sub_class)
            {
                USBH1_PRINTF0("    Interface subclass  : SFF-8070i\n");
            }
            else if (USBH1_SCSI == g_usbh1_hmsc_tmp_sub_class)
            {
                USBH1_PRINTF0("    Interface subclass  : SCSI transparent command set\n");
            }
            else if (USBH1_ATAPI_MMC5 == g_usbh1_hmsc_tmp_sub_class)
            {
                USBH1_PRINTF0("    Interface subclass  : ATAPI command set\n");
            }
            else
            {
                USBH1_PRINTF1("### Interface subclass is %x , not support.\n", gp_usbh1_hmsc_interface_table[6]);

                /* Unknown Command */
                result = USBH1_ERROR;
            }

            /* Check Interface Descriptor (protocol) */
            if (USBH1_BOTP == gp_usbh1_hmsc_interface_table[USBH1_IF_B_INTERFACEPROTOCOL])
            {
                USBH1_PRINTF0("    Interface protocol   : BOT \n");
            }
            else
            {
                USBH1_PRINTF1("### Interface protocol is %x , not support.\n", gp_usbh1_hmsc_interface_table[7]);
                result = USBH1_ERROR;
            }

            /* Check Endpoint number */
            if (gp_usbh1_hmsc_interface_table[USBH1_IF_B_NUMENDPOINTS] < 2)
            {
                USBH1_PRINTF1("### Endpoint number is %x , less than 2.\n", gp_usbh1_hmsc_interface_table[4]);
                result = USBH1_ERROR;
            }

            /* Send GetDescriptor(Stirng) */
            if (USBH1_ERROR != result)
            {
/*                retval = usbh1_hmsc_get_string_desc(g_usbh1_hmsc_devaddr, 0, usbh1_hmsc_class_check);*/
                retval = usbh1_hmsc_get_string_desc(g_usbh1_hmsc_devaddr, 0, usbh1_hmsc_classcheck);
                g_usbh1_hmsc_init_seq++;
            }
        break;

        case USBH1_SEQ_1:
            retval = usbh1_hmsc_std_req_check(p_mess->result);
            if (USBH1_ERROR == retval)
            {
                result = USBH1_ERROR;
            }
            else
            {
                /* Send GetDescriptor(Stirng) */
/*
            	retval = usbh1_hmsc_get_string_desc(g_usbh1_hmsc_devaddr, (uint16_t)gp_usbh1_hmsc_device_table[15],
                                                usbh1_hmsc_class_check);
*/
                retval = usbh1_hmsc_get_string_desc(g_usbh1_hmsc_devaddr, (uint16_t)gp_usbh1_hmsc_device_table[15],
                                                usbh1_hmsc_classcheck);
                g_usbh1_hmsc_init_seq++;
            }
        break;

        case USBH1_SEQ_2:
            retval = usbh1_hmsc_std_req_check(p_mess->result);
            if (USBH1_ERROR == retval)
            {
                result = USBH1_ERROR;
            }

            if (USBH1_ERROR != result)
            {
                /* Return to MGR */
                R_USBH1_HstdReturnEnuMGR(retval);
                g_usbh1_hmsc_init_seq = USBH1_SEQ_0;
            }
        break;

        default:
            result = USBH1_ERROR;
        break;
    }

    if (USBH1_ERROR == result)
    {
        /* Return to MGR */
        R_USBH1_HstdReturnEnuMGR(USBH1_ERROR);
        g_usbh1_hmsc_init_seq = USBH1_SEQ_0;
    }
}   /* eof usbh1_hmsc_class_check() */


/******************************************************************************
Function Name   : usbh1_hmsc_set_rw_cbw
Description     : CBW parameter initialization for the READ10/WRITE10 command
Arguments       : uint16_t command :
                : uint32_t secno :
                : uint16_t seccnt :
                : uint32_t trans_byte :
                : uint16_t side :
Return value    : none
******************************************************************************/
void usbh1_hmsc_set_rw_cbw(uint16_t command, uint32_t secno, uint16_t seccnt, uint32_t trans_byte, uint16_t side)
{
    /* CBW parameter set */
    g_usbh1_hmsc_cbw[side].dcbw_tag                       = g_usbh1_hmsc_cbw_tag_no[side];
    g_usbh1_hmsc_cbw[side].dcbw_dtl_lo                    = (uint8_t)trans_byte;
    g_usbh1_hmsc_cbw[side].dcbw_dtl_ml                    = (uint8_t)(trans_byte >> 8);
    g_usbh1_hmsc_cbw[side].dcbw_dtl_mh                    = (uint8_t)(trans_byte >> 16);
    g_usbh1_hmsc_cbw[side].dcbw_dtl_hi                    = (uint8_t)(trans_byte >> 24);
    g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 0;
    g_usbh1_hmsc_cbw[side].bmcbw_flags.reserved7          = 0;
    g_usbh1_hmsc_cbw[side].bcbw_lun.bcbw_lun              = 0;    /* Support LUN0 Only */
    g_usbh1_hmsc_cbw[side].bcbw_lun.reserved4             = 0;
    g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 0;
    g_usbh1_hmsc_cbw[side].bcbw_cb_length.reserved3       = 0;

    /* ATAPI_COMMAND */
    g_usbh1_hmsc_cbw[side].cbw_cb[0] = (uint8_t)command;

    /* LUN */
    g_usbh1_hmsc_cbw[side].cbw_cb[1] = 0x00;

    /* Sector address */
    g_usbh1_hmsc_cbw[side].cbw_cb[2] = (uint8_t)(secno >> 24);
    g_usbh1_hmsc_cbw[side].cbw_cb[3] = (uint8_t)(secno >> 16);
    g_usbh1_hmsc_cbw[side].cbw_cb[4] = (uint8_t)(secno >> 8);
    g_usbh1_hmsc_cbw[side].cbw_cb[5] = (uint8_t)secno;

    /* Reserved */
    g_usbh1_hmsc_cbw[side].cbw_cb[6] = 0x00;

    /* Sector length */
    g_usbh1_hmsc_cbw[side].cbw_cb[7] = (uint8_t)(seccnt >> 8);

    /* Block address */
    g_usbh1_hmsc_cbw[side].cbw_cb[8] = (uint8_t)seccnt;

    /* Control data */
    g_usbh1_hmsc_cbw[side].cbw_cb[9] = (uint8_t)0x00;

    /* ATAPI command check */
    switch (command)
    {
        case USBH1_ATAPI_TEST_UNIT_READY:
        case USBH1_ATAPI_REQUEST_SENSE:
        case USBH1_ATAPI_INQUIRY:
        case USBH1_ATAPI_MODE_SELECT6:
        case USBH1_ATAPI_MODE_SENSE6:
        case USBH1_ATAPI_START_STOP_UNIT:
        case USBH1_ATAPI_PREVENT_ALLOW:
        case USBH1_ATAPI_READ_FORMAT_CAPACITY:
        case USBH1_ATAPI_READ_CAPACITY:
            USBH1_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        /* Initialized READ CBW TAG */
        case USBH1_ATAPI_READ10:
            g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;   /* 10bytes */
        break;

        /* Initialized WRITE CBW TAG */
        case USBH1_ATAPI_WRITE10:
            g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 0;
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;   /* 10bytes */
        break;

        case USBH1_ATAPI_SEEK:
        case USBH1_ATAPI_WRITE_AND_VERIFY:
        case USBH1_ATAPI_VERIFY10:
        case USBH1_ATAPI_MODE_SELECT10:
        case USBH1_ATAPI_MODE_SENSE10:
        default:
            USBH1_PRINTF0("### Non-mounted command demand generating !\n");
        break;
    }

    if (USBH1_ATAPI == g_usbh1_hmsc_sub_class[side])
    {
        g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length = USBH1_MSC_CBWCB_LENGTH;  /* 12bytes */
    }
}   /* eof usbh1_hmsc_set_rw_cbw() */

/******************************************************************************
Function Name   : usbh1_hmsc_set_els_cbw
Description     : CBW parameter initialization for other commands
Arguments       : uint8_t *cbwcb :
                : uint32_t trans_byte :
                : uint16_t side :
Return value    : none
******************************************************************************/
void usbh1_hmsc_set_els_cbw(uint8_t *p_cbwcb, uint32_t trans_byte, uint16_t side)
{
    uint8_t     i;

    /* CBW parameter set */
    g_usbh1_hmsc_cbw[side].dcbw_tag       = g_usbh1_hmsc_cbw_tag_no[side];
    g_usbh1_hmsc_cbw[side].dcbw_dtl_lo    = (uint8_t)trans_byte;
    g_usbh1_hmsc_cbw[side].dcbw_dtl_ml    = (uint8_t)(trans_byte >> 8);
    g_usbh1_hmsc_cbw[side].dcbw_dtl_mh    = (uint8_t)(trans_byte >> 16);
    g_usbh1_hmsc_cbw[side].dcbw_dtl_hi    = (uint8_t)(trans_byte >> 24);

    /* Receive */
    g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir        = 0;
    g_usbh1_hmsc_cbw[side].bmcbw_flags.reserved7      = 0;
    g_usbh1_hmsc_cbw[side].bcbw_lun.bcbw_lun          = 0;    /*** Support LUN0 ONLY ***/
    g_usbh1_hmsc_cbw[side].bcbw_lun.reserved4         = 0;
    g_usbh1_hmsc_cbw[side].bcbw_cb_length.reserved3   = 0;

    for (i = 0; i < 12; i++)
    {
        g_usbh1_hmsc_cbw[side].cbw_cb[i] = p_cbwcb[i];
    }

    /* ATAPI command check */
    switch (p_cbwcb[0])
    {
        case USBH1_ATAPI_TEST_UNIT_READY:
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH1_ATAPI_REQUEST_SENSE:
            g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH1_ATAPI_FORMAT_UNIT:
            USBH1_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        case USBH1_ATAPI_INQUIRY:
            g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH1_ATAPI_MODE_SELECT6:
        case USBH1_ATAPI_MODE_SENSE6:
        break;

        case USBH1_ATAPI_START_STOP_UNIT:
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH1_ATAPI_PREVENT_ALLOW:
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH1_ATAPI_READ_FORMAT_CAPACITY:
            g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;
        break;

        case USBH1_ATAPI_READ_CAPACITY:
            g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;
        break;

        case USBH1_ATAPI_READ10:
        case USBH1_ATAPI_WRITE10:
            USBH1_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        case USBH1_ATAPI_SEEK:
        case USBH1_ATAPI_WRITE_AND_VERIFY:
        case USBH1_ATAPI_VERIFY10:
            USBH1_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        case USBH1_ATAPI_MODE_SELECT10:
            USBH1_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        case USBH1_ATAPI_MODE_SENSE10:
            g_usbh1_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;
        break;

        default:
            USBH1_PRINTF0("### Non-mounted command demand generating !\n");
        break;
    }

    if (USBH1_ATAPI == g_usbh1_hmsc_sub_class[side])
    {
        g_usbh1_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length = USBH1_MSC_CBWCB_LENGTH;  /* 12bytes */
    }
}   /* eof usbh1_hmsc_set_els_cbw() */

/******************************************************************************
Function Name   : usbh1_hmsc_clr_data
Description     : data clear
Arguments       : uint16_t len :
                : uint8_t *buf :
Return value    : none
******************************************************************************/
void usbh1_hmsc_clr_data(uint16_t len, uint8_t *p_buf)
{
    uint16_t    i;

    for (i = 0; i < len; ++i)
    {
        *p_buf++ = 0x00;
    }
}   /* eof usbh1_hmsc_clr_data() */

/******************************************************************************
Function Name   : usbh1_hmsc_no_data
Description     : HMSC No data
Arguments       : uint16_t side :
Return value    : uint16_t :
******************************************************************************/
uint16_t usbh1_hmsc_no_data(uint16_t side)
{
    st_usbh1_utr_t    mess;

    mess.keyword = side;
    gs_usbh1_hmsc_process = USBH1_MSG_HMSC_NO_DATA;

    usbh1_hmsc_specified_path(&mess);

    gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;

    return USBH1_OK;
}   /* eof usbh1_hmsc_no_data() */

/******************************************************************************
Function Name   : usbh1_hmsc_data_in
Description     : HMSC Data In
Arguments       : uint16_t side :
                : uint8_t *buff :
                : uint32_t size :
Return value    : uint16_t :
******************************************************************************/
uint16_t usbh1_hmsc_data_in(uint16_t side, uint8_t *p_buff, uint32_t size)
{
    st_usbh1_utr_t    mess;

    mess.keyword = side;
    gsp_usbh1_hmsc_buff = p_buff;
    gs_usbh1_hmsc_trans_size = size;
    gs_usbh1_hmsc_process = USBH1_MSG_HMSC_DATA_IN;

    usbh1_hmsc_specified_path(&mess);

    gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;

    return USBH1_OK;
}   /* eof usbh1_hmsc_data_in() */

/******************************************************************************
Function Name   : usbh1_hmsc_data_out
Description     : HMSC Data Out
Arguments       : uint16_t side :
                : uint8_t *buff :
                : uint32_t size :
Return value    : uint16_t :
******************************************************************************/
uint16_t usbh1_hmsc_data_out(uint16_t side, const uint8_t *p_buff, uint32_t size)
{
    st_usbh1_utr_t    mess;

    mess.keyword = side;
    gsp_usbh1_hmsc_buff = (uint8_t *)p_buff;
    gs_usbh1_hmsc_trans_size = size;
    gs_usbh1_hmsc_process = USBH1_MSG_HMSC_DATA_OUT;

    usbh1_hmsc_specified_path(&mess);

    gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;

    return USBH1_OK;
}   /* eof usbh1_hmsc_data_out() */

/******************************************************************************
Function Name   : usbh1_hmsc_data_act
Description     : Send Data request
Arguments       : st_usbh1_utr_t *mess :
Return value    : uint16_t :
******************************************************************************/
static uint16_t usbh1_hmsc_data_act(st_usbh1_utr_t *p_mess)
{
    static uint16_t side;
    uint16_t        hmsc_retval;
    uint16_t        result;
    uint8_t         *p_buff;
    uint32_t        size;

    p_buff = gsp_usbh1_hmsc_buff;
    size = gs_usbh1_hmsc_trans_size;
    result = p_mess->result;

    switch (gs_usbh1_hmsc_data_seq)
    {
        case USBH1_SEQ_0:    /* CBW */
            side = p_mess->keyword;
            hmsc_retval = usbh1_hmsc_send_cbw(side);
            gs_usbh1_hmsc_data_seq++;
        break;

        case USBH1_SEQ_1:    /* Check CBW and Send Data */
            hmsc_retval = usbh1_hmsc_send_cbw_check(side, result);
            if (USBH1_HMSC_STALL == hmsc_retval)
            {
                gs_usbh1_hmsc_process = USBH1_MSG_HMSC_CBW_ERR;
                gs_usbh1_hmsc_stall_err_seq = USBH1_SEQ_0;
                p_mess->keyword = g_usbh1_hmsc_drvno_tbl[side].devadr;
                usbh1_hmsc_specified_path(p_mess);
                gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;
            }
            else if (USBH1_HMSC_OK != hmsc_retval)
            {
                USBH1_PRINTF1("### Data : SendCBW error(drive:%d) \n", side);
                gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;
                usbh1_hmsc_command_result(hmsc_retval);
            }
            else    /* Success */
            {
                switch (gs_usbh1_hmsc_process)
                {
                    case USBH1_MSG_HMSC_NO_DATA:
                        hmsc_retval = usbh1_hmsc_get_csw(side);
                        gs_usbh1_hmsc_data_seq = USBH1_SEQ_4;
                    break;

                    case USBH1_MSG_HMSC_DATA_OUT:
                        usbh1_hmsc_send_data(side, p_buff, size);
                        gs_usbh1_hmsc_data_seq++;
                    break;

                    case USBH1_MSG_HMSC_DATA_IN:
                        usbh1_hmsc_get_data(side, p_buff, size);
                        gs_usbh1_hmsc_data_seq++;
                    break;
                }
            }
        break;

        case USBH1_SEQ_2:   /* Check Data and Send CSW*/
            if (USBH1_MSG_HMSC_DATA_OUT == gs_usbh1_hmsc_process)
            {
                hmsc_retval = usbh1_hmsc_send_data_check(side, result);
            }
            else if (USBH1_MSG_HMSC_DATA_IN == gs_usbh1_hmsc_process)
            {
                hmsc_retval = usbh1_hmsc_get_data_check(side, result);
            }

            if (USBH1_HMSC_STALL == hmsc_retval)
            {
                if (USBH1_MSG_HMSC_DATA_OUT == gs_usbh1_hmsc_process)
                {
                    usbh1_hmsc_clear_stall(g_usbh1_hmsc_out_pipe[side], usbh1_hmsc_check_result);
                }
                else if (USBH1_MSG_HMSC_DATA_IN == gs_usbh1_hmsc_process)
                {
                    usbh1_hmsc_clear_stall(g_usbh1_hmsc_in_pipe[side], usbh1_hmsc_check_result);
                }

                gs_usbh1_hmsc_data_seq++;
            }
            else if (USBH1_HMSC_OK != hmsc_retval)
            {
                USBH1_PRINTF1("### Data : SendData error(drive:%d) \n", side);
                usbh1_hmsc_command_result(hmsc_retval);
                gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;
            }
            else    /* Success */
            {
                hmsc_retval = usbh1_hmsc_get_csw(side);
                gs_usbh1_hmsc_data_seq = USBH1_SEQ_4;
            }
        break;

        case USBH1_SEQ_3:    /* Check ClearStall and Send CSW */
            usbh1_hmsc_std_req_check(result);
            usbh1_hmsc_get_csw(side);
            gs_usbh1_hmsc_data_seq++;
        break;

        case USBH1_SEQ_4:    /* Check CSW */
            hmsc_retval = usbh1_hmsc_get_csw_check(side, result);
            switch (hmsc_retval)
            {
                case USBH1_HMSC_OK:    /* Success */
                    if (USBH1_TRUE == gs_usbh1_hmsc_csw_err_roop)
                    {
                        gs_usbh1_hmsc_csw_err_roop = USBH1_FALSE;
                        hmsc_retval = USBH1_HMSC_CSW_ERR;
                    }
                    usbh1_hmsc_command_result(hmsc_retval);
                break;

                case USBH1_HMSC_CSW_ERR:
                    USBH1_PRINTF1("*** Data : CSW-NG(drive:%d) \n", side);
                    gs_usbh1_hmsc_csw_err_roop = USBH1_TRUE;
                    R_USBH1_HmscRequestSense(side, p_buff);
                break;

                case USBH1_HMSC_STALL:
                    USBH1_PRINTF1("*** Data : CSW-STALL(drive:%d) \n", side);
                    gs_usbh1_hmsc_process = USBH1_MSG_HMSC_CSW_PHASE_ERR;
                    gs_usbh1_hmsc_stall_err_seq = USBH1_SEQ_0;
                    p_mess->keyword = g_usbh1_hmsc_drvno_tbl[side].devadr;
                    usbh1_hmsc_specified_path(p_mess);
                break;

                case USBH1_MSC_CSW_PHASE_ERR:
                    USBH1_PRINTF1("*** Data : CSW-PhaseError(drive:%d) \n", side);
                    gs_usbh1_hmsc_process = USBH1_MSG_HMSC_CSW_PHASE_ERR;
                    gs_usbh1_hmsc_stall_err_seq = USBH1_SEQ_0;
                    p_mess->keyword = g_usbh1_hmsc_drvno_tbl[side].devadr;
                    usbh1_hmsc_specified_path(p_mess);
                break;

                default:
                    if (USBH1_TRUE == gs_usbh1_hmsc_csw_err_roop)
                    {
                        gs_usbh1_hmsc_csw_err_roop = USBH1_FALSE;
                        hmsc_retval = USBH1_HMSC_CSW_ERR;
                    }
                    USBH1_PRINTF1("### Data : GetCSW error(drive:%d) \n", side);
                    usbh1_hmsc_command_result(hmsc_retval);
                break;
            }

            gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;
        break;

        default:
            gs_usbh1_hmsc_data_seq = USBH1_SEQ_0;
            usbh1_hmsc_command_result(hmsc_retval);
        break;
    }

    return hmsc_retval;   /* Data read error */
}   /* eof usbh1_hmsc_data_act() */

/******************************************************************************
Function Name   : usbh1_hmsc_stall_err
Description     : HMSC Stall Error
Arguments       : st_usbh1_utr_t *mess :
Return value    : none
******************************************************************************/
static void usbh1_hmsc_stall_err(st_usbh1_utr_t *p_mess)
{
    static uint16_t devadr;
    uint16_t        side;
    uint16_t        result;

    result = p_mess->result;

    switch (gs_usbh1_hmsc_stall_err_seq)
    {
        case USBH1_SEQ_0:
            devadr = p_mess->keyword;
            R_USBH1_HmscMassStorageReset(devadr, usbh1_hmsc_check_result);
            gs_usbh1_hmsc_stall_err_seq++;
        break;

        case USBH1_SEQ_1:
            usbh1_hmsc_std_req_check(result);
            R_USBH1_HmscRefDrvNo(&side, devadr);
            usbh1_hmsc_clear_stall(g_usbh1_hmsc_out_pipe[side], usbh1_hmsc_check_result);
            gs_usbh1_hmsc_stall_err_seq++;
        break;

        case USBH1_SEQ_2:
            R_USBH1_HmscRefDrvNo(&side, devadr);
            usbh1_hmsc_clear_stall(g_usbh1_hmsc_in_pipe[side], usbh1_hmsc_check_result);
            gs_usbh1_hmsc_stall_err_seq++;
        break;

        case USBH1_SEQ_3:
            if (USBH1_MSG_HMSC_CSW_PHASE_ERR == p_mess->msginfo)
            {
                result = USBH1_HMSC_CSW_PHASE_ERR;
            }
            else
            {
                result = USBH1_HMSC_CBW_ERR;
            }

            usbh1_hmsc_command_result(result);
            gs_usbh1_hmsc_stall_err_seq = USBH1_SEQ_0;
        break;

        default:
            if (USBH1_MSG_HMSC_CSW_PHASE_ERR == p_mess->msginfo)
            {
                result = USBH1_HMSC_CSW_PHASE_ERR;
            }
            else
            {
                result = USBH1_HMSC_CBW_ERR;
            }
            usbh1_hmsc_command_result(result);
            gs_usbh1_hmsc_stall_err_seq = USBH1_SEQ_0;
        break;
    }
}   /* eof usbh1_hmsc_stall_err() */

/******************************************************************************
Function Name   : usbh1_hmsc_specified_path
Description     : Next Process Selector
Arguments       : st_usbh1_utr_t *mess :
Return value    : none
******************************************************************************/
static void usbh1_hmsc_specified_path(st_usbh1_utr_t *p_mess)
{
    st_usbh1_utr_t    *p_blf;
    usbh1_er_t        err;

    /* Get mem pool blk */
    err = R_USBH1_PGET_BLK(USBH1_HMSC_MPL, &p_blf);
    if (USBH1_OK == err)
    {
        p_blf->msginfo  = gs_usbh1_hmsc_process;
        p_blf->keyword  = p_mess->keyword;
        p_blf->result   = p_mess->result;

        /* Send message */
        err = R_USBH1_SND_MSG(USBH1_HMSC_MBX, (usbh1_msg_t*)p_blf);
        if (USBH1_OK != err)
        {
            err = R_USBH1_REL_BLK(USBH1_HMSC_MPL,p_blf);
            USBH1_PRINTF0("### SpecifiedPass function snd_msg error\n");
        }
    }
    else
    {
        USBH1_PRINTF0("### SpecifiedPass function pget_blk error\n");
    }
}   /* eof usbh1_hmsc_specified_path() */

/******************************************************************************
Function Name   : usbh1_hmsc_check_result
Description     : Hub class check result
Arguments       : st_usbh1_utr_t *mess :
Return value    : none
******************************************************************************/
static void usbh1_hmsc_check_result(st_usbh1_utr_t *p_mess)
{
    usbh1_hmsc_specified_path(p_mess);
}   /* eof usbh1_hmsc_check_result() */

/******************************************************************************
Function Name   : usbh1_hmsc_command_result
Description     : Hub class check result
Arguments       : uint16_t result     : Result
Return value    : none
******************************************************************************/
static void usbh1_hmsc_command_result(uint16_t result)
{
    st_usbh1_utr_t    *p_blf;
    usbh1_er_t        err;

    /* Get mem pool blk */
    err = R_USBH1_PGET_BLK(USBH1_HSTRG_MPL, &p_blf);
    if (USBH1_OK == err)
    {
        p_blf->msginfo  = g_usbh1_hmsc_strg_process;
        p_blf->result   = result;

        /* Send message */
        err = R_USBH1_SND_MSG(USBH1_HSTRG_MBX, (usbh1_msg_t*)p_blf);
        if (USBH1_OK != err)
        {
            err = R_USBH1_REL_BLK(USBH1_HSTRG_MPL, p_blf);
            USBH1_PRINTF0("### CheckResult function snd_msg error\n");
        }
    }
    else
    {
        USBH1_PRINTF0("### CheckResult function pget_blk error\n");
    }
}   /* eof usbh1_hmsc_command_result() */


/******************************************************************************
Function Name   : usbh1_hmsc_get_string_desc
Description     : Set GetDescriptor
Arguments       : uint16_t addr            : device address
                : uint16_t string          : descriptor index
                : usbh1_utr_cb_t complete    : callback function
Return value    : uint16_t                 : error info
******************************************************************************/
static uint16_t usbh1_hmsc_get_string_desc(uint16_t addr, uint16_t string, usbh1_utr_cb_t complete)
{
    static st_usbh1_setup_t   string_setup;
    uint16_t                i;

    if (0 == string)
    {
        string_setup.index  = (uint16_t)0x0000;
        string_setup.length = (uint16_t)0x0004;
    }
    else
    {
        /* Set LanguageID */
        string_setup.index  = (uint16_t)(gs_usbh1_hmsc_class_data[2]);
        string_setup.index |= (uint16_t)((uint16_t)(gs_usbh1_hmsc_class_data[3]) << 8);
        string_setup.length = (uint16_t)USBH1_HMSC_CLSDATASIZE;
    }

    string_setup.type       = (USBH1_GET_DESCRIPTOR | USBH1_DEV_TO_HOST | USBH1_STANDARD | USBH1_DEVICE);
    string_setup.value      = (uint16_t)(USBH1_STRING_DESCRIPTOR + string);
    string_setup.devaddr    = addr;

    for (i = 0; i < string_setup.length; i++)
    {
        gs_usbh1_hmsc_class_data[i] = (uint8_t)0xFF;
    }

    g_usbh1_hmsc_class_control.p_tranadr    = (void *)gs_usbh1_hmsc_class_data;
    g_usbh1_hmsc_class_control.complete   = complete;
    g_usbh1_hmsc_class_control.tranlen    = (uint32_t)string_setup.length;
    g_usbh1_hmsc_class_control.keyword    = USBH1_PIPE0;
    g_usbh1_hmsc_class_control.p_setup      = &string_setup;

    return (R_USBH1_HstdTransferStart(&g_usbh1_hmsc_class_control));
}   /* eof usbh1_hmsc_get_string_desc() */

/******************************************************************************
Function Name   : usbh1_hmsc_send_cbw
Description     : Send CBW
Arguments       : uint16_t side           : Drive Number
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh1_hmsc_send_cbw(uint16_t side)
{
    usbh1_er_t    err;

    /* Set CBW TAG usbh1_hmsc_CbwTagCount()*/
    g_usbh1_hmsc_cbw_tag_no[side]++;
    if ((uint16_t)0 == g_usbh1_hmsc_cbw_tag_no[side])
    {
        g_usbh1_hmsc_cbw_tag_no[side] = (uint16_t)1;
    }

    gs_usbh1_hmsc_trans_data[side].keyword    = g_usbh1_hmsc_out_pipe[side];    /* Device number */
    gs_usbh1_hmsc_trans_data[side].p_tranadr    = (void*)&g_usbh1_hmsc_cbw[side]; /* Transfer data address */
    gs_usbh1_hmsc_trans_data[side].tranlen    = (uint32_t)USBH1_MSC_CBWLENGTH;  /* Transfer data length */
    gs_usbh1_hmsc_trans_data[side].complete   = &usbh1_hmsc_check_result;        /* Call Back Function Info */

    err = R_USBH1_HstdTransferStart(&gs_usbh1_hmsc_trans_data[side]);
    if (USBH1_OK != err)
    {
        USBH1_PRINTF1("### Mass Storage Device Class submit error(drive:%d) !\n", side);
        return USBH1_HMSC_SUBMIT_ERR;
    }

    return err;
}   /* eof usbh1_hmsc_send_cbw() */

/******************************************************************************
Function Name   : usbh1_hmsc_send_cbw_check
Description     : Check send CBW
Arguments       : uint16_t side           : Drive Number
                : uint16_t result         : Transfer Result
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh1_hmsc_send_cbw_check(uint16_t side, uint16_t result)
{
    switch (result)
    {
        case USBH1_DATA_NONE:     /* Send CBW */
            return USBH1_HMSC_OK;
        break;

        case USBH1_DATA_STALL:    /* Stall */
            USBH1_PRINTF1("*** CBW Transfer STALL(drive:%d) !\n", side);
            return USBH1_HMSC_STALL;;
        break;

        case USBH1_DATA_ERR:
            USBH1_PRINTF1("### CBW Transfer ERROR(drive:%d) !\n", side);
        break;

        default:
            USBH1_PRINTF1("### CBW Transfer error(drive:%d) !\n", side);
        break;
    }

    return USBH1_HMSC_CBW_ERR;
}   /* eof usbh1_hmsc_send_cbw_check() */

/******************************************************************************
Function Name   : usbh1_hmsc_get_data
Description     : Receive Data request
Arguments       : uint16_t side           : Drive Number
                : uint8_t *buff           : Receive Data Buffer Address
                : uint32_t size           : Receive Data Size
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh1_hmsc_get_data(uint16_t side, uint8_t *p_buff, uint32_t size)
{
    usbh1_er_t    err;

    gs_usbh1_hmsc_receive_data[side].keyword  = g_usbh1_hmsc_in_pipe[side]; /* Device number */
    gs_usbh1_hmsc_receive_data[side].p_tranadr  = (void*)p_buff;            /* Transfer data address */
    gs_usbh1_hmsc_receive_data[side].tranlen  = size;                     /* Transfer data length */
    gs_usbh1_hmsc_receive_data[side].complete = &usbh1_hmsc_check_result;    /* Call Back Function Info */

    err = R_USBH1_HstdTransferStart(&gs_usbh1_hmsc_receive_data[side]);
    if (USBH1_OK != err)
    {
        USBH1_PRINTF1("### Mass Storage Device Class submit error(drive:%d) !\n", side);
        return USBH1_HMSC_SUBMIT_ERR;
    }

    return err;
}   /* eof usbh1_hmsc_get_data() */


/******************************************************************************
Function Name   : usbh1_hmsc_get_data_check
Description     : Check Get Data
Arguments       : uint16_t side           : Drive Number
                : uint16_t result         : Transfer Result
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh1_hmsc_get_data_check(uint16_t side, uint16_t result)
{
    switch (result)
    {
        case USBH1_DATA_SHT:
            /* Continue */
        case USBH1_DATA_OK:
            return USBH1_HMSC_OK;
        break;

        case USBH1_DATA_STALL:
            USBH1_PRINTF1("*** GetData STALL(drive:%d) !\n", side);
            return USBH1_HMSC_STALL;
        break;

        case USBH1_DATA_ERR:
            USBH1_PRINTF1("### GetData ERROR(drive:%d) !\n", side);
        break;

        case USBH1_DATA_OVR:
            USBH1_PRINTF1("### GetData over(drive:%d) !\n", side);
        break;

        default:
            USBH1_PRINTF1("### GetData error(drive:%d) !\n", side);
        break;
    }

    return USBH1_HMSC_DAT_RD_ERR;
}   /* eof usbh1_hmsc_get_data_check() */

/******************************************************************************
Function Name   : usbh1_hmsc_send_data
Description     : Send Pipe Data
Arguments       : uint16_t side           : Drive Number
                : uint8_t  *buff          : Data Info Address
                : uint32_t size           : Data Size
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh1_hmsc_send_data(uint16_t side, uint8_t *p_buff, uint32_t size)
{
    usbh1_er_t    err;

    gs_usbh1_hmsc_trans_data[side].keyword    = g_usbh1_hmsc_out_pipe[side];    /* Device number */
    gs_usbh1_hmsc_trans_data[side].p_tranadr  = (void*)p_buff;                /* Transfer data address */
    gs_usbh1_hmsc_trans_data[side].tranlen    = size;                         /* Transfer data length */
    gs_usbh1_hmsc_trans_data[side].complete   = &usbh1_hmsc_check_result;        /* Call Back Function Info */

    err = R_USBH1_HstdTransferStart(&gs_usbh1_hmsc_trans_data[side]);
    if (USBH1_OK != err)
    {
        USBH1_PRINTF1("### Mass Storage Device Class submit error(drive:%d) !\n", side);
        return USBH1_HMSC_SUBMIT_ERR;
    }

    return err;
}   /* eof usbh1_hmsc_send_data() */

/******************************************************************************
Function Name   : usbh1_hmsc_send_data_check
Description     : Check Send Data
Arguments       : uint16_t side           : Drive Number
                : uint16_t result         : Transfer Result
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh1_hmsc_send_data_check(uint16_t side, uint16_t result)
{
    switch (result)
    {
        case USBH1_DATA_NONE:
            return USBH1_HMSC_OK;
        break;

        case USBH1_DATA_STALL:
            USBH1_PRINTF1("*** SendData STALL(drive:%d) !\n", side);
            return USBH1_HMSC_STALL;
        break;

        case USBH1_DATA_ERR:
            USBH1_PRINTF1("### SendData ERROR(drive:%d) !\n", side);
        break;

        default:
            USBH1_PRINTF1("### SendData error(drive:%d) !\n", side);
        break;
    }

    return USBH1_HMSC_DAT_WR_ERR;
}   /* eof usbh1_hmsc_send_data_check() */

/******************************************************************************
Function Name   : usbh1_hmsc_get_csw
Description     : Receive CSW
Arguments       : uint16_t side           : Drive Number
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh1_hmsc_get_csw(uint16_t side)
{
    usbh1_er_t    err;

    gs_usbh1_hmsc_receive_data[side].keyword  = g_usbh1_hmsc_in_pipe[side];     /* Device number */
    gs_usbh1_hmsc_receive_data[side].p_tranadr  = (void*)&g_usbh1_hmsc_csw[side]; /* Transfer data address */
    gs_usbh1_hmsc_receive_data[side].tranlen  = (uint32_t)USBH1_MSC_CSW_LENGTH; /* Transfer data length */
    gs_usbh1_hmsc_receive_data[side].complete = &usbh1_hmsc_check_result;        /* Call Back Function Info */

    err = R_USBH1_HstdTransferStart(&gs_usbh1_hmsc_receive_data[side]);
    if (USBH1_OK != err)
    {
        USBH1_PRINTF1("### Mass Storage Device Class submit error(drive:%d) !\n", side);
        return USBH1_HMSC_SUBMIT_ERR;
    }

    return err;
}   /* eof usbh1_hmsc_get_csw() */

/******************************************************************************
Function Name   : usbh1_hmsc_get_csw_check
Description     : Check Receive CSW
Arguments       : uint16_t side           : Drive Number
                : uint16_t result         : Transfer Result
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh1_hmsc_get_csw_check(uint16_t side, uint16_t result)
{
    switch (result)
    {
        case USBH1_DATA_SHT:
            /* Continue */
        case USBH1_DATA_OK:
            /* CSW Check */
            if (USBH1_MSC_CSW_SIGNATURE != g_usbh1_hmsc_csw[side].dcsw_signature)
            {
                USBH1_PRINTF2("### CSW signature error 0x%08x:SIGN=0x%08x.\n",
                                g_usbh1_hmsc_csw[side].dcsw_signature, USBH1_MSC_CSW_SIGNATURE);
                return USBH1_HMSC_CSW_ERR;
            }

            if (g_usbh1_hmsc_csw[side].dcsw_tag != g_usbh1_hmsc_cbw[side].dcbw_tag)
            {
                USBH1_PRINTF2("### CSW Tag error 0x%08x:CBWTAG=0x%08x.\n",
                                g_usbh1_hmsc_csw[side].dcsw_tag, g_usbh1_hmsc_cbw[side].dcbw_tag);
                return USBH1_HMSC_CSW_ERR;
            }

            switch (g_usbh1_hmsc_csw[side].bcsw_status)
            {
                case USBH1_MSC_CSW_OK:
                    return USBH1_HMSC_OK;
                break;

                case USBH1_MSC_CSW_NG:
                    return USBH1_HMSC_CSW_ERR;
                break;

                case USBH1_MSC_CSW_PHASE_ERR:
                    return USBH1_HMSC_CSW_PHASE_ERR;
                break;

                default:
                    USBH1_PRINTF1("### CSW status error 0x%2x.\n", g_usbh1_hmsc_csw[side].bcsw_status);
                    return USBH1_HMSC_CSW_ERR;
                break;
            }
        break;

        case USBH1_DATA_STALL:
            /* Stall */
            USBH1_PRINTF1("*** GetCSW Transfer STALL(drive:%d) !\n", side);
            return USBH1_HMSC_STALL;;
        break;

        case USBH1_DATA_ERR:
            USBH1_PRINTF1("### GetCSW Transfer ERROR(drive:%d) !\n", side);
        break;

        case USBH1_DATA_OVR:
            USBH1_PRINTF1("### GetCSW receive over(drive:%d) !\n", side);
        break;

        default:
            USBH1_PRINTF1("### GetCSW Transfer error(drive:%d) !\n", side);
        break;
    }

    return USBH1_HMSC_CSW_ERR;
}   /* eof usbh1_hmsc_get_csw_check() */

/******************************************************************************
Function Name   : usbh1_hmsc_clear_stall
Description     : Clear Stall
Arguments       : uint16_t     pipe     : PipeID
                : usbh1_utr_cb_t complete : Callback function
Return value    : uint16_t
******************************************************************************/
static usbh1_er_t usbh1_hmsc_clear_stall(uint16_t pipe, usbh1_utr_cb_t complete)
{
    usbh1_er_t    err;

    err = R_USBH1_HstdChangeDeviceState(complete, USBH1_DO_CLR_STALL, pipe);

    return err;
}   /* eof usbh1_hmsc_clear_stall() */

/******************************************************************************
Function Name   : usbh1_hmsc_std_req_check
Description     : Sample Standard Request Check
Arguments       : uint16_t result        : Transfer Result
Return value    : uint16_t               : error info
******************************************************************************/
static uint16_t usbh1_hmsc_std_req_check(uint16_t result)
{
    if (USBH1_DATA_STALL == result)
    {
        USBH1_PRINTF0("*** Standard Request STALL !\n");
        return USBH1_ERROR;
    }
    else if (USBH1_CTRL_END != result)
    {
        USBH1_PRINTF0("*** Standard Request error !\n");
        return USBH1_ERROR;
    }
    else
    {
        return USBH1_OK;
    }
}   /* eof usbh1_hmsc_std_req_check() */

/******************************************************************************
End  Of File
******************************************************************************/
