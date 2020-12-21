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
 * File Name     : riic_userdef.h
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream it! v2 board
 * Description   : RIIC user defined functions header
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 21.10.2014 1.00
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RIIC_USERDEF_H
    #define RIIC_USERDEF_H

    #include "r_typedefs.h"     /* Standard I/O definitions header */

/******************************************************************************
 Functions Prototypes
 ******************************************************************************/

/* ==== User-defined functions ==== */
void userdef_riic0_init (uint32_t cks, uint32_t brl, uint32_t brh);
void userdef_riic1_init (uint32_t cks, uint32_t brl, uint32_t brh);
void userdef_riic2_init (uint32_t cks, uint32_t brl, uint32_t brh);
void userdef_riic3_init (uint32_t cks, uint32_t brl, uint32_t brh);
void userdef_riic0_init_rx_full (void);
void userdef_riic1_init_rx_full (void);
void userdef_riic2_init_rx_full (void);
void userdef_riic3_init_rx_full (void);
void userdef_riic0_init_tx_empty (void);
void userdef_riic1_init_tx_empty (void);
void userdef_riic2_init_tx_empty (void);
void userdef_riic3_init_tx_empty (void);
void userdef_riic0_init_tx_end (void);
void userdef_riic1_init_tx_end (void);
void userdef_riic2_init_tx_end (void);
void userdef_riic3_init_tx_end (void);
void userdef_riic0_set_rx_full (void);
void userdef_riic1_set_rx_full (void);
void userdef_riic2_set_rx_full (void);
void userdef_riic3_set_rx_full (void);
void userdef_riic0_set_tx_empty (void);
void userdef_riic1_set_tx_empty (void);
void userdef_riic2_set_tx_empty (void);
void userdef_riic3_set_tx_empty (void);
void userdef_riic0_set_tx_end (void);
void userdef_riic1_set_tx_end (void);
void userdef_riic2_set_tx_end (void);
void userdef_riic3_set_tx_end (void);
int32_t userdef_riic0_wait_rx_full (void);
int32_t userdef_riic1_wait_rx_full (void);
int32_t userdef_riic2_wait_rx_full (void);
int32_t userdef_riic3_wait_rx_full (void);
int32_t userdef_riic0_wait_tx_empty (void);
int32_t userdef_riic1_wait_tx_empty (void);
int32_t userdef_riic2_wait_tx_empty (void);
int32_t userdef_riic3_wait_tx_empty (void);
int32_t userdef_riic0_wait_tx_end (uint32_t mode);
int32_t userdef_riic1_wait_tx_end (uint32_t mode);
int32_t userdef_riic2_wait_tx_end (uint32_t mode);
int32_t userdef_riic3_wait_tx_end (uint32_t mode);
int32_t userdef_riic0_wait_bus_master (uint32_t mode);
int32_t userdef_riic1_wait_bus_master (uint32_t mode);
int32_t userdef_riic2_wait_bus_master (uint32_t mode);
int32_t userdef_riic3_wait_bus_master (uint32_t mode);
int32_t userdef_riic0_wait_stop (void);
int32_t userdef_riic1_wait_stop (void);
int32_t userdef_riic2_wait_stop (void);
int32_t userdef_riic3_wait_stop (void);

//extern volatile struct st_riic * priic_get_reg_addr (uint32_t channel);

#endif  /* RIIC_USERDEF_H */

/* End of File */
