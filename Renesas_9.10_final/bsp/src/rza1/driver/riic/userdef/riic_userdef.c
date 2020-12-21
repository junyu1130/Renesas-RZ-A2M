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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name     : riic_userdef.c
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream It! v2 board
 * Description   : RIIC driver (User define function)
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 21.10.2014 1.00
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_typedefs.h"            /* Default type definition header */
#include "iodefine.h"            /* I/O Register root header */
#include "dev_drv.h"            /* Device driver header */

#include "../../intc/inc/devdrv_intc.h"    /* Interrupt Controller driver header */
#include "../inc/r_riic_header.h"        /* RIIC Driver header */
#include "rza_io_regrw.h"        /* Low level register read/write header */
#include "../inc/r_riic_stream_it.h"    /* Interchangeable compiler specific header */
#include "../inc/riic_userdef.h"

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
#define ICC0_MAX_TIMEOUT (20000000L)
#define ICC1_MAX_TIMEOUT (20000000L)
#define ICC2_MAX_TIMEOUT (20000000L)
#define ICC3_MAX_TIMEOUT (20000000L)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static volatile uint8_t riic0_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic0_transmit_end_flg   = DEVDRV_FLAG_OFF;

static volatile uint8_t riic1_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic1_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic1_transmit_end_flg   = DEVDRV_FLAG_OFF;

static volatile uint8_t riic2_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic2_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic2_transmit_end_flg   = DEVDRV_FLAG_OFF;

static volatile uint8_t riic3_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic3_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic3_transmit_end_flg   = DEVDRV_FLAG_OFF;

static void sample_riic_ri0_interrupt (uint32_t int_sense);
static void sample_riic_ti0_interrupt (uint32_t int_sense);
static void sample_riic_tei0_interrupt (uint32_t int_sense);

static void sample_riic_ri1_interrupt (uint32_t int_sense);
static void sample_riic_ti1_interrupt (uint32_t int_sense);
static void sample_riic_tei1_interrupt (uint32_t int_sense);

static void sample_riic_ri2_interrupt (uint32_t int_sense);
static void sample_riic_ti2_interrupt (uint32_t int_sense);
static void sample_riic_tei2_interrupt (uint32_t int_sense);

static void sample_riic_ri3_interrupt (uint32_t int_sense);
static void sample_riic_ti3_interrupt (uint32_t int_sense);
static void sample_riic_tei3_interrupt (uint32_t int_sense);

/******************************************************************************
 * Function Name: userdef_riic0_init
 * Description  : Called to initialise the RIIC channel 0 peripheral
 *              : Uses the following interrupts:
 *              : Receive data full interrupt
 *              : Transmit data empty interrupt
 *              : Transmission complete interrupt
 * Arguments    : uint32_t cks  : I2c Clock Selection
 *              :               : (CKS bit setting in RIICnMR1 register)
 *              :               :   RIIC_CKS_DIVISION_1   : P0 phi/1 clock
 *              :               :   RIIC_CKS_DIVISION_2   : P0 phi/2 clock
 *              :               :   RIIC_CKS_DIVISION_4   : P0 phi/4 clock
 *              :               :   RIIC_CKS_DIVISION_8   : P0 phi/8 clock
 *              :               :   RIIC_CKS_DIVISION_16  : P0 phi/16 clock
 *              :               :   RIIC_CKS_DIVISION_32  : P0 phi/32 clock
 *              :               :   RIIC_CKS_DIVISION_64  : P0 phi/64 clock
 *              :               :   RIIC_CKS_DIVISION_128 : P0 phi/128 clock
 *              : uint32_t brl  : Lower byte value of the width of the SCL clock
 *              : uint32_t brh  : High byte value of the width of the SCL clock
 * Return Value : none
 ******************************************************************************/
