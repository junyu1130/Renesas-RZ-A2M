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
 * @file          r_rza2_scifa_lld_baud.c
 * Device(s)      RZ/A2
 * Tool-Chain     GCC ARM Embedded 5.4 2016q3
 * H/W Platform   RZ/A2
 * @brief         SCIFA low level driver baud rate setting
 *******************************************************************************/
/*******************************************************************************
 * History       DD.MM.YYYY Version Description
 * @date         15.05.2018 1.00
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
#include "r_rza2_scifa_lld.h"          /* Low level Driver Header */
#include "r_rza2_scifa_lld_cfg.h"      /* Low level Driver Configuration Header */

/*******************************************************************************
 Private global variables and functions
 *******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define R_PRV_NUM_DIVISORS_ASYNC    (9)
#define R_PRV_NUM_DIVISORS_SYNC     (4)

/*******************************************************************************
 Typedefs
 *******************************************************************************/

/**
 * @typedef st_baud_divisorb_t Baud Rate divisor struct.
 */
typedef struct
{
    int16_t     divisor;    /*!< clock divisor */
    uint8_t     abcs;       /*!< abcs value to get divisor */
    uint8_t     bgdm;       /*!< bdgm value to get divisor */
    uint8_t     cks;        /*!< cks  value to get divisor (cks = n) */
} st_baud_divisorb_t;

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static volatile struct st_scifa *SCIFA[SCIFA_CFG_LLD_NUM_CHANNELS] =
{
    /* SCIFA Channels */
    &SCIFA0, &SCIFA1, &SCIFA2, &SCIFA3, &SCIFA4
};

static uint32_t gs_divisor_index;
static uint32_t gs_num_divisors;
static uint32_t gs_ratio;
static uint32_t gs_brr;
static uint32_t gs_divisor;
static uint32_t gs_int_mddr;
static st_baud_divisorb_t const *gsp_baud_info;
static float gs_float_mddr;
static float gs_error;
static float gs_abs_error;
static float gs_actual_baud_rate;

/* Baud divisor info */

/**@section
   BRR = (PCLK / (divisor x baud)) - 1
   when ABCS = 1, divisor = 32 x 2^(2n - 1)
   when ABCS = 0, divisor = 64 x 2^(2n - 1) */

/**@section
   when ABCS = 0 & BGDM = 0, divisor = 64 x 2^(2n - 1)
   when ABCS = 1 & BGDM = 0 OR ABCS = 0 & BGDM = 1, divisor = 32 x 2^(2n - 1)
   when ABCS = 1 & BGDM = 1, divisor = 16 x 2^(2n - 1) */

const static st_baud_divisorb_t gs_scifa_async_baud[R_PRV_NUM_DIVISORS_ASYNC] =
{
    {8,    1, 1, 0},    // divisor result, ABCS, BGDM, n
    {16,   0, 1, 0},
    {32,   0, 0, 0},
    {64,   0, 1, 1},
    {128,  0, 0, 1},
    {256,  0, 1, 2},
    {512,  0, 0, 2},
    {1024, 0, 1, 3},
    {2048, 0, 0, 3}
};

const static st_baud_divisorb_t gs_scifa_sync_baud[R_PRV_NUM_DIVISORS_SYNC] =
{
    {4,    0, 0, 0},    // divisor result, ABCS, BGDM, n
    {16,   0, 0, 1},
    {64,   0, 0, 2},
    {256,  0, 0, 3},
};

/* Prototypes */

static void write_baud_rate(int_t channel);
static void write_mddr(int_t channel);
static void calc_brr_only_actual_baud_rate(uint32_t desired_baud_rate, uint32_t clock_freq);
static void calculate_baud_rate_with_mddr(uint32_t clock_freq);
static void find_divisor_value(int_t channel, uint32_t desired_baud_rate, uint32_t clock_freq, scifa_sync_mode_t mode);

/******************************************************************************
 Private function definitions
 ******************************************************************************/

/**
 * @brief write_baud_rate writes the Baud rate 
 * Baud rate written to the BRR, ABCS, BGDM, 
 * and CKS registers / fields
 *
 * @param[in] channel  The SCIFA channel to update
 *
 * @return None
*/
static void write_baud_rate(int_t channel)
{
    /* set BRR */
    RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->BRR_MDDR.BRR.BYTE, (uint8_t) gs_brr, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* set SEMR.ABCS0 bit */
    RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->SEMR.BYTE, gsp_baud_info[gs_divisor_index].abcs, SCIFA_SEMR_ABCS0_SHIFT, SCIFA_SEMR_ABCS0);

    /* set SEMR.BGDM bit */
    RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->SEMR.BYTE, gsp_baud_info[gs_divisor_index].bgdm, SCIFA_SEMR_BGDM_SHIFT, SCIFA_SEMR_BGDM);

    /* set SMR.CKS[1:0] bits */
    RZA_IO_RegWrite_16((volatile uint16_t *) &SCIFA[channel]->SMR.WORD, gsp_baud_info[gs_divisor_index].cks, SCIFA_SMR_CKS_SHIFT, SCIFA_SMR_CKS);
}
/******************************************************************************
 * End of Function write_baud_rate
 ******************************************************************************/

 /**
  * @brief write_mddr Writes to the MDDR register and enable MDDR
  * 
  * @param[in] channel The SCIFA channel to update.
  *
  * @return None
  */
