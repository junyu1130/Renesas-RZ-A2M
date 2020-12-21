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
 *
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name    : r_rza2_riic_lld_int.c
 * $Rev: 10756 $
 * $Date:: 2018-12-05 17:54:33 +0900#$
 * Device(s)    : RZ/A2M
 * Tool-Chain   : e2Studio Ver 7.1.0
 *              : GCC ARM Embedded 5.4.1.20160919
 * OS           : None
 * H/W Platform : RZ/A2M Evaluation Board
 * Description  : RZ/A2M RIIC Driver - Interrupt control of RIIC module
 * Operation    :
 * Limitations  :
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/* System Includes */
#include "r_typedefs.h"
#include "iodefine.h"
#include "iobitmask.h"
#include "rza_io_regrw.h"              /* Low level register read/write header */

/* Driver Includes */
#include "driver.h"                    /* Device driver header */
#include "r_riic_hld_api.h"            /* High level API header for callback function */
#include "r_riic_drv_api.h"            /* High level Driver header for callback funtion */
#include "r_rza2m_riic_lld_api.h"      /* Low level Driver Header */
#include "r_rza2m_riic_lld_cfg.h"      /* Low level Driver Configuration Header */
#include "r_rza2m_riic_lld_prv.h"      /* Low level Driver Header */
#include "r_intc_drv_api.h"            /* Interrupt Driver API Header */

/*******************************************************************************
 Private global variables and functions
 *******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedefs
 *******************************************************************************/
/* To make casting to the ISR prototype expected by the Renesas GIC drivers. */
typedef void (*ISR_FUNCTION) (uint32_t);

/******************************************************************************
 Private function definitions
 ******************************************************************************/
static void INTRIICTEI_HandlerProc (uint32_t channel);
static void INTRIIC0TEI_Handler (void);
static void INTRIIC1TEI_Handler (void);
static void INTRIIC2TEI_Handler (void);
static void INTRIIC3TEI_Handler (void);

static void INTRIICRI_HandlerProc (uint32_t channel);
static void INTRIIC0RI_Handler (void);
static void INTRIIC1RI_Handler (void);
static void INTRIIC2RI_Handler (void);
static void INTRIIC3RI_Handler (void);

static void INTRIICTI_HandlerProc (uint32_t channel);
static void INTRIIC0TI_Handler (void);
static void INTRIIC1TI_Handler (void);
static void INTRIIC2TI_Handler (void);
static void INTRIIC3TI_Handler (void);

static void INTRIICSTI_HandlerProc (uint32_t channel);
static void INTRIIC0STI_Handler (void);
static void INTRIIC1STI_Handler (void);
static void INTRIIC2STI_Handler (void);
static void INTRIIC3STI_Handler (void);

static void INTRIICSPI_HandlerProc (uint32_t channel);
static void INTRIIC0SPI_Handler (void);
static void INTRIIC1SPI_Handler (void);
static void INTRIIC2SPI_Handler (void);
static void INTRIIC3SPI_Handler (void);

static void INTRIICNAKI_HandlerProc (uint32_t channel);
static void INTRIIC0NAKI_Handler (void);
static void INTRIIC1NAKI_Handler (void);
static void INTRIIC2NAKI_Handler (void);
static void INTRIIC3NAKI_Handler (void);

static void INTRIICALI_HandlerProc (uint32_t channel);
static void INTRIIC0ALI_Handler (void);
static void INTRIIC1ALI_Handler (void);
static void INTRIIC2ALI_Handler (void);
static void INTRIIC3ALI_Handler (void);

static void INTRIICTMOI_HandlerProc (uint32_t channel);
static void INTRIIC0TMOI_Handler (void);
static void INTRIIC1TMOI_Handler (void);
static void INTRIIC2TMOI_Handler (void);
static void INTRIIC3TMOI_Handler (void);

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static volatile struct st_riic *gsp_riic[RIIC_CFG_LLD_NUM_CHANNELS] =
{
/* RIIC Channels */
&RIIC30, &RIIC31, &RIIC32, &RIIC33 };

/* RIIC interrupt info */

