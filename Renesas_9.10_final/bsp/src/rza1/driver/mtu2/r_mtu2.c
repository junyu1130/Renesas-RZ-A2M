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
* File Name : r_mtu2.c
* Version : 1.0
* Description : Basic driver for the MTU2 peripheral. Handles the MTU2 timers
*               in Compare/Clear mode
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
* : 11.05.2016 1.00 First Release
******************************************************************************/



/******************************************************************************
Includes <System Includes> , “Project Includes”
******************************************************************************/
#include "iodefine.h"
#include "r_typedefs.h"
#include "dev_drv.h"
#include "r_mtu2.h"


/******************************************************************************
Private global variables and functions
******************************************************************************/
volatile uint16_t * const TCNT[5] = {
        &MTU2.TCNT_0, &MTU2.TCNT_1, &MTU2.TCNT_2, &MTU2.TCNT_3, &MTU2.TCNT_4
};
volatile uint16_t * const TGRA[5] = {
        &MTU2.TGRA_0, &MTU2.TGRA_1, &MTU2.TGRA_2, &MTU2.TGRA_3, &MTU2.TGRA_4
};
volatile uint8_t * const TMDR[5] = {
        &MTU2.TMDR_0, &MTU2.TMDR_1, &MTU2.TMDR_2, &MTU2.TMDR_3, &MTU2.TMDR_4
};
volatile uint8_t * const TIORH[5] = {
        &MTU2.TIORH_0, &MTU2.TIOR_1, &MTU2.TIOR_2, &MTU2.TIORH_3, &MTU2.TIORH_4
};
volatile uint8_t * const TSR[5] = {
        &MTU2.TSR_0, &MTU2.TSR_1, &MTU2.TSR_2, &MTU2.TSR_3, &MTU2.TSR_4
};
volatile uint8_t * const TCR[5] = {
        &MTU2.TCR_0, &MTU2.TCR_1, &MTU2.TCR_2, &MTU2.TCR_3, &MTU2.TCR_4
};

const uint8_t TCR_Prescale[5][6] = {
    {0x00,0x01,0x02,0x03,0x00,0x00},
    {0x00,0x01,0x02,0x03,0x06,0x00},
    {0x00,0x01,0x02,0x03,0x00,0x07},
    {0x00,0x01,0x02,0x03,0x04,0x05},
    {0x00,0x01,0x02,0x03,0x04,0x05}
};


/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name: R_MTU2_Initialize_CompareClear
* Description  : Sets the chosen timer to Compare/Clear mode with the wanted
*                prescaler.
* Arguments    : uint8_t timer: [0-4] index of the wanted timer
*                R_MTU2_prescale_t prescale: prescale value (from the Enum)
* Return Value : none
******************************************************************************/
void R_MTU2_Initialize_CompareClear(uint8_t timer, R_MTU2_prescale_t prescale) {
    if(timer>4)
        return;
    /* Enable access to write-protected MTU2 registers */
    MTU2.TRWER = 1u;

    /* Stop the chosen timer    */
    if(timer < 3)  {
        MTU2.TSTR &= ~((0x01)<<timer);
    } else {
        MTU2.TSTR &= ~((0x08)<<timer);
    }

    /* Clear the timer count */
    *TCNT[timer] = 0u;
    /* MTU2 channel normal operation */
    *TMDR[timer] = 0u;
    /* Timer output is not required */
    *TIORH[timer] = 0u;
    /* Clear the compare match status flag */
    *TSR[timer] &= 0xFE;
    /* TCNT cleared by TGRA compare match, rising edge count,
       clock source prescale   */
    *TCR[timer] = (0x20 | TCR_Prescale[timer][prescale]);

    /* Disable access to MTU2 write-protected registers */
    MTU2.TRWER = 0u;
}


/******************************************************************************
* Function Name: R_MTU2_StartTimer
* Description  : Starts the chosen timer
* Arguments    : uint8_t timer: [0-4] index of the wanted timer
* Return Value : none
******************************************************************************/
void R_MTU2_StartTimer(uint8_t timer) {
    /* Clear the compare match status flag     */
    *TSR[timer] &= 0xFE;
    /* Start timer                            */
    if(timer < 3)  {
        MTU2.TSTR |= ((0x01)<<timer);
    } else {
        MTU2.TSTR |= ((0x08)<<timer);
    }
}

/******************************************************************************
* Function Name: R_MTU2_StopTimer
* Description  : Stops the chosen timer
* Arguments    : uint8_t timer: [0-4] index of the wanted timer
* Return Value : none
******************************************************************************/
void R_MTU2_StopTimer(uint8_t timer) {
    /* Clear the compare match status flag     */
    *TSR[timer] &= 0xFE;
    /* Start timer                            */
    if(timer < 3)  {
        MTU2.TSTR &= ~((0x01)<<timer);
    } else {
        MTU2.TSTR &= ~((0x08)<<timer);
    }
}

/******************************************************************************
* Function Name: R_MTU2_LoadCompareValue
* Description  : Changes the compare value for the given timer
* Arguments    : uint8_t timer: [0-4] index of the wanted timer
*                uint16_t value: new compare value
* Return Value : none
******************************************************************************/
void R_MTU2_LoadCompareValue(uint8_t timer, uint16_t value) {
    *TGRA[timer] = value;
}

/******************************************************************************
* Function Name: R_MTU2_WaitCompareMatch
* Description  : Checks whether the timer has reached its compare value
* Arguments    : uint8_t timer: [0-4] index of the wanted timer
* Return Value : boolean: value reached or not.
******************************************************************************/
bool_t R_MTU2_WaitCompareMatch(uint8_t timer)  {
    return (0x01 != (*TSR[timer] & 0x01));
}
