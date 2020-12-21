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
* File Name    : r_usbh1_hmsc_api.c
* Description  : USB Host MSC Driver API
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


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
Renesas Host MSC Driver API functions
******************************************************************************/


/******************************************************************************
Function Name   : R_USBH1_HmscTask
Description     : USB HMSC Task
Arguments       : none
Return value    : none
******************************************************************************/
void R_USBH1_HmscTask(void)
{
#if (BSP_CFG_RTOS_USED == 1)
	st_usbh1_utr_t  *p_mess;
	while(1)
	{
		USBH1_RTOS_RCV_MSG(USBH1_HMSC_MSG, (usbh1_msg_t **)&p_mess);
		usbh1_hmsc_task();
		g_usbh1_msg_check &= ~(1<<USBH1_HMSC_TSK);
	}
#else /* (BSP_CFG_RTOS_USED == 1) */
	usbh1_hmsc_task();
#endif /* (BSP_CFG_RTOS_USED == 1) */
}   /* eof R_USBH1_HmscTask() */

/******************************************************************************
Function Name   : R_USBH1_HmscDriverStart
Description     : USB Host Initialize process
Argument        : none
Return          : none
******************************************************************************/
void R_USBH1_HmscDriverStart(void)
{
    uint16_t    i;

    for (i = 0; i < USBH1_MAXSTRAGE; i++)
    {
        g_usbh1_hmsc_drvno_tbl[i].use_flag = USBH1_FALSE;

        g_usbh1_hmsc_cbw_tag_no[i] = (uint16_t)1;

        g_usbh1_hmsc_cbw[i].dcbw_signature                = USBH1_MSC_CBW_SIGNATURE;
        g_usbh1_hmsc_cbw[i].dcbw_tag                      = g_usbh1_hmsc_cbw_tag_no[i];
        g_usbh1_hmsc_cbw[i].dcbw_dtl_lo                   = 0;
        g_usbh1_hmsc_cbw[i].dcbw_dtl_ml                   = 0;
        g_usbh1_hmsc_cbw[i].dcbw_dtl_mh                   = 0;
        g_usbh1_hmsc_cbw[i].dcbw_dtl_hi                   = 0;
        g_usbh1_hmsc_cbw[i].bmcbw_flags.cbw_dir           = 0;
        g_usbh1_hmsc_cbw[i].bmcbw_flags.reserved7         = 0;
        g_usbh1_hmsc_cbw[i].bcbw_lun.bcbw_lun             = 0;
        g_usbh1_hmsc_cbw[i].bcbw_lun.reserved4            = 0;
        g_usbh1_hmsc_cbw[i].bcbw_cb_length.bcbw_cb_length = 0;
        g_usbh1_hmsc_cbw[i].bcbw_cb_length.reserved3      = 0;

        g_usbh1_hmsc_cbw[i].cbw_cb[0]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[1]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[2]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[3]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[4]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[5]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[6]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[7]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[8]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[9]     = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[10]    = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[11]    = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[12]    = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[13]    = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[14]    = 0;
        g_usbh1_hmsc_cbw[i].cbw_cb[15]    = 0;
    }

    R_USBH1_CstdSetTaskPri(USBH1_HMSC_TSK, USBH1_PRI_3);
    R_USBH1_CstdSetTaskPri(USBH1_HSTRG_TSK, USBH1_PRI_3);
}   /* eof R_USBH1_HmscDriverStart() */

/******************************************************************************
Function Name   : R_USBH1_HmscClassCheck
Description     : check connected device
Arguments       : uint16_t **table :
Return value    : none
******************************************************************************/
void R_USBH1_HmscClassCheck(uint16_t **pp_table)
{
    st_usbh1_utr_t    *p_blf;
    usbh1_er_t        err;

    gp_usbh1_hmsc_device_table    = (uint8_t*)(pp_table[0]);
    gp_usbh1_hmsc_config_table    = (uint8_t*)(pp_table[1]);
    gp_usbh1_hmsc_interface_table = (uint8_t*)(pp_table[2]);
    g_usbh1_hmsc_devaddr          = *pp_table[7];

    R_USBH1_HstdSetPipe(pp_table);

    /* Get mem pool blk */
    err = R_USBH1_PGET_BLK(USBH1_HMSC_MPL, &p_blf);
    if (USBH1_OK == err)
    {
        p_blf->msginfo = USBH1_MSG_CLS_INIT;
        g_usbh1_hmsc_init_seq = USBH1_SEQ_0;

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
}   /* eof R_USBH1_HmscClassCheck() */

/******************************************************************************
Function Name   : R_USBH1_HmscRead10
Description     : Read10
Arguments       : uint16_t side        : Side
                : uint8_t  *buff       : Buffer address
                : uint32_t secno       : Sector number
                : uint16_t seccnt      : Sector count
                : uint32_t trans_byte  : Trans byte
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscRead10(uint16_t side, uint8_t *buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    uint16_t    hmsc_retval;

    /* Set CBW parameter */
    usbh1_hmsc_set_rw_cbw((uint16_t)USBH1_ATAPI_READ10, secno, seccnt, trans_byte, side);

    /* Data In */
    hmsc_retval = usbh1_hmsc_data_in(side, buff, trans_byte);

    return hmsc_retval;
}   /* eof R_USBH1_HmscRead10() */