/* interrupt information table define */
static const st_riic_int_param_t gs_riic_int_info[RIIC_CFG_LLD_NUM_CHANNELS][RIIC_INT_MAX] =
{
{ /* ch0 */
{ (uint32_t) &INTRIIC0TEI_Handler, INTC_ID_IIC_INTRIICTEI0, INT_SENSE_LEVEL }, /* TEI */
{ (uint32_t) &INTRIIC0RI_Handler, INTC_ID_IIC_INTRIICRI0, INT_SENSE_EDGE }, /* RI */
{ (uint32_t) &INTRIIC0TI_Handler, INTC_ID_IIC_INTRIICTI0, INT_SENSE_EDGE }, /* TI */
{ (uint32_t) &INTRIIC0SPI_Handler, INTC_ID_IIC_INTRIICSPI0, INT_SENSE_LEVEL }, /* SPI */
{ (uint32_t) &INTRIIC0STI_Handler, INTC_ID_IIC_INTRIICSTI0, INT_SENSE_LEVEL }, /* STI */
{ (uint32_t) &INTRIIC0NAKI_Handler, INTC_ID_IIC_INTRIICNAKI0, INT_SENSE_LEVEL }, /* NAKI */
{ (uint32_t) &INTRIIC0ALI_Handler, INTC_ID_IIC_INTRIICALI0, INT_SENSE_LEVEL }, /* ALI */
{ (uint32_t) &INTRIIC0TMOI_Handler, INTC_ID_IIC_INTRIICTMOI0, INT_SENSE_LEVEL } /* TMOI */
},

{ /* ch1 */
{ (uint32_t) &INTRIIC1TEI_Handler, INTC_ID_IIC_INTRIICTEI1, INT_SENSE_LEVEL }, /* TEI */
{ (uint32_t) &INTRIIC1RI_Handler, INTC_ID_IIC_INTRIICRI1, INT_SENSE_EDGE }, /* RI */
{ (uint32_t) &INTRIIC1TI_Handler, INTC_ID_IIC_INTRIICTI1, INT_SENSE_EDGE }, /* TI */
{ (uint32_t) &INTRIIC1SPI_Handler, INTC_ID_IIC_INTRIICSPI1, INT_SENSE_LEVEL }, /* SPI */
{ (uint32_t) &INTRIIC1STI_Handler, INTC_ID_IIC_INTRIICSTI1, INT_SENSE_LEVEL }, /* STI */
{ (uint32_t) &INTRIIC1NAKI_Handler, INTC_ID_IIC_INTRIICNAKI1, INT_SENSE_LEVEL }, /* NAKI */
{ (uint32_t) &INTRIIC1ALI_Handler, INTC_ID_IIC_INTRIICALI1, INT_SENSE_LEVEL }, /* ALI */
{ (uint32_t) &INTRIIC1TMOI_Handler, INTC_ID_IIC_INTRIICTMOI1, INT_SENSE_LEVEL } /* TMOI */
},

{ /* ch2 */
{ (uint32_t) &INTRIIC2TEI_Handler, INTC_ID_IIC_INTRIICTEI2, INT_SENSE_LEVEL }, /* TEI */
{ (uint32_t) &INTRIIC2RI_Handler, INTC_ID_IIC_INTRIICRI2, INT_SENSE_EDGE }, /* RI */
{ (uint32_t) &INTRIIC2TI_Handler, INTC_ID_IIC_INTRIICTI2, INT_SENSE_EDGE }, /* TI */
{ (uint32_t) &INTRIIC2SPI_Handler, INTC_ID_IIC_INTRIICSPI2, INT_SENSE_LEVEL }, /* SPI */
{ (uint32_t) &INTRIIC2STI_Handler, INTC_ID_IIC_INTRIICSTI2, INT_SENSE_LEVEL }, /* STI */
{ (uint32_t) &INTRIIC2NAKI_Handler, INTC_ID_IIC_INTRIICNAKI2, INT_SENSE_LEVEL }, /* NAKI */
{ (uint32_t) &INTRIIC2ALI_Handler, INTC_ID_IIC_INTRIICALI2, INT_SENSE_LEVEL }, /* ALI */
{ (uint32_t) &INTRIIC2TMOI_Handler, INTC_ID_IIC_INTRIICTMOI2, INT_SENSE_LEVEL } /* TMOI */
},

{ /* ch3 */
{ (uint32_t) &INTRIIC3TEI_Handler, INTC_ID_IIC_INTRIICTEI3, INT_SENSE_LEVEL }, /* TEI */
{ (uint32_t) &INTRIIC3RI_Handler, INTC_ID_IIC_INTRIICRI3, INT_SENSE_EDGE }, /* RI */
{ (uint32_t) &INTRIIC3TI_Handler, INTC_ID_IIC_INTRIICTI3, INT_SENSE_EDGE }, /* TI */
{ (uint32_t) &INTRIIC3SPI_Handler, INTC_ID_IIC_INTRIICSPI3, INT_SENSE_LEVEL }, /* SPI */
{ (uint32_t) &INTRIIC3STI_Handler, INTC_ID_IIC_INTRIICSTI3, INT_SENSE_LEVEL }, /* STI */
{ (uint32_t) &INTRIIC3NAKI_Handler, INTC_ID_IIC_INTRIICNAKI3, INT_SENSE_LEVEL }, /* NAKI */
{ (uint32_t) &INTRIIC3ALI_Handler, INTC_ID_IIC_INTRIICALI3, INT_SENSE_LEVEL }, /* ALI */
{ (uint32_t) &INTRIIC3TMOI_Handler, INTC_ID_IIC_INTRIICTMOI3, INT_SENSE_LEVEL } /* TMOI */
} };