static void write_mddr(int_t channel)
{
    /* enable MDDR access */
    RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->SEMR.BYTE, (uint16_t) 1, SCIFA_SEMR_MDDRS_SHIFT, SCIFA_SEMR_MDDRS);

    /* write MDDR */
    RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->BRR_MDDR.MDDR.BYTE, (uint8_t) gs_int_mddr, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* enable MDDR */
    RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->SEMR.BYTE, (uint16_t) 1, SCIFA_SEMR_BRME_SHIFT, SCIFA_SEMR_BRME);
}
/******************************************************************************
 * End of Function write_mddr
 ******************************************************************************/

/**
 * @brief Calculate the actual Baud rate and error when only the BRR is being used
 *
 * @param[in] desired_baud_rate  The desired Baud rate
 *
 * @param[in] clock_freq  The peripheral clock frequency
 *
 * @return None
*/
static void calc_brr_only_actual_baud_rate(uint32_t desired_baud_rate, uint32_t clock_freq)
{
    /*                    Clock in Hz
       Baud rate =   _____________________
                     prescalar x (BRR + 1)    */

    gs_actual_baud_rate = clock_freq;
    gs_actual_baud_rate /= (gs_brr + 1);
    gs_actual_baud_rate /= gs_divisor;

    /* calculate the bit rate error rate */
    gs_error = (((float) clock_freq / (desired_baud_rate * gs_divisor * (gs_brr + 1))) - 1) * 100;
    gs_abs_error = (gs_error < 0) ? (-gs_error) : gs_error;
}
/******************************************************************************
 * End of Function calc_brr_only_actual_baud_rate
 ******************************************************************************/

/**
 * @brief Calculate the actual Baud rate and error when the MDDR is being used
 *
 * @param[in] clock_freq  The peripheral clock frequency
 *
 * @return None
*/
static void calculate_baud_rate_with_mddr(uint32_t clock_freq)
{
    /*                           Clock in Hz
       Baud rate =   ____________________________________
                     prescalar x (256 / MDDR) x (BRR + 1)    */

    gs_actual_baud_rate = clock_freq;
    gs_actual_baud_rate /= (gs_brr + 1);
    gs_actual_baud_rate /= gs_divisor;
    gs_actual_baud_rate /= (256.0 / gs_int_mddr);
}
/******************************************************************************
 * End of Function calculate_baud_rate_with_mddr
 ******************************************************************************/

/**
 * @brief Determine the value of the prescalars needed to achieve the desired
 *        Baud rate
 *
 * @param[in] desired_baud_rate  The desired Baud rate
 *
 * @param[in] clock_freq  The peripheral clock frequency
 *
 * @param[in] mode  asynchronous or synchronous
 *
 * @return None
 */
static void find_divisor_value(int_t channel, uint32_t desired_baud_rate, uint32_t clock_freq, scifa_sync_mode_t mode)
{
    /* select divisor table depending on the selected mode */
    if (SCIFA_MODE_ASYNC == mode)
    {
        gsp_baud_info = gs_scifa_async_baud;
        gs_num_divisors = R_PRV_NUM_DIVISORS_ASYNC;
    }
    else
    {
        gsp_baud_info = gs_scifa_sync_baud;
        gs_num_divisors = R_PRV_NUM_DIVISORS_SYNC;
    }

    /* find the divisor; table has associated ABCS, BGDM and CKS values */
    /* BRR must be 255 or less */
    /* the "- 1" is ignored in some steps for approximations */
    /* BRR = (PCLK / (divisor * desired_baud)) - 1 */
    /* BRR = (ratio / divisor) - 1 */

    gs_ratio = clock_freq / desired_baud_rate;
    gs_divisor_index = 0;

    if (0 == channel)
    {
		/* cast to uint32_t for comparison with ratio */
		while ((gs_divisor_index < gs_num_divisors) && (gs_ratio >= (uint32_t) (gsp_baud_info[gs_divisor_index].divisor * 256)) &&
			/* The hardware manual states that for channel 0, P1f/16 input is not provided */
			/* so the setting CKS [1:0] 1 0 cannot be used */
			/* this restriction may be lifted in future releases */
			(1 == gsp_baud_info[gs_divisor_index].cks))
		{
			gs_divisor_index++;
		}
    }
    else
    {
		/* cast to uint32_t for comparison with ratio */
		while ((gs_divisor_index < gs_num_divisors) && (gs_ratio >= (uint32_t) (gsp_baud_info[gs_divisor_index].divisor * 256)))
		{
			gs_divisor_index++;
		}
    }
}
/******************************************************************************
 * End of Function find_divisor_value
 ******************************************************************************/

/******************************************************************************
 Public function definitions
 ******************************************************************************/