/******************************************************************************
Function Name   : R_USBH1_HmscWrite10
Description     : Write10
Arguments       : uint16_t side        : Side
                : const uint8_t  *buff : Buffer address
                : uint32_t secno       : Sector number
                : uint16_t seccnt      : Sector count
                : uint32_t trans_byte  : Trans byte
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscWrite10( uint16_t side, const uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    uint16_t    hmsc_retval;

    /* set CBW parameter */
    usbh1_hmsc_set_rw_cbw((uint16_t)USBH1_ATAPI_WRITE10, secno, seccnt, trans_byte, side);

    /* Data OUT */
    hmsc_retval = usbh1_hmsc_data_out(side, p_buff, trans_byte);

    return hmsc_retval;
}   /* eof R_USBH1_HmscWrite10() */

/******************************************************************************
Function Name   : R_USBH1_HmscTestUnit
Description     : TestUnit
Arguments       : uint16_t side :
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscTestUnit(uint16_t side)
{
    uint8_t     cbwcb[12];
    uint16_t    hmsc_retval;

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)12, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USBH1_ATAPI_TEST_UNIT_READY;

    /* Set CBW parameter */
    usbh1_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)0, side);

    /* No Data */
    hmsc_retval = usbh1_hmsc_no_data(side);

    return hmsc_retval;
}   /* eof R_USBH1_HmscTestUnit() */

/******************************************************************************
Function Name   : R_USBH1_HmscRequestSense
Description     : RequestSense
Arguments       : uint16_t side :
                : uint8_t *buff :
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscRequestSense(uint16_t side, uint8_t *p_buff)
{
    uint8_t     cbwcb[12];
    uint8_t     length = 18;
    uint16_t    hmsc_retval;

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)12, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USBH1_ATAPI_REQUEST_SENSE;

    /* Allocation length */
    cbwcb[4] = length;

    /* Set CBW parameter */
    usbh1_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)length, side);

    /* Data IN */
    hmsc_retval = usbh1_hmsc_data_in(side, p_buff, (uint32_t)length);

    return hmsc_retval;
}   /* eof R_USBH1_HmscRequestSense() */

/******************************************************************************
Function Name   : R_USBH1_HmscInquiry
Description     : Inquiry
Arguments       : uint16_t side :
                : uint8_t *buff :
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscInquiry(uint16_t side, uint8_t *p_buff)
{
    uint8_t     cbwcb[12];
    uint8_t     length = 36;
    uint16_t    hmsc_retval;

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)12, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USBH1_ATAPI_INQUIRY;

    /* Allocation length */
    cbwcb[4] = length;

    /* Set CBW parameter */
    usbh1_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)length, side);

    /* Data IN */
    hmsc_retval = usbh1_hmsc_data_in(side, p_buff, (uint32_t)length);

    return hmsc_retval;
}   /* eof R_USBH1_HmscInquiry() */

/******************************************************************************
Function Name   : R_USBH1_HmscReadCapacity
Description     : ReadCapacity
Arguments       : uint16_t side :
                : uint8_t *buff :
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscReadCapacity(uint16_t side, uint8_t *p_buff)
{
    uint8_t     cbwcb[12];
    uint8_t     length = 8;
    uint16_t    hmsc_retval;

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)12, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USBH1_ATAPI_READ_CAPACITY;

    /* Set CBW parameter */
    usbh1_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)length, side);

    /* Data IN */
    hmsc_retval = usbh1_hmsc_data_in(side, p_buff, (uint32_t)length);

    return hmsc_retval;
}   /* eof R_USBH1_HmscReadCapacity() */