/* Prototypes */

/******************************************************************************
 Private function definitions
 ******************************************************************************/

/**
 * @brief interrupt_init Initialise the interrupt 
 *
 * @param[in] channel  The RIIC channel on initialising interrupt
 *
 * @param[in] *p_cfg   The RIIC channel configration information
 *
 * @return None
 */
void interrupt_init (const int_t channel, const st_riic_config_t *p_cfg)
{
    /* cast to ISR_FUNCTION */
    R_INTC_RegistIntFunc(gs_riic_int_info[channel][RIIC_INT_TEI].int_num,
            (ISR_FUNCTION ) gs_riic_int_info[channel][RIIC_INT_TEI].int_handler);
    R_INTC_Enable(gs_riic_int_info[channel][RIIC_INT_TEI].int_num);
    R_INTC_SetPriority(gs_riic_int_info[channel][RIIC_INT_TEI].int_num, p_cfg->tei_priority);

    /* cast to ISR_FUNCTION */
    R_INTC_RegistIntFunc(gs_riic_int_info[channel][RIIC_INT_RI].int_num,
            (ISR_FUNCTION ) gs_riic_int_info[channel][RIIC_INT_RI].int_handler);
    R_INTC_Enable(gs_riic_int_info[channel][RIIC_INT_RI].int_num);
    R_INTC_SetPriority(gs_riic_int_info[channel][RIIC_INT_RI].int_num, p_cfg->ri_priority);

    /* cast to ISR_FUNCTION */
    R_INTC_RegistIntFunc(gs_riic_int_info[channel][RIIC_INT_TI].int_num,
            (ISR_FUNCTION ) gs_riic_int_info[channel][RIIC_INT_TI].int_handler);
    R_INTC_Enable(gs_riic_int_info[channel][RIIC_INT_TI].int_num);
    R_INTC_SetPriority(gs_riic_int_info[channel][RIIC_INT_TI].int_num, p_cfg->ti_priority);

    /* cast to ISR_FUNCTION */
    R_INTC_RegistIntFunc(gs_riic_int_info[channel][RIIC_INT_STI].int_num,
            (ISR_FUNCTION ) gs_riic_int_info[channel][RIIC_INT_STI].int_handler);
    R_INTC_Enable(gs_riic_int_info[channel][RIIC_INT_STI].int_num);
    R_INTC_SetPriority(gs_riic_int_info[channel][RIIC_INT_STI].int_num, p_cfg->sti_priority);

    /* cast to ISR_FUNCTION */
    R_INTC_RegistIntFunc(gs_riic_int_info[channel][RIIC_INT_SPI].int_num,
            (ISR_FUNCTION ) gs_riic_int_info[channel][RIIC_INT_SPI].int_handler);
    R_INTC_Enable(gs_riic_int_info[channel][RIIC_INT_SPI].int_num);
    R_INTC_SetPriority(gs_riic_int_info[channel][RIIC_INT_SPI].int_num, p_cfg->spi_priority);

    /* cast to ISR_FUNCTION */
    R_INTC_RegistIntFunc(gs_riic_int_info[channel][RIIC_INT_NAKI].int_num,
            (ISR_FUNCTION ) gs_riic_int_info[channel][RIIC_INT_NAKI].int_handler);
    R_INTC_Enable(gs_riic_int_info[channel][RIIC_INT_NAKI].int_num);
    R_INTC_SetPriority(gs_riic_int_info[channel][RIIC_INT_NAKI].int_num, p_cfg->naki_priority);

    /* cast to ISR_FUNCTION */
    R_INTC_RegistIntFunc(gs_riic_int_info[channel][RIIC_INT_ALI].int_num,
            (ISR_FUNCTION ) gs_riic_int_info[channel][RIIC_INT_ALI].int_handler);
    R_INTC_Enable(gs_riic_int_info[channel][RIIC_INT_ALI].int_num);
    R_INTC_SetPriority(gs_riic_int_info[channel][RIIC_INT_ALI].int_num, p_cfg->ali_priority);

    /* cast to ISR_FUNCTION */
    R_INTC_RegistIntFunc(gs_riic_int_info[channel][RIIC_INT_TMOI].int_num,
            (ISR_FUNCTION ) gs_riic_int_info[channel][RIIC_INT_TMOI].int_handler);
    R_INTC_Enable(gs_riic_int_info[channel][RIIC_INT_TMOI].int_num);
    R_INTC_SetPriority(gs_riic_int_info[channel][RIIC_INT_TMOI].int_num, p_cfg->tmoi_priority);
}
/******************************************************************************
 * End of Function interrupt_init
 ******************************************************************************/