void userdef_riic0_init (uint32_t cks, uint32_t brl, uint32_t brh)
{
#ifdef USE_RIIC_CH0
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 0) ---- */
    RZA_IO_RegWrite_8((volatile uint8_t *) &CPG.STBCR9, 0,
            CPG_STBCR9_MSTP97_SHIFT, CPG_STBCR9_MSTP97);

    dummy_buf = RZA_IO_RegRead_8((volatile uint8_t *) &CPG.STBCR9,
            CPG_STBCR9_MSTP97_SHIFT, CPG_STBCR9_MSTP97);

    /* ==== RIIC initial setting ==== */
    /* Clear the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnCR1.UINT8[0]), 0,
            RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnCR1.UINT8[0]), 1,
            RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnCR1.UINT8[0]), 1,
            RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IIC) Selection */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnMR1.UINT8[0]), (uint8_t) cks,
            RIICn_RIICnMR1_CKS_SHIFT, RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnBRL.UINT8[0]), (uint8_t) brl,
            RIICn_RIICnBRL_BRL_SHIFT, RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnBRH.UINT8[0]), (uint8_t) brh,
            RIICn_RIICnBRH_BRH_SHIFT, RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnFER.UINT8[0]), 1,
            RIICn_RIICnFER_NACKE_SHIFT, RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_RIE_SHIFT, RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_TIE_SHIFT, RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_TEIE_SHIFT, RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC0.RIICnCR1.UINT8[0]), 0,
            RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Initialise flags */
    riic0_receive_full_flg = DEVDRV_FLAG_OFF;
    riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
    riic0_transmit_end_flg = DEVDRV_FLAG_OFF;

    /* Register active interrupts */
    R_INTC_RegistIntFunc(INTC_ID_INTIICRI0, sample_riic_ri0_interrupt);
    R_INTC_RegistIntFunc(INTC_ID_INTIICTI0, sample_riic_ti0_interrupt);
    R_INTC_RegistIntFunc(INTC_ID_INTIICTEI0, sample_riic_tei0_interrupt);

    /* Set active interrupt priorities */
    R_INTC_SetPriority(INTC_ID_INTIICRI0, 9);
    R_INTC_SetPriority(INTC_ID_INTIICTI0, 9);
    R_INTC_SetPriority(INTC_ID_INTIICTEI0, 9);

    /* Enable active interrupts */
    R_INTC_Enable(INTC_ID_INTIICRI0);
    R_INTC_Enable(INTC_ID_INTIICTI0);
    R_INTC_Enable(INTC_ID_INTIICTEI0);
#endif
}
/*******************************************************************************
 End of function userdef_riic0_init
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_init
 * Description  : Called to initialise the RIIC channel 1 peripheral
 *              : Uses the following interrupts:
 *              : Receive data full interrupt
 *              : Transmit data empty interrupt
 *              : Transmission complete interrupt
 * Arguments    : uint32_t cks  : I2c Clock Selection
 *              :               : (Settings in the CKS bit of RIICnMR1 register)
 *              :               :   RIIC_CKS_DIVISION_1   : P0 phi/1 clock
 *              :               :   RIIC_CKS_DIVISION_2   : P0 phi/2 clock
 *              :               :   RIIC_CKS_DIVISION_4   : P0 phi/4 clock
 *              :               :   RIIC_CKS_DIVISION_8   : P0 phi/8 clock
 *              :               :   RIIC_CKS_DIVISION_16  : P0 phi/16 clock
 *              :               :   RIIC_CKS_DIVISION_32  : P0 phi/32 clock
 *              :               :   RIIC_CKS_DIVISION_64  : P0 phi/64 clock
 *              :               :   RIIC_CKS_DIVISION_128 : P0 phi/128 clock
 *              : uint32_t brl  : Lower byte value of the width of the SCL clock
 *              : uint32_t brh  : High byte value of the width of the SCL clock
 * Return Value : none
 ******************************************************************************/
