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
* File Name     : r_riic_header.h
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONEv16.01-EABI
* H/W Platform  : Stream It! v2 Board
* Description   : RIIC driver header
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "r_typedefs.h"        /* Standard I/O definitions header */

/* Multiple inclusion prevention macro */
#ifndef R_RIIC_HEADER_H
#define R_RIIC_HEADER_H

/* define which channels are in use */
/* #define USE_RIIC_CH0 */
#define USE_RIIC_CH1
/* #define USE_RIIC_CH2 */
/* #define USE_RIIC_CH3 */

#define RIIC_CH_TOTAL           (4)

#define RIIC_CH_0               (0)
#define RIIC_CH_1               (1)
#define RIIC_CH_2               (2)
#define RIIC_CH_3               (3)

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* ==== CKS frequency setting (Clock(IIC clock) source selection) ==== */
#define RIIC_CKS_DIVISION_1     (0) /* P0 clock divided by 1   */
#define RIIC_CKS_DIVISION_2     (1) /* P0 clock divided by 2   */
#define RIIC_CKS_DIVISION_4     (2) /* P0 clock divided by 4   */
#define RIIC_CKS_DIVISION_8     (3) /* P0 clock divided by 8   */
#define RIIC_CKS_DIVISION_16    (4) /* P0 clock divided by 16  */
#define RIIC_CKS_DIVISION_32    (5) /* P0 clock divided by 32  */
#define RIIC_CKS_DIVISION_64    (6) /* P0 clock divided by 64  */
#define RIIC_CKS_DIVISION_128   (7) /* P0 clock divided by 128 */

/* ==== Transmission mode ==== */
#define RIIC_TX_MODE_START      (0) /* Start condition issuance request   */
#define RIIC_TX_MODE_RESTART    (1) /* Restart condition issuance request */
#define RIIC_TX_MODE_STOP       (2) /* Stop condition issuance request    */

/* ==== Wait operation after transmitting slave address ==== */
#define RIIC_TEND_WAIT_TRANSMIT (0) /* Wait for the continuation of transmission mode to have been transmitted */
#define RIIC_TEND_WAIT_RECEIVE  (1) /* Wait for the transition of reception mode to have been transmitted      */

/* ==== Bus mastership wait mode ==== */
#define RIIC_BUS_MASTERSHIP_WAIT_FREE   (0) /* Wait for bus free */
#define RIIC_BUS_MASTERSHIP_WAIT_BUSY   (1) /* Wait for bus busy */

/* ==== Return values ==== */
#define DEVDRV_ERROR_RIIC_NACK  (-2)    /* NACK reception (No acknowledge from the slave device) */

/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t R_RIIC_Init (uint32_t channel, uint32_t cks, uint32_t brl, uint32_t brh);
int32_t R_RIIC_SingleRead (uint32_t channel, uint8_t * pdata);
int32_t R_RIIC_SingleWrite (uint32_t channel, uint8_t data, uint32_t mode);
int32_t R_RIIC_WriteCond (uint32_t channel, uint32_t mode);
int32_t R_RIIC_Write (uint32_t channel, const uint8_t * pbuffer, uint32_t byte);
int32_t R_RIIC_Read (uint32_t channel, uint8_t * pbuffer, uint32_t byte);
int32_t R_RIIC_ReadDummy (uint32_t channel);
int32_t R_RIIC_DetectStop (uint32_t channel);
int32_t R_RIIC_ClearNack (uint32_t channel);
int32_t R_RIIC_RiInterrupt (uint32_t channel);
int32_t R_RIIC_TiInterrupt (uint32_t channel);
int32_t R_RIIC_TeiInterrupt (uint32_t channel);

volatile struct st_riic * priic_get_reg_addr (uint32_t channel);

#endif  /* R_RIIC_HEADER_H */

/* End of File */