/**
 * @brief interrupt_uninit Uninitialise the interrupt 
 *
 * @param[in] channel  The RIIC channel on initialising interrupt
 *
 * @return None
 */
void interrupt_uninit (const int_t channel)
{
    e_riic_int_type_t int_type;

    for (int_type = RIIC_INT_TEI; int_type < RIIC_INT_MAX; int_type++)
    {
        R_INTC_Disable(gs_riic_int_info[channel][int_type].int_num);
        R_INTC_RegistIntFunc(gs_riic_int_info[channel][int_type].int_num, NULL);
    }
}
/******************************************************************************
 * End of Function interrupt_uninit
 ******************************************************************************/

/**
 * @brief INTRIICTEI_HandlerProc Process of TEI interrupt handler 
 *
 * @param[in] channel  The RIIC channel with TEI interrupt
 *
 * @return None
 */
static void INTRIICTEI_HandlerProc (uint32_t channel)
{
    uint32_t dummy;

    r_riic_hld_set_tx_end((int_t) channel);

    /* clear interrupt source */
    dummy = RZA_IO_RegRead_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, RIIC_ICSR2_TEND_SHIFT,
            RIIC_ICSR2_TEND);

    /* cast to volatile uint32_t * */
    RZA_IO_RegWrite_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, 0, RIIC_ICSR2_TEND_SHIFT, RIIC_ICSR2_TEND);

    UNUSED_PARAM(dummy);

}
/******************************************************************************
 * End of Function INTRIICTEI_HandlerProc
 ******************************************************************************/

/**
 * @brief INTRIIC0TEI_Handler TEI interrupt handler for RIIC0
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC0TEI_Handler (void)
{

    INTRIICTEI_HandlerProc(RIIC_CH0);

}
/******************************************************************************
 * End of Function INTRIIC0TEI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC1TEI_Handler TEI interrupt handler for RIIC1
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC1TEI_Handler (void)
{
    INTRIICTEI_HandlerProc(RIIC_CH1);

}
/******************************************************************************
 * End of Function INTRIIC1TEI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC2TEI_Handler TEI interrupt handler for RIIC2
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC2TEI_Handler (void)
{
    INTRIICTEI_HandlerProc(RIIC_CH2);

}
/******************************************************************************
 * End of Function INTRIIC2TEI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC3TEI_Handler TEI interrupt handler for RIIC3
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC3TEI_Handler (void)
{
    INTRIICTEI_HandlerProc(RIIC_CH3);

}
/******************************************************************************
 * End of Function INTRIIC3TEI_Handler
 ******************************************************************************/