void userdef_riic1_init (uint32_t cks, uint32_t brl, uint32_t brh)
{
#ifdef USE_RIIC_CH1
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 1) ---- */
    RZA_IO_RegWrite_8((volatile uint8_t *) &CPG.STBCR9, 0,
    CPG_STBCR9_MSTP96_SHIFT, CPG_STBCR9_MSTP96);

    dummy_buf = RZA_IO_RegRead_8((volatile uint8_t *) &CPG.STBCR9,
    CPG_STBCR9_MSTP96_SHIFT, CPG_STBCR9_MSTP96);

    /* ==== RIIC initial setting ==== */
    /* Clear the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnCR1.UINT8[0]), 0,
    RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnCR1.UINT8[0]), 1,
    RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnCR1.UINT8[0]), 1,
    RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IIC) Selection */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnMR1.UINT8[0]), (uint8_t) cks,
    RIICn_RIICnMR1_CKS_SHIFT, RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnBRL.UINT8[0]), (uint8_t) brl,
    RIICn_RIICnBRL_BRL_SHIFT, RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnBRH.UINT8[0]), (uint8_t) brh,
    RIICn_RIICnBRH_BRH_SHIFT, RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnFER.UINT8[0]), 1,
    RIICn_RIICnFER_NACKE_SHIFT, RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnIER.UINT8[0]), 1,
    RIICn_RIICnIER_RIE_SHIFT, RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnIER.UINT8[0]), 1,
    RIICn_RIICnIER_TIE_SHIFT, RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnIER.UINT8[0]), 1,
    RIICn_RIICnIER_TEIE_SHIFT, RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC1.RIICnCR1.UINT8[0]), 0,
    RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Initialise flags */
    riic1_receive_full_flg = DEVDRV_FLAG_OFF;
    riic1_transmit_empty_flg = DEVDRV_FLAG_OFF;
    riic1_transmit_end_flg = DEVDRV_FLAG_OFF;

    /* Register active interrupts */
    R_INTC_RegistIntFunc(INTC_ID_INTIICRI1, sample_riic_ri1_interrupt);
    R_INTC_RegistIntFunc(INTC_ID_INTIICTI1, sample_riic_ti1_interrupt);
    R_INTC_RegistIntFunc(INTC_ID_INTIICTEI1, sample_riic_tei1_interrupt);

    /* Set active interrupt priorities */
    R_INTC_SetPriority(INTC_ID_INTIICRI1, 9);
    R_INTC_SetPriority(INTC_ID_INTIICTI1, 9);
    R_INTC_SetPriority(INTC_ID_INTIICTEI1, 9);

    /* Enable active interrupts */
    R_INTC_Enable(INTC_ID_INTIICRI1);
    R_INTC_Enable(INTC_ID_INTIICTI1);
    R_INTC_Enable(INTC_ID_INTIICTEI1);
#endif
}
/*******************************************************************************
 End of function userdef_riic1_init
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_init
 * Description  : Called to initialise the RIIC channel 2 peripheral
 *              : Uses the following interrupts:
 *              : Receive data full interrupt
 *              : Transmit data empty interrupt
 *              : Transmission complete interrupt
 * Arguments    : uint32_t cks  : I2c Clock Selection
 *              :               : (Settings in the CKS bit of RIICnMR1 register)
 *              :               :   RIIC_CKS_DIVISION_1   : P0 phi/1 clock
 *              :               :   RIIC_CKS_DIVISION_2   : P0 phi/2 clock
 *              :               :   RIIC_CKS_DIVISION_4   : P0 phi/4 clock
 *              :               :   RIIC_CKS_DIVISION_8   : P0 phi/8 clock
 *              :               :   RIIC_CKS_DIVISION_16  : P0 phi/16 clock
 *              :               :   RIIC_CKS_DIVISION_32  : P0 phi/32 clock
 *              :               :   RIIC_CKS_DIVISION_64  : P0 phi/64 clock
 *              :               :   RIIC_CKS_DIVISION_128 : P0 phi/128 clock
 *              : uint32_t brl  : Lower byte value of the width of the SCL clock
 *              : uint32_t brh  : High byte value of the width of the SCL clock
 * Return Value : none
 ******************************************************************************/