/******************************************************************************
Function Name   : R_USBH1_HmscReadFormatCapacity
Description     : ReadFormatCapacity
Arguments       : uint16_t side :
                : uint8_t *buff :
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscReadFormatCapacity(uint16_t side, uint8_t *p_buff)
{
    uint8_t     cbwcb[12];
    uint8_t     length = 0x20;
    uint16_t    hmsc_retval;

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)12, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USBH1_ATAPI_READ_FORMAT_CAPACITY;

    /* Allocation length */
    cbwcb[8] = length;

    /* Set CBW parameter */
    usbh1_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)length, side);

    /* Data IN */
    hmsc_retval = usbh1_hmsc_data_in(side, p_buff, (uint32_t)length);

    return hmsc_retval;
}   /* eof R_USBH1_HmscReadFormatCapacity() */

/******************************************************************************
Function Name   : R_USBH1_HmscGetMaxUnit
Description     : Get Max LUN request
Argument        : uint16_t addr             : Device Address
                : usbh1_utr_cb_t complete     : CallBack Function
Return value    : usbh1_er_t                  : Error Code
******************************************************************************/
usbh1_er_t R_USBH1_HmscGetMaxUnit(uint16_t addr, uint8_t *buff, usbh1_utr_cb_t complete)
{
    static st_usbh1_setup_t   get_max_lun_table;
    usbh1_er_t                err;

    get_max_lun_table.type      = 0xFEA1;
    get_max_lun_table.value     = 0x0000;
    get_max_lun_table.index     = 0x0000;
    get_max_lun_table.length    = 0x0001;
    get_max_lun_table.devaddr   = addr;

    /* Recieve MaxLUN */
    g_usbh1_hmsc_class_control.keyword    = USBH1_PIPE0;
    g_usbh1_hmsc_class_control.p_tranadr    = buff;
    g_usbh1_hmsc_class_control.tranlen    = get_max_lun_table.length;
    g_usbh1_hmsc_class_control.p_setup      = &get_max_lun_table;
    g_usbh1_hmsc_class_control.complete   = complete;

    err = R_USBH1_HstdTransferStart(&g_usbh1_hmsc_class_control);

    return err;
}   /* eof R_USBH1_HmscGetMaxUnit() */

/******************************************************************************
Function Name   : R_USBH1_HmscMassStorageReset
Description     : Mass Strage Reset request
Argument        : uint16_t addr             : Device Address
                : usbh1_utr_cb_t complete     : CallBack Function
Return value    : usbh1_er_t                  : Error Code
******************************************************************************/
usbh1_er_t R_USBH1_HmscMassStorageReset(uint16_t addr, usbh1_utr_cb_t complete)
{
    static st_usbh1_setup_t   mass_storage_reset_table;
    usbh1_er_t                err;


    mass_storage_reset_table.type       = 0xFF21;
    mass_storage_reset_table.value      = 0x0000;
    mass_storage_reset_table.index      = 0x0000;
    mass_storage_reset_table.length     = 0x0000;
    mass_storage_reset_table.devaddr    = addr;

    /* Set MassStorageReset */
    g_usbh1_hmsc_class_control.keyword    = USBH1_PIPE0;
    g_usbh1_hmsc_class_control.p_tranadr    = USBH1_NULL;
    g_usbh1_hmsc_class_control.tranlen    = mass_storage_reset_table.length;
    g_usbh1_hmsc_class_control.p_setup      = &mass_storage_reset_table;
    g_usbh1_hmsc_class_control.complete   = complete;

    err = R_USBH1_HstdTransferStart(&g_usbh1_hmsc_class_control);

    return err;
}   /* eof R_USBH1_HmscMassStorageReset() */

/******************************************************************************
Function Name   : R_USBH1_HmscPreventAllow
Description     : PreventAllow
Arguments       : uint16_t side :
                : uint8_t *buff :
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscPreventAllow(uint16_t side, uint8_t *p_buff)
{
    uint8_t     cbwcb[12];
    uint16_t    hmsc_retval;

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)12, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USBH1_ATAPI_PREVENT_ALLOW;

    /* Reserved */
    cbwcb[4] = p_buff[0];

    /* Set CBW parameter */
    usbh1_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)0, side);

    /* No Data */
    hmsc_retval = usbh1_hmsc_no_data(side);

    return hmsc_retval;
}   /* eof R_USBH1_HmscPreventAllow() */