/**
 * @brief INTRIICRI_HandlerProc Process of RI interrupt handler 
 *
 * @param[in] channel  The RIIC channel with RI interrupt
 *
 * @return None
 */
static void INTRIICRI_HandlerProc (uint32_t channel)
{
    /* cast channel to int_t */
    r_riic_hld_set_rx_full((int_t) channel);
}
/******************************************************************************
 * End of Function INTRIICRI_HandlerProc
 ******************************************************************************/

/**
 * @brief INTRIIC0TEI_Handler RI interrupt handler for RIIC0
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC0RI_Handler (void)
{
    INTRIICRI_HandlerProc(RIIC_CH0);

}
/******************************************************************************
 * End of Function INTRIIC0RI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC1RI_Handler RI interrupt handler for RIIC1
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC1RI_Handler (void)
{
    INTRIICRI_HandlerProc(RIIC_CH1);

}
/******************************************************************************
 * End of Function INTRIIC1RI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC0RI_Handler RI interrupt handler for RIIC2
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC2RI_Handler (void)
{
    INTRIICRI_HandlerProc(RIIC_CH2);

}
/******************************************************************************
 * End of Function INTRIIC2RI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC3RI_Handler RI interrupt handler for RIIC3
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC3RI_Handler (void)
{
    INTRIICRI_HandlerProc(RIIC_CH3);

}
/******************************************************************************
 * End of Function INTRIIC3RI_Handler
 ******************************************************************************/

/**
 * @brief INTRIICTI_HandlerProc Process of TI interrupt handler 
 *
 * @param[in] channel  The RIIC channel with TI interrupt
 *
 * @return None
 */
static void INTRIICTI_HandlerProc (uint32_t channel)
{
    /* cast to int_t */
    r_riic_hld_set_tx_empty((int_t) channel);

}
/******************************************************************************
 * End of Function INTRIICTI_HandlerProc
 ******************************************************************************/

/**
 * @brief INTRIIC0TI_Handler TI interrupt handler for RIIC0
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC0TI_Handler (void)
{
    INTRIICTI_HandlerProc(RIIC_CH0);

}
/******************************************************************************
 * End of Function INTRIIC0TI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC0TEI_Handler TI interrupt handler for RIIC1
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC1TI_Handler (void)
{
    INTRIICTI_HandlerProc(RIIC_CH1);

}
/******************************************************************************
 * End of Function INTRIIC1TI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC2TI_Handler TI interrupt handler for RIIC2
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC2TI_Handler (void)
{
    INTRIICTI_HandlerProc(RIIC_CH2);

}
/******************************************************************************
 * End of Function INTRIIC2TI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC0TEI_Handler TI interrupt handler for RIIC3
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC3TI_Handler (void)
{
    INTRIICTI_HandlerProc(RIIC_CH3);

}
/******************************************************************************
 * End of Function INTRIIC3TI_Handler
 ******************************************************************************/

/**
 * @brief INTRIICSTI_HandlerProc Process of STI interrupt handler 
 *
 * @param[in] channel  The RIIC channel with STI interrupt
 *
 * @return None
 */
static void INTRIICSTI_HandlerProc (uint32_t channel)
{
    uint32_t dummy;

    /* cast to int_t */
    r_riic_hld_set_start_asserted((int_t) channel);

    /* clear interrupt source */
    dummy = RZA_IO_RegRead_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, RIIC_ICSR2_START_SHIFT,
            RIIC_ICSR2_START);

    /* cast to volatile uint32_t * */
    RZA_IO_RegWrite_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, 0, RIIC_ICSR2_START_SHIFT,
            RIIC_ICSR2_START);

    UNUSED_PARAM(dummy);

}
/******************************************************************************
 * End of Function INTRIICSTI_HandlerProc
 ******************************************************************************/

/**
 * @brief INTRIIC0STI_Handler STI interrupt handler for RIIC0
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC0STI_Handler (void)
{
    INTRIICSTI_HandlerProc(RIIC_CH0);

}
/******************************************************************************
 * End of Function INTRIIC0STI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC1STI_Handler STI interrupt handler for RIIC1
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC1STI_Handler (void)
{
    INTRIICSTI_HandlerProc(RIIC_CH1);

}
/******************************************************************************
 * End of Function INTRIIC1STI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC2STI_Handler STI interrupt handler for RIIC2
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC2STI_Handler (void)
{
    INTRIICSTI_HandlerProc(RIIC_CH2);

}
/******************************************************************************
 * End of Function INTRIIC2STI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC3STI_Handler STI interrupt handler for RIIC3
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC3STI_Handler (void)
{
    INTRIICSTI_HandlerProc(RIIC_CH3);

}
/******************************************************************************
 * End of Function INTRIIC3STI_Handler
 ******************************************************************************/