void userdef_riic2_init (uint32_t cks, uint32_t brl, uint32_t brh)
{
#ifdef USE_RIIC_CH2
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 2) ---- */
    RZA_IO_RegWrite_8((volatile uint8_t *) &CPG.STBCR9, 0,
            CPG_STBCR9_MSTP95_SHIFT, CPG_STBCR9_MSTP95);

    dummy_buf = RZA_IO_RegRead_8((volatile uint8_t *) &CPG.STBCR9,
            CPG_STBCR9_MSTP95_SHIFT, CPG_STBCR9_MSTP95);

    /* ==== RIIC initial setting ==== */
    /* Clear the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnCR1.UINT8[0]), 0,
            RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnCR1.UINT8[0]), 1,
            RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnCR1.UINT8[0]), 1,
            RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IIC) Selection */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnMR1.UINT8[0]), (uint8_t) cks,
            RIICn_RIICnMR1_CKS_SHIFT, RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnBRL.UINT8[0]), (uint8_t) brl,
            RIICn_RIICnBRL_BRL_SHIFT, RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnBRH.UINT8[0]), (uint8_t) brh,
            RIICn_RIICnBRH_BRH_SHIFT, RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnFER.UINT8[0]), 1,
            RIICn_RIICnFER_NACKE_SHIFT, RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_RIE_SHIFT, RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_TIE_SHIFT, RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_TEIE_SHIFT, RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC2.RIICnCR1.UINT8[0]), 0,
            RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Initialise flags */
    riic2_receive_full_flg = DEVDRV_FLAG_OFF;
    riic2_transmit_empty_flg = DEVDRV_FLAG_OFF;
    riic2_transmit_end_flg = DEVDRV_FLAG_OFF;

    /* Register active interrupts */
    R_INTC_RegistIntFunc(INTC_ID_INTIICRI2, Sample_RIIC_Ri2_Interrupt);
    R_INTC_RegistIntFunc(INTC_ID_INTIICTI2, Sample_RIIC_Ti2_Interrupt);
    R_INTC_RegistIntFunc(INTC_ID_INTIICTEI2, Sample_RIIC_Tei2_Interrupt);

    /* Set active interrupt priorities */
    R_INTC_SetPriority(INTC_ID_INTIICRI2, 9);
    R_INTC_SetPriority(INTC_ID_INTIICTI2, 9);
    R_INTC_SetPriority(INTC_ID_INTIICTEI2, 9);

    /* Enable active interrupts */
    R_INTC_Enable(INTC_ID_INTIICRI2);
    R_INTC_Enable(INTC_ID_INTIICTI2);
    R_INTC_Enable(INTC_ID_INTIICTEI2);
#endif
}
/*******************************************************************************
 End of function userdef_riic2_init
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_init
 * Description  : Called to initialise the RIIC channel 3 peripheral
 *              : Uses the following interrupts:
 *              : Receive data full interrupt
 *              : Transmit data empty interrupt
 *              : Transmission complete interrupt
 * Arguments    : uint32_t cks  : I2c Clock Selection
 *              :               : (Settings in the CKS bit of RIICnMR1 register)
 *              :               :   RIIC_CKS_DIVISION_1   : P0 phi/1 clock
 *              :               :   RIIC_CKS_DIVISION_2   : P0 phi/2 clock
 *              :               :   RIIC_CKS_DIVISION_4   : P0 phi/4 clock
 *              :               :   RIIC_CKS_DIVISION_8   : P0 phi/8 clock
 *              :               :   RIIC_CKS_DIVISION_16  : P0 phi/16 clock
 *              :               :   RIIC_CKS_DIVISION_32  : P0 phi/32 clock
 *              :               :   RIIC_CKS_DIVISION_64  : P0 phi/64 clock
 *              :               :   RIIC_CKS_DIVISION_128 : P0 phi/128 clock
 *              : uint32_t brl  : Lower byte value of the width of the SCL clock
 *              : uint32_t brh  : High byte value of the width of the SCL clock
 * Return Value : none
 ******************************************************************************/
void userdef_riic3_init (uint32_t cks, uint32_t brl, uint32_t brh)
{
#ifdef USE_RIIC_CH3
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 1) ---- */
    RZA_IO_RegWrite_8((volatile uint8_t *) &CPG.STBCR9, 0,
            CPG_STBCR9_MSTP94_SHIFT, CPG_STBCR9_MSTP94);

    dummy_buf = RZA_IO_RegRead_8((volatile uint8_t *) &CPG.STBCR9,
            CPG_STBCR9_MSTP94_SHIFT, CPG_STBCR9_MSTP94);

    /* Clear the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]), 0,
            RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]), 1,
            RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]), 1,
            RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IIC) Selection */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnMR1.UINT8[0]), (uint8_t) cks,
            RIICn_RIICnMR1_CKS_SHIFT, RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnBRL.UINT8[0]), (uint8_t) brl,
            RIICn_RIICnBRL_BRL_SHIFT, RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnBRH.UINT8[0]), (uint8_t) brh,
            RIICn_RIICnBRH_BRH_SHIFT, RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnFER.UINT8[0]), 1,
            RIICn_RIICnFER_NACKE_SHIFT, RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_RIE_SHIFT, RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_TIE_SHIFT, RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]), 1,
            RIICn_RIICnIER_TEIE_SHIFT, RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]), 0,
            RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Register active interrupts */
    R_INTC_RegistIntFunc(INTC_ID_INTIICRI3, Sample_RIIC_Ri3_Interrupt);
    R_INTC_RegistIntFunc(INTC_ID_INTIICTI3, Sample_RIIC_Ti3_Interrupt);
    R_INTC_RegistIntFunc(INTC_ID_INTIICTEI3, Sample_RIIC_Tei3_Interrupt);

    /* Set active interrupt priorities */
    R_INTC_SetPriority(INTC_ID_INTIICRI3, 9);
    R_INTC_SetPriority(INTC_ID_INTIICTI3, 9);
    R_INTC_SetPriority(INTC_ID_INTIICTEI3, 9);

    /* Enable active interrupts */
    R_INTC_Enable(INTC_ID_INTIICRI3);
    R_INTC_Enable(INTC_ID_INTIICTI3);
    R_INTC_Enable(INTC_ID_INTIICTEI3);
