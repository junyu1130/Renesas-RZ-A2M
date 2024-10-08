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
* Copyright (C) 2013-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name : lin_iodefine.h
* $Rev: 29 $
* $Date:: 2016-12-20 17:40:26 +0900#$
* Description : Definition of I/O Register for RZ/A1L (V2.00l)
******************************************************************************/
#ifndef LIN_IODEFINE_H
#define LIN_IODEFINE_H
/* ->QAC 0639 : Over 127 members (C90) */
/* ->QAC 0857 : Over 1024 #define (C90) */
/* ->MISRA 18.4 : Pack unpack union */ /* ->SEC M1.6.2 */
/* ->SEC M1.10.1 : Not magic number */

#define LIN0    (*(struct st_lin     *)0xFCFE9000uL) /* LIN0 */


/* Start of channel array defines of LIN */

/* Channel array defines of LIN */
/*(Sample) value = LIN[ channel ]->RLN3nLWBR; */
#define LIN_COUNT  (1)
#define LIN_ADDRESS_LIST \
{   /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */ \
    &LIN0 \
}   /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */ /* { } is for MISRA 19.4 */

/* End of channel array defines of LIN */


#define LIN0RLN30LWBR (LIN0.RLN3nLWBR)
#define LIN0RLN30LBRP0 (LIN0.RLN3nLBRP0)
#define LIN0RLN30LBRP1 (LIN0.RLN3nLBRP1)
#define LIN0RLN30LSTC (LIN0.RLN3nLSTC)
#define LIN0RLN30LMD (LIN0.RLN3nLMD)
#define LIN0RLN30LBFC (LIN0.RLN3nLBFC)
#define LIN0RLN30LSC (LIN0.RLN3nLSC)
#define LIN0RLN30LWUP (LIN0.RLN3nLWUP)
#define LIN0RLN30LIE (LIN0.RLN3nLIE)
#define LIN0RLN30LEDE (LIN0.RLN3nLEDE)
#define LIN0RLN30LCUC (LIN0.RLN3nLCUC)
#define LIN0RLN30LTRC (LIN0.RLN3nLTRC)
#define LIN0RLN30LMST (LIN0.RLN3nLMST)
#define LIN0RLN30LST (LIN0.RLN3nLST)
#define LIN0RLN30LEST (LIN0.RLN3nLEST)
#define LIN0RLN30LDFC (LIN0.RLN3nLDFC)
#define LIN0RLN30LIDB (LIN0.RLN3nLIDB)
#define LIN0RLN30LCBR (LIN0.RLN3nLCBR)
#define LIN0RLN30LDBR1 (LIN0.RLN3nLDBR1)
#define LIN0RLN30LDBR2 (LIN0.RLN3nLDBR2)
#define LIN0RLN30LDBR3 (LIN0.RLN3nLDBR3)
#define LIN0RLN30LDBR4 (LIN0.RLN3nLDBR4)
#define LIN0RLN30LDBR5 (LIN0.RLN3nLDBR5)
#define LIN0RLN30LDBR6 (LIN0.RLN3nLDBR6)
#define LIN0RLN30LDBR7 (LIN0.RLN3nLDBR7)
#define LIN0RLN30LDBR8 (LIN0.RLN3nLDBR8)

#define LIN_LDBn_COUNT (8)


typedef struct st_lin
{
                                                           /* LIN              */
    volatile uint8_t   dummy1[1];                              /*                  */
    volatile uint8_t   RLN3nLWBR;                              /*  RLN3nLWBR       */
    volatile uint8_t   RLN3nLBRP0;                             /*  RLN3nLBRP0      */
    volatile uint8_t   RLN3nLBRP1;                             /*  RLN3nLBRP1      */
    volatile uint8_t   RLN3nLSTC;                              /*  RLN3nLSTC       */
    volatile uint8_t   dummy2[3];                              /*                  */
    volatile uint8_t   RLN3nLMD;                               /*  RLN3nLMD        */
    volatile uint8_t   RLN3nLBFC;                              /*  RLN3nLBFC       */
    volatile uint8_t   RLN3nLSC;                               /*  RLN3nLSC        */
    volatile uint8_t   RLN3nLWUP;                              /*  RLN3nLWUP       */
    volatile uint8_t   RLN3nLIE;                               /*  RLN3nLIE        */
    volatile uint8_t   RLN3nLEDE;                              /*  RLN3nLEDE       */
    volatile uint8_t   RLN3nLCUC;                              /*  RLN3nLCUC       */
    volatile uint8_t   dummy3[1];                              /*                  */
    volatile uint8_t   RLN3nLTRC;                              /*  RLN3nLTRC       */
    volatile uint8_t   RLN3nLMST;                              /*  RLN3nLMST       */
    volatile uint8_t   RLN3nLST;                               /*  RLN3nLST        */
    volatile uint8_t   RLN3nLEST;                              /*  RLN3nLEST       */
    volatile uint8_t   RLN3nLDFC;                              /*  RLN3nLDFC       */
    volatile uint8_t   RLN3nLIDB;                              /*  RLN3nLIDB       */
    volatile uint8_t   RLN3nLCBR;                              /*  RLN3nLCBR       */
    volatile uint8_t   dummy4[1];                              /*                  */

/* #define LIN_LDBn_COUNT (8) */
    volatile uint8_t   RLN3nLDBR1;                             /*  RLN3nLDBR1      */
    volatile uint8_t   RLN3nLDBR2;                             /*  RLN3nLDBR2      */
    volatile uint8_t   RLN3nLDBR3;                             /*  RLN3nLDBR3      */
    volatile uint8_t   RLN3nLDBR4;                             /*  RLN3nLDBR4      */
    volatile uint8_t   RLN3nLDBR5;                             /*  RLN3nLDBR5      */
    volatile uint8_t   RLN3nLDBR6;                             /*  RLN3nLDBR6      */
    volatile uint8_t   RLN3nLDBR7;                             /*  RLN3nLDBR7      */
    volatile uint8_t   RLN3nLDBR8;                             /*  RLN3nLDBR8      */
} r_io_lin_t;


/* Channel array defines of LIN (2)*/
#ifdef  DECLARE_LIN_CHANNELS
volatile struct st_lin*  LIN[ LIN_COUNT ] =
    /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */
    LIN_ADDRESS_LIST;
    /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */
#endif  /* DECLARE_LIN_CHANNELS */
/* End of channel array defines of LIN (2)*/


/* <-SEC M1.10.1 */
/* <-MISRA 18.4 */ /* <-SEC M1.6.2 */
/* <-QAC 0857 */
/* <-QAC 0639 */
#endif