/**
 * @brief INTRIICSPI_HandlerProc Process of SPI interrupt handler 
 *
 * @param[in] channel  The RIIC channel with SPI interrupt
 *
 * @return None
 */
static void INTRIICSPI_HandlerProc (uint32_t channel)
{
    uint32_t dummy;

    /* cast to int_t */
    r_riic_hld_set_stop_asserted((int_t) channel);

    /* clear interrupt source */
    dummy = RZA_IO_RegRead_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, RIIC_ICSR2_STOP_SHIFT,
            RIIC_ICSR2_STOP);

    /* cast to volatile uint32_t * */
    RZA_IO_RegWrite_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, 0, RIIC_ICSR2_STOP_SHIFT, RIIC_ICSR2_STOP);

    UNUSED_PARAM(dummy);
}
/******************************************************************************
 * End of Function INTRIICSPI_HandlerProc
 ******************************************************************************/

/**
 * @brief INTRIIC0SPI_Handler SPI interrupt handler for RIIC0
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC0SPI_Handler (void)
{
    INTRIICSPI_HandlerProc(RIIC_CH0);
}
/******************************************************************************
 * End of Function INTRIIC0SPI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC1SPI_Handler SPI interrupt handler for RIIC1
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC1SPI_Handler (void)
{
    INTRIICSPI_HandlerProc(RIIC_CH1);

}
/******************************************************************************
 * End of Function INTRIIC1SPI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC2SPI_Handler SPI interrupt handler for RIIC2
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC2SPI_Handler (void)
{
    INTRIICSPI_HandlerProc(RIIC_CH2);

}
/******************************************************************************
 * End of Function INTRIIC2SPI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC3SPI_Handler SPI interrupt handler for RIIC3
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC3SPI_Handler (void)
{
    INTRIICSPI_HandlerProc(RIIC_CH3);

}
/******************************************************************************
 * End of Function INTRIIC3SPI_Handler
 ******************************************************************************/

/**
 * @brief INTRIICALI_HandlerProc Process of ALI interrupt handler 
 *
 * @param[in] channel  The RIIC channel with ALI interrupt
 *
 * @return None
 */
static void INTRIICALI_HandlerProc (uint32_t channel)
{
    uint32_t dummy;

    /* cast to int_t */
    r_riic_hld_set_arbitrate_assert((int_t) channel);

    /* clear interrupt source */
    dummy = RZA_IO_RegRead_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, RIIC_ICSR2_AL_SHIFT, RIIC_ICSR2_AL);

    /* cast to volatile uint32_t * */
    RZA_IO_RegWrite_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, 0, RIIC_ICSR2_AL_SHIFT, RIIC_ICSR2_AL);

    UNUSED_PARAM(dummy);
}
/******************************************************************************
 * End of Function INTRIICALI_HandlerProc
 ******************************************************************************/

/**
 * @brief INTRIIC0ALI_Handler ALI interrupt handler for RIIC0
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC0ALI_Handler (void)
{
    INTRIICALI_HandlerProc(RIIC_CH0);
}
/******************************************************************************
 * End of Function INTRIIC0ALI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC1ALI_Handler ALI interrupt handler for RIIC1
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC1ALI_Handler (void)
{
    INTRIICALI_HandlerProc(RIIC_CH1);

}
/******************************************************************************
 * End of Function INTRIIC1ALI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC2ALI_Handler ALI interrupt handler for RIIC2
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC2ALI_Handler (void)
{
    INTRIICALI_HandlerProc(RIIC_CH2);

}
/******************************************************************************
 * End of Function INTRIIC2ALI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC3ALI_Handler ALI interrupt handler for RIIC3
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC3ALI_Handler (void)
{
    INTRIICALI_HandlerProc(RIIC_CH3);

}
/******************************************************************************
 * End of Function INTRIIC3ALI_Handler
 ******************************************************************************/