/**
 * @brief Sets the device baud rate within 1% of value desired.
 *
 * This function determines the best possible settings for the
 * baud rate registers for the specified peripheral clock speed
 * and baud rate. Note that this does not guarantee a low bit
 * error rate, just the best possible one. The bit rate error is
 * returned in .1% increments. If the hardware cannot support
 * the specified combination, DEV_ERROR is returned.
 *
 * NOTE: The transmitter and receiver (TE and RE bits in SCR) must be disabled
 *       prior to calling this function.
 *
 *       The application must pause for 1 bit time after the BRR register
 *       is loaded before transmitting/receiving to allow time for the clock
 *       to settle.
 * @param[in]  channel: Channel number of SCIFA(0-4)
 * @param[in]  desired_baud_rate: Baud rate; 19200, 57600, 115200, etc.
 * @param[in]  clock_freq: Peripheral clock speed; e.g. 24000000 for 24MHz
 * @param[in]  mode: asynchronous or synchronous
 * @param[out] p_achieved_baud_rate - Pointer to achieved Baud rate
 *
 * @retval  0 DRV_SUCCESS - If a Baud rate within 1% of the desired value was
 *                          achieved.
 * @retval -1 DRV_ERROR 
 */
int_t R_SCIFA_SetBaud (int_t channel, uint32_t desired_baud_rate, uint32_t clock_freq, scifa_sync_mode_t mode, uint32_t *p_achieved_baud_rate)
{
    /* check arguments */
    if ((channel >= R_PRV_SCIF_LAST_CHANNEL) || (channel < 0))
    {
        return (DRV_ERROR);
    }

    find_divisor_value(channel, desired_baud_rate, clock_freq, mode);

    /* return if BRR will be > 255 or < 0 */
    if (gs_divisor_index == gs_num_divisors)
    {
        return (DRV_ERROR);                /* impossible desired_baud_rate requested; return error */
    }

    /* cast to uint32_t for divisor */
    gs_divisor = (uint32_t) gsp_baud_info[gs_divisor_index].divisor;
    gs_brr = gs_ratio / gs_divisor;

    if (0 == gs_brr)
    {
        return (DRV_ERROR);                /* illegal value; return error */
    }

    /* enable BRR access */
    RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->SEMR.BYTE, (uint16_t) 0, SCIFA_SEMR_MDDRS_SHIFT, SCIFA_SEMR_MDDRS);
    gs_brr = gs_ratio / (gs_divisor / 2);                                // divide by half the divisor

    /* if odd, "round up" by ignoring -1; divide by 2 again for rest of divisor */
    gs_brr = ((gs_brr & 0x01) ? (gs_brr / 2) : ((gs_brr / 2) - 1));

    write_baud_rate(channel);            /* write BRR, ABCS0, BGDM and CKS */

    calc_brr_only_actual_baud_rate(desired_baud_rate, clock_freq);    /* calculate achieved Baud rate and error */

    /* if error is less than 1.0 then use BRR without MDDR */
    if ((gs_abs_error <= 1.0) || (1 == gs_brr))
    {
         /* disable MDDR access */
        RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->SEMR.BYTE, (uint16_t) 0, SCIFA_SEMR_BRME_SHIFT, SCIFA_SEMR_BRME);

        /* set the achieved Baud rate */
        *p_achieved_baud_rate = (uint32_t) gs_actual_baud_rate;
        return (DRV_SUCCESS);
    }

    /* The MDDR can only reduce the Baud rate, so if the current BRR setting results in a Baud rate that is lower than the desired rate,
       then we need to reduce the BRR by one in order to increase the Baud rate so that it is greater than the desired rate. */
    if (gs_actual_baud_rate < desired_baud_rate)
    {
        /* if BRR is already 1, then it can't go any lower */
        if (1 == gs_brr)
        {
            /* disable MDDR */
            RZA_IO_RegWrite_8((volatile uint8_t *) &SCIFA[channel]->SEMR.BYTE, (uint16_t) 0, SCIFA_SEMR_BRME_SHIFT, SCIFA_SEMR_BRME);
            return (DRV_ERROR);
        }

        /* reduce the BRR value by one */
        gs_brr--;
        SCIFA[channel]->BRR_MDDR.BRR.BYTE = gs_brr;
    }

    /* calculate MDDR value */
    gs_float_mddr = ((float) desired_baud_rate * gs_divisor * 256 * (gs_brr + 1)) / clock_freq;
    gs_float_mddr *= 2;

    /* cast float to uint32_t */
    gs_int_mddr = (uint32_t) gs_float_mddr;
    gs_int_mddr = (gs_int_mddr & 0x01) ? ((gs_int_mddr / 2) + 1) : (gs_int_mddr / 2);

    calculate_baud_rate_with_mddr(clock_freq);

    write_mddr(channel);

    /* set the achieved Baud rate */
    *p_achieved_baud_rate = (uint32_t) gs_actual_baud_rate;

    return (DRV_SUCCESS);
}
/******************************************************************************
 * End of Function R_SCIFA_SetBaud
 ******************************************************************************/

/* End of File */