#endif
}
/*******************************************************************************
 End of function userdef_riic3_init
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_init_rx_full
 * Description  : Called to process INTC_ID_INTIICRI for channel 0
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic0_init_rx_full (void)
{
    riic0_receive_full_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic0_init_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_init_rx_full
 * Description  : Called to process INTC_ID_INTIICRI for channel 1
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic1_init_rx_full (void)
{
    riic1_receive_full_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic1_init_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_init_rx_full
 * Description  : Called to process INTC_ID_INTIICRI for channel 2
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic2_init_rx_full (void)
{
    riic2_receive_full_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic2_init_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_init_rx_full
 * Description  : Called to process INTC_ID_INTIICRI for channel 3
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic3_init_rx_full (void)
{
    riic3_receive_full_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic3_init_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_init_tx_empty
 * Description  : Called to process INTC_ID_INTIICTI for channel 0
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic0_init_tx_empty (void)
{
    riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic0_init_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_init_tx_empty
 * Description  : Called to process INTC_ID_INTIICTI for channel 1
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic1_init_tx_empty (void)
{
    riic1_transmit_empty_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic1_init_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_init_tx_empty
 * Description  : Called to process INTC_ID_INTIICTI for channel 2
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic2_init_tx_empty (void)
{
    riic2_transmit_empty_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic2_init_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_init_tx_empty
 * Description  : Called to process INTC_ID_INTIICTI for channel 3
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic3_init_tx_empty (void)
{
    riic3_transmit_empty_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic3_init_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_init_tx_end
 * Description  : Called to process INTC_ID_INTIICTEI for channel 0
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic0_init_tx_end (void)
{
    riic0_transmit_end_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic0_init_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_init_tx_end
 * Description  : Called to process INTC_ID_INTIICTEI for channel 1
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic1_init_tx_end (void)
{
    riic1_transmit_end_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic1_init_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_init_tx_end
 * Description  : Called to process INTC_ID_INTIICTEI for channel 2
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic2_init_tx_end (void)
{
    riic2_transmit_end_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic2_init_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_init_tx_end
 * Description  : Called to process INTC_ID_INTIICTEI for channel 3
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic3_init_tx_end (void)
{
    riic3_transmit_end_flg = DEVDRV_FLAG_OFF;
}
/*******************************************************************************
 End of function userdef_riic3_init_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_set_rx_full
 * Description  : Set receive full flag for channel 0
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic0_set_rx_full (void)
{
    riic0_receive_full_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic0_set_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_set_rx_full
 * Description  : Set receive full flag for channel 1
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic1_set_rx_full (void)
{
    riic1_receive_full_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic1_set_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_set_rx_full
 * Description  : Set receive full flag for channel 2
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic2_set_rx_full (void)
{
    riic2_receive_full_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic2_set_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_set_rx_full
 * Description  : Set receive full flag for channel 3
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic3_set_rx_full (void)
{
    riic3_receive_full_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic3_set_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_set_tx_empty
 * Description  : Set transmit empty flag for channel 0
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic0_set_tx_empty (void)
{
    riic0_transmit_empty_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic0_set_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_set_tx_empty
 * Description  : Set transmit empty flag for channel 1
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic1_set_tx_empty (void)
{
    riic1_transmit_empty_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic1_set_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_set_tx_empty
 * Description  : Set transmit empty flag for channel 2
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic2_set_tx_empty (void)
{
    riic2_transmit_empty_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic2_set_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_set_tx_empty
 * Description  : Set transmit empty flag for channel 3
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic3_set_tx_empty (void)
{
    riic3_transmit_empty_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic3_set_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_set_tx_end
 * Description  : Set transmit end flag for channel 0
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic0_set_tx_end (void)
{
    riic0_transmit_end_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic0_set_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_set_tx_end
 * Description  : Set transmit end flag for channel 1
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic1_set_tx_end (void)
{
    riic1_transmit_end_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic1_set_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_set_tx_end
 * Description  : Set transmit end flag for channel 2
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic2_set_tx_end (void)
{
    riic2_transmit_end_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic2_set_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_set_tx_end
 * Description  : Set transmit end flag for channel 3
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void userdef_riic3_set_tx_end (void)
{
    riic3_transmit_end_flg = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function userdef_riic3_set_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_wait_rx_full
 * Description  : Wait in function till receive full flag set for channel 0
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic0_wait_rx_full (void)
{
    int32_t timeout = ICC0_MAX_TIMEOUT;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == riic0_receive_full_flg) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic0_wait_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_wait_rx_full
 * Description  : Wait in function till receive full flag set for channel 1
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic1_wait_rx_full (void)
{
    int32_t timeout = ICC1_MAX_TIMEOUT;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == riic1_receive_full_flg) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic1_wait_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_wait_rx_full
 * Description  : Wait in function till receive full flag set for channel 2
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic2_wait_rx_full (void)
{
    int32_t timeout = ICC2_MAX_TIMEOUT;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == riic2_receive_full_flg) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic2_wait_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_wait_rx_full
 * Description  : Wait in function till receive full flag set for channel 3
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic3_wait_rx_full (void)
{
    int32_t timeout = ICC3_MAX_TIMEOUT;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == riic3_receive_full_flg) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic3_wait_rx_full
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_wait_tx_empty
 * Description  : Wait in function till transmit empty flag set for channel 0
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic0_wait_tx_empty (void)
{
    int32_t timeout = ICC0_MAX_TIMEOUT;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == riic0_transmit_empty_flg) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic0_wait_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_wait_tx_empty
 * Description  : Wait in function till transmit empty flag set for channel 1
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic1_wait_tx_empty (void)
{
    int32_t timeout = ICC1_MAX_TIMEOUT;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == riic1_transmit_empty_flg) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic1_wait_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_wait_tx_empty
 * Description  : Wait in function till transmit empty flag set for channel 2
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic2_wait_tx_empty (void)
{
    int32_t timeout = ICC2_MAX_TIMEOUT;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == riic2_transmit_empty_flg) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic2_wait_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_wait_tx_empty
 * Description  : Wait in function till transmit empty flag set for channel 3
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic3_wait_tx_empty (void)
{
    int32_t timeout = ICC3_MAX_TIMEOUT;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == riic3_transmit_empty_flg) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic3_wait_tx_empty
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_wait_tx_end
 * Description  : Wait in function till transmit end flag set for channel 0
 * Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
 *              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic0_wait_tx_end (uint32_t mode)
{
    int32_t timeout = ICC0_MAX_TIMEOUT;

    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == riic0_transmit_end_flg) && (0 != timeout))
        {
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == riic0_receive_full_flg) && (0 != timeout))
        {
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic0_wait_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_wait_tx_end
 * Description  : Wait in function till transmit end flag set for channel 1
 * Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
 *              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic1_wait_tx_end (uint32_t mode)
{
    int32_t timeout = ICC1_MAX_TIMEOUT;

    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == riic1_transmit_end_flg) && (0 != timeout))
        {
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == riic1_receive_full_flg) && (0 != timeout))
        {
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic1_wait_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_wait_tx_end
 * Description  : Wait in function till transmit end flag set for channel 1
 * Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
 *              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic2_wait_tx_end (uint32_t mode)
{
    int32_t timeout = ICC2_MAX_TIMEOUT;

    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == riic2_transmit_end_flg) && (0 != timeout))
        {
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == riic2_receive_full_flg) && (0 != timeout))
        {
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic2_wait_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_wait_tx_end
 * Description  : Wait in function till transmit end flag set for channel 3
 * Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
 *              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic3_wait_tx_end (uint32_t mode)
{
    int32_t timeout = ICC3_MAX_TIMEOUT;

    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == riic3_transmit_end_flg) && (0 != timeout))
        {
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == riic3_receive_full_flg) && (0 != timeout))
        {
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic3_wait_tx_end
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_wait_bus_master
 * Description  : Wait for bus busy or bus free state for channel 0
 * Arguments    : uint32_t mode
 *              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
 *              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic0_wait_bus_master (uint32_t mode)
{
    int32_t timeout = ICC0_MAX_TIMEOUT;

    if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
        while ((1 == RZA_IO_RegRead_8(&(RIIC0.RIICnCR2.UINT8[0]),
                RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY)) && (0 != timeout))
        {
            /* Wait */
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        while ((0 == RZA_IO_RegRead_8(&(RIIC0.RIICnCR2.UINT8[0]),
                RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY)) && (0 != timeout))
        {
            /* Wait */
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic0_wait_bus_master
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_wait_bus_master
 * Description  : Wait for bus busy or bus free state for channel 1
 * Arguments    : uint32_t mode
 *              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
 *              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic1_wait_bus_master (uint32_t mode)
{
    int32_t timeout = ICC1_MAX_TIMEOUT;

    if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
        while ((1 == RZA_IO_RegRead_8(&(RIIC1.RIICnCR2.UINT8[0]),
                RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY)) && (0 != timeout))
        {
            /* Wait */
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        while ((0 == RZA_IO_RegRead_8(&(RIIC1.RIICnCR2.UINT8[0]),
                RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY)) && (0 != timeout))
        {
            /* Wait */
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic1_wait_bus_master
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_wait_bus_master
 * Description  : Wait for bus busy or bus free state for channel 2
 * Arguments    : uint32_t mode
 *              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
 *              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic2_wait_bus_master (uint32_t mode)
{
    int32_t timeout = ICC2_MAX_TIMEOUT;

    if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
        while ((1 == RZA_IO_RegRead_8(&(RIIC2.RIICnCR2.UINT8[0]),
                RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY)) && (0 != timeout))
        {
            /* Wait */
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        while ((0 == RZA_IO_RegRead_8(&(RIIC2.RIICnCR2.UINT8[0]),
                RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY)) && (0 != timeout))
        {
            /* Wait */
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic2_wait_bus_master
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_wait_bus_master
 * Description  : Wait for bus busy or bus free state for channel 3
 * Arguments    : uint32_t mode
 *              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
 *              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic3_wait_bus_master (uint32_t mode)
{
    int32_t timeout = ICC3_MAX_TIMEOUT;

    if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
        /* Wait till bus free */
        while ((1 == RZA_IO_RegRead_8(&(RIIC3.RIICnCR2.UINT8[0]),
                RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY)) && (0 != timeout))
        {
            /* Wait */
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        /* Wait till bus busy */
        while ((0 == RZA_IO_RegRead_8(&(RIIC3.RIICnCR2.UINT8[0]),
                RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY)) && (0 != timeout))
        {
            /* Wait */
            __asm__("nop");

            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic3_wait_bus_master
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic0_wait_stop
 * Description  : Wait for stop bit state for channel 0
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic0_wait_stop (void)
{
    int32_t timeout = ICC0_MAX_TIMEOUT;

    /* Wait */
    while ((0 == RZA_IO_RegRead_8(&(RIIC0.RIICnSR2.UINT8[0]),
            RIICn_RIICnSR2_STOP_SHIFT, RIICn_RIICnSR2_STOP)) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic0_wait_stop
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic1_wait_stop
 * Description  : Wait for stop bit state for channel 1
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic1_wait_stop (void)
{
    int32_t timeout = ICC1_MAX_TIMEOUT;

    /* Wait */
    while ((0 == RZA_IO_RegRead_8(&(RIIC1.RIICnSR2.UINT8[0]),
            RIICn_RIICnSR2_STOP_SHIFT, RIICn_RIICnSR2_STOP)) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic1_wait_stop
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic2_wait_stop
 * Description  : Wait for stop bit state for channel 2
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic2_wait_stop (void)
{
    int32_t timeout = ICC2_MAX_TIMEOUT;

    /* Wait */
    while ((0 == RZA_IO_RegRead_8(&(RIIC2.RIICnSR2.UINT8[0]),
            RIICn_RIICnSR2_STOP_SHIFT, RIICn_RIICnSR2_STOP)) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic2_wait_stop
 *******************************************************************************/

/******************************************************************************
 * Function Name: userdef_riic3_wait_stop
 * Description  : Wait for stop bit state for channel 3
 * Arguments    : none
 * Return Value : int32_t - DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 ******************************************************************************/
int32_t userdef_riic3_wait_stop (void)
{
    int32_t timeout = ICC3_MAX_TIMEOUT;

    /* Wait */
    while ((0 == RZA_IO_RegRead_8(&(RIIC3.RIICnSR2.UINT8[0]),
            RIICn_RIICnSR2_STOP_SHIFT, RIICn_RIICnSR2_STOP)) && (0 != timeout))
    {
        __asm__("nop");

        if (timeout > 0)
        {
            timeout--;
        }
    }

    return (0 == timeout) ? DEVDRV_ERROR_TIMEOUT : DEVDRV_SUCCESS;
}
/*******************************************************************************
 End of function userdef_riic3_wait_stop
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_ri0_interrupt
 * Description  : Channel 0's receive data full interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_ri0_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_RiInterrupt(DEVDRV_CH_0);
}
/*******************************************************************************
 End of function sample_riic_ri0_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_ti0_interrupt
 * Description  : Channel 0's transmit data empty interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_ti0_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TiInterrupt(DEVDRV_CH_0);
}
/*******************************************************************************
 End of function sample_riic_ti0_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_tei0_interrupt
 * Description  : Channel 0's transmission end interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_tei0_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TeiInterrupt(DEVDRV_CH_0);
}
/*******************************************************************************
 End of function sample_riic_tei0_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_ri1_interrupt
 * Description  : Channel 1's receive data full interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_ri1_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_RiInterrupt(DEVDRV_CH_1);
}
/*******************************************************************************
 End of function sample_riic_ri1_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_ti1_interrupt
 * Description  : Channel 1's transmit data empty interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_ti1_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TiInterrupt(DEVDRV_CH_1);
}
/*******************************************************************************
 End of function sample_riic_ti1_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_tei1_interrupt
 * Description  : Channel 1's transmission end interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_tei1_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TeiInterrupt(DEVDRV_CH_1);
}
/*******************************************************************************
 End of function sample_riic_tei1_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_ri2_interrupt
 * Description  : Channel 2's receive data full interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_ri2_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_RiInterrupt(DEVDRV_CH_2);
}
/*******************************************************************************
 End of function sample_riic_ri2_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_ti2_interrupt
 * Description  : Channel 2's transmit data empty interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_ti2_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TiInterrupt(DEVDRV_CH_2);
}
/*******************************************************************************
 End of function sample_riic_ti2_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_tei2_interrupt
 * Description  : Channel 2's transmission end interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_tei2_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TeiInterrupt(DEVDRV_CH_2);
}
/*******************************************************************************
 End of function sample_riic_tei2_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_ri3_interrupt
 * Description  : Channel 3's receive data full interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_ri3_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_RiInterrupt(DEVDRV_CH_3);
}
/*******************************************************************************
 End of function sample_riic_ri3_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_ti3_interrupt
 * Description  : Channel 3's transmit data empty interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_ti3_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TiInterrupt(DEVDRV_CH_3);
}
/*******************************************************************************
 End of function sample_riic_ti3_interrupt
 *******************************************************************************/

/******************************************************************************
 * Function Name: sample_riic_tei3_interrupt
 * Description  : Channel 3's transmission end interrupt handler
 * Arguments    : uint32_t int_sense : Interrupt detection
 *              :                    :   INTC_LEVEL_SENSITIVE : Level sense
 *              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
 * Return Value : none
 ******************************************************************************/
static void sample_riic_tei3_interrupt (uint32_t int_sense)
{
    UNUSED_PARAM(int_sense);

    R_RIIC_TeiInterrupt(DEVDRV_CH_3);
}
/*******************************************************************************
 End of function sample_riic_tei3_interrupt
 *******************************************************************************/

/* End of File */