/******************************************************************************
Function Name   : R_USBH1_HmscModeSense10
Description     : ModeSense10
Arguments       : uint16_t side :
                : uint8_t *buff :
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscModeSense10(uint16_t side, uint8_t *p_buff)
{
    uint8_t     cbwcb[12];
    uint8_t     length = 26;
    uint16_t    hmsc_retval;

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)12, cbwcb);

    /* Data set */
    cbwcb[0] = USBH1_ATAPI_MODE_SENSE10;  /* Command */
    cbwcb[1] = 0x08;                    /* Set LUN / DBD=1 */
    cbwcb[7] = 0x00;                    /* Allocation length */
    cbwcb[8] = 0x02;                    /* Allocation length */

    /* Set CBW parameter */
    usbh1_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)length, side);

    /* Data IN */
    hmsc_retval = usbh1_hmsc_data_in(side, p_buff, (uint32_t)length);

    return hmsc_retval;
}   /* eof R_USBH1_HmscModeSense10() */

/******************************************************************************
Function Name   : R_USBH1_HmscModeSelect6
Description     : ModeSelect6
Arguments       : uint16_t side :
                : uint8_t *buff :
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscModeSelect6(uint16_t side, uint8_t *p_buff)
{
    uint8_t     cbwcb[12];
    uint8_t     length = 18;
    uint16_t    hmsc_retval;

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)12, cbwcb);

    /* Data set */
    cbwcb[0] = USBH1_ATAPI_MODE_SELECT6;  /* Command */
    cbwcb[1] = 0x10;                    /* SP=0 */
    cbwcb[4] = 0x18;                    /* Parameter list length ??? */

    /* Set CBW parameter */
    usbh1_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)length, side);

    /* Data clear */
    usbh1_hmsc_clr_data((uint16_t)24, p_buff);

    /* Data set */
    p_buff[3]   = 0x08;
    p_buff[10]  = 0x02;
    p_buff[12]  = 0x08;
    p_buff[13]  = 0x0A;

    /* Data OUT */
    hmsc_retval = usbh1_hmsc_data_out(side, p_buff, (uint32_t)length);

    return hmsc_retval;
}   /* eof R_USBH1_HmscModeSelect6() */

/******************************************************************************
Function Name   : R_USBH1_HmscGetDevSts
Description     : Responds to HMSCD operation state
Arguments       : none
Return value    : uint16_t :
******************************************************************************/
uint16_t R_USBH1_HmscGetDevSts(uint16_t side)
{
    return g_usbh1_hmsc_drvno_tbl[side].use_flag;
}   /* eof R_USBH1_HmscGetDevSts() */

/******************************************************************************
Function Name   : R_USBH1_HmscAllocDrvNo
Description     : Alloc Drive no.
Arguments       : uint16_t *side        : Drive number
                : uint16_t devadr       : Device address
Return value    : Drive no.
******************************************************************************/
uint16_t R_USBH1_HmscAllocDrvNo(uint16_t *p_side, uint16_t devadr)
{
    uint16_t    i;

    for (i = 0; i < USBH1_MAXSTRAGE ; i++)
    {
        if (USBH1_FALSE == g_usbh1_hmsc_drvno_tbl[i].use_flag)
        {
            g_usbh1_hmsc_drvno_tbl[i].use_flag    = USBH1_TRUE;
            g_usbh1_hmsc_drvno_tbl[i].devadr      = devadr;
            *p_side = i;

            return USBH1_OK;
        }
    }

    return USBH1_ERROR;
} /* eof R_USBH1_HmscAllocDrvNo() */

/******************************************************************************
Function Name   : R_USBH1_HmscFreeDrvNo
Description     : Release Drive no.
Arguments       : uint16_t side        : Drive no.
Return value    : result
******************************************************************************/
uint16_t R_USBH1_HmscFreeDrvNo(uint16_t side)
{
    if (side > USBH1_MAXSTRAGE)
    {
        return USBH1_ERROR;
    }

    g_usbh1_hmsc_drvno_tbl[side].use_flag = USBH1_FALSE;

    return USBH1_OK;
} /* eof R_USBH1_HmscFreeDrvNo() */

/******************************************************************************
Function Name   : R_USBH1_HmscRefDrvNo
Description     : Get Drive no.
Arguments       : uint16_t *side        : Drive number
                : uint16_t devadr       : Device address
Return value    : Drive no.
******************************************************************************/
uint16_t R_USBH1_HmscRefDrvNo(uint16_t *p_side, uint16_t devadr)
{
    uint16_t    i;

    for (i = 0; i < USBH1_MAXSTRAGE ; i++)
    {
        if (devadr == g_usbh1_hmsc_drvno_tbl[i].devadr)
        {
            *p_side = i;

            return USBH1_OK;
        }
    }

    return USBH1_ERROR;
} /* eof R_USBH1_HmscRefDrvNo() */


/******************************************************************************
End  Of File
******************************************************************************/