/**
 * @brief INTRIICNAKI_HandlerProc Process of NAKI interrupt handler 
 *
 * @param[in] channel  The RIIC channel with NAKI interrupt
 *
 * @return None
 */
static void INTRIICNAKI_HandlerProc (uint32_t channel)
{
    uint32_t dummy;

    /* cast to int_t*/
    r_riic_hld_set_nack_asserted((int_t) channel);

    /* clear interrupt source */
    dummy = RZA_IO_RegRead_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, RIIC_ICSR2_NACKF_SHIFT,
            RIIC_ICSR2_NACKF);

    /* cast to volatile uint32_t * */
    RZA_IO_RegWrite_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, 0, RIIC_ICSR2_NACKF_SHIFT,
            RIIC_ICSR2_NACKF);

    UNUSED_PARAM(dummy);
}
/******************************************************************************
 * End of Function INTRIICNAKI_HandlerProc
 ******************************************************************************/

/**
 * @brief INTRIIC0NAKI_Handler NAKI interrupt handler for RIIC0
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC0NAKI_Handler (void)
{
    INTRIICNAKI_HandlerProc(RIIC_CH0);
}
/******************************************************************************
 * End of Function INTRIIC0NAKI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC1NAKI_Handler NAKI interrupt handler for RIIC1
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC1NAKI_Handler (void)
{
    INTRIICNAKI_HandlerProc(RIIC_CH1);

}
/******************************************************************************
 * End of Function INTRIIC1NAKI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC2NAKI_Handler NAKI interrupt handler for RIIC2
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC2NAKI_Handler (void)
{
    INTRIICNAKI_HandlerProc(RIIC_CH2);

}
/******************************************************************************
 * End of Function INTRIIC2NAKI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC3NAKI_Handler TALI interrupt handler for RIIC3
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC3NAKI_Handler (void)
{
    INTRIICNAKI_HandlerProc(RIIC_CH3);

}
/******************************************************************************
 * End of Function INTRIIC3NAKI_Handler
 ******************************************************************************/

/**
 * @brief INTRIICTMOI_HandlerProc Process of TMOI interrupt handler 
 *
 * @param[in] channel  The RIIC channel with TMOI interrupt
 *
 * @return None
 */
static void INTRIICTMOI_HandlerProc (uint32_t channel)
{
    uint32_t dummy;

    /* cast to int_t */
    r_riic_hld_set_timeout_asserted((int_t) channel);

    /* clear interrupt source */
    dummy = RZA_IO_RegRead_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, RIIC_ICSR2_TMOF_SHIFT,
            RIIC_ICSR2_TMOF);

    /* cast to volatile uint32_t * */
    RZA_IO_RegWrite_32((volatile uint32_t *) &gsp_riic[channel]->ICSR2.LONG, 0, RIIC_ICSR2_TMOF_SHIFT, RIIC_ICSR2_TMOF);

    UNUSED_PARAM(dummy);
}
/******************************************************************************
 * End of Function INTRIICTMOI_HandlerProc
 ******************************************************************************/

/**
 * @brief INTRIIC0TEI_Handler TMOI interrupt handler for RIIC0
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC0TMOI_Handler (void)
{
    INTRIICTMOI_HandlerProc(RIIC_CH0);

}
/******************************************************************************
 * End of Function INTRIIC0TMOI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC1TMOI_Handler TMOI interrupt handler for RIIC1
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC1TMOI_Handler (void)
{
    INTRIICTMOI_HandlerProc(RIIC_CH1);

}
/******************************************************************************
 * End of Function INTRIIC1TMOI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC2TMOI_Handler TMOI interrupt handler for RIIC2
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC2TMOI_Handler (void)
{
    INTRIICTMOI_HandlerProc(RIIC_CH2);

}
/******************************************************************************
 * End of Function INTRIIC2TMOI_Handler
 ******************************************************************************/

/**
 * @brief INTRIIC3TMOI_Handler TMOI interrupt handler for RIIC3
 *
 * @param None
 *
 * @return None
 */
static void INTRIIC3TMOI_Handler (void)
{
    INTRIICTEI_HandlerProc(RIIC_CH3);

}
/******************************************************************************
 * End of Function INTRIIC3TMOI_Handler
 ******************************************************************************/

/* End of File */
