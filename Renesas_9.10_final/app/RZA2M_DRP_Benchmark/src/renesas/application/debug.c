/******************************************************************************
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
 *******************************************************************************
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */

/******************************************************************************
 * File Name    : debug.c
 * Device(s)    : RZ/A2M
 * Tool-Chain   : GCC ARM Embedded 7.2.1.20170904
 * OS           : None
 * H/W Platform : RZ/A2M EVB kit
 * Description  : Debugging facilities
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.06.2019 1.00 First Release
 ******************************************************************************/

/**
 * License
 *
 * Copyright (C) 2010-2015 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.6.0
 **/

/* Dependencies */
#include "debug.h"

#include <r_bsp.h>

/* Peripheral clock 1 (66.67 MHz) */
#define PCLK1_HZ 				(400000000 / 6)
#define COM_QUEUE_LENGTH  		(60)

/* Handlers for the Rx and Tx interrupts respectively. */
static void prvRXI_Handler( uint32_t ulUnusedParameter );
static void prvTXI_Handler( uint32_t ulUnusedParameter );

/* The queue used to hold received characters. */
static OsQueue xRxedChars;
static OsQueue xCharsForTx;


/*****************************************************************************
 Function Name: debug_init
 Description:   UART initialisation
 Parameters:    baudrate - UART baud rate
 Return value:  None
 *****************************************************************************/
void debug_init(uint32_t baudrate)
{
   uint32_t status;
   bool_t RxQueueRet;
   bool_t TxQueueRet;
	/* Create the queues used to hold Rx/Tx characters.  Note the comments at
	the top of this file regarding the use of queues in this manner. */
   RxQueueRet = osCreateQueue( &xRxedChars, COM_QUEUE_LENGTH, ( unsigned portBASE_TYPE ) sizeof( char ) );
   TxQueueRet = osCreateQueue( &xCharsForTx, COM_QUEUE_LENGTH + 1, ( unsigned portBASE_TYPE ) sizeof( char ) );

	if( ( RxQueueRet != FALSE ) && ( TxQueueRet != FALSE ) )
	{

	    /* Register RXI and TXI handlers. */
	    R_INTC_RegistIntFunc( INTC_ID_SCIFA_RXI4, prvRXI_Handler );
	    //R_INTC_RegistIntFunc( INTC_ID_SCIFA_TXI4, prvTXI_Handler );

	    /* Set both interrupts such that they can interrupt the tick.  Also
	    set the Rx interrupt above the Tx interrupt in the hope that (for test
	    purposes) the Tx interrupt will interrupt the Rx interrupt. */
	    R_INTC_SetPriority( INTC_ID_SCIFA_RXI4, configMAX_API_CALL_INTERRUPT_PRIORITY );
	    //R_INTC_SetPriority( INTC_ID_SCIFA_TXI4, ( configMAX_API_CALL_INTERRUPT_PRIORITY + 1 ) );

	   /* Enable SCIFA4 peripheral clock */
	   CPG.STBCR4.BYTE &= (~CPG_STBCR4_MSTP43);

	   /* Disable SCIFA4 module */
	   SCIFA4.SCR.WORD = 0;

	   /* Reset transmit and receive FIFO */
	   SCIFA4.FCR.WORD |= (SCIFA_FCR_TFRST | SCIFA_FCR_RFRST);

	   /* Read serial status register */
	   status = SCIFA4.FSR.WORD;

	   /* Clear ER, BRK and DR flags */
	   SCIFA4.FSR.WORD = status & (~((SCIFA_FSR_ER | SCIFA_FSR_BRK) | SCIFA_FSR_DR));

	   /* Read line status register */
	   status = SCIFA4.LSR.WORD;

	   /* Clear ORER flag */
	   SCIFA4.LSR.WORD = status & (~SCIFA_LSR_ORER);

	   /* Select internal clock */
	   SCIFA4.SCR.WORD &= (~SCIFA_SCR_CKE);

	   /* Configure UART (8 bits, no parity, 1 stop bit) */
	   SCIFA4.SMR.WORD = 0;

	   /* Enable double-speed mode */
	   SCIFA4.SEMR.BYTE = SCIFA_SEMR_BGDM;

	   /* Set baud rate */
	   SCIFA4.BRR_MDDR.BRR.BYTE = (PCLK1_HZ / (16 * baudrate)) - 1;

	   /* Configure transmit and receive FIFO triggers */
	   SCIFA4.FCR.WORD = 0;

	   /* Clear TFRST and RFRST bits */
	   SCIFA4.FCR.WORD &= (~(SCIFA_FCR_TFRST | SCIFA_FCR_RFRST));

	   /* Enable transmission and reception */
	   SCIFA4.SCR.WORD = SCIFA_SCR_TE | SCIFA_SCR_RE | SCIFA_SCR_RIE;
	   R_INTC_Enable( INTC_ID_SCIFA_RXI4 );
	   //R_INTC_Enable( INTC_ID_SCIFA_TXI4 );
	}
}
/*****************************************************************************
 End of function debug_init
 *****************************************************************************/

/*****************************************************************************
 Function Name: debug_display_array
 Description:   Display the contents of an array
 Parameters:    stream - pointer to a FILE object that identifies an output stream
                prepend - string to prepend to the left of each line
                data - pointer to the data array
                length - number of bytes to display
 Return value:  None
 *****************************************************************************/
void debug_display_array(FILE *stream,
        const char_t *prepend, const void *data, size_t length)
{
    uint16_t i;

    for (i = 0; i < length; i++)
    {
        /* Beginning of a new line? */
        if ((i % 16) == 0)
        {
            fprintf(stream, "%s", prepend);
        }

        /* Display current data byte */
        fprintf(stream, "%02d ", *((uint8_t *) data + i));

        /* End of current line? */
        if (((i % 16) == 15) || ((length - 1) == i))
        {
            fprintf(stream, "\r\n");
        }
    }
}
/*****************************************************************************
 End of function debug_display_array
 *****************************************************************************/

/**
 * @brief Write character to stream
 * @param[in] c The character to be written
 * @param[in] Pointer to a FILE object that identifies an output stream
 * @return On success, the character written is returned. If a writing
 *   error occurs, EOF is returned
 **/

/*****************************************************************************
 Function Name: fputc
 Description:   Write character to stream
 Parameters:    c - the character to be written
                stream - pointer to a FILE object that identifies an output stream
 Return value:  int_t - the character written or EOF on failure
 *****************************************************************************/
int_t fputc(int_t c, FILE *stream)
{
   /* Standard error output or Standard output? */
   if ((stream == stderr) || (stream == stdout))
   {
      /* Wait for the transmit FIFO to be available for writing */
      while (!(SCIFA4.FSR.WORD & SCIFA_FSR_TDFE))
      {
          __asm__("nop");
      }

      /* Send character */
      SCIFA4.FTDR.BYTE = c;

      /* Clear TDFE flag */
      SCIFA4.FSR.WORD &= (~SCIFA_FSR_TDFE);

      /* On success, the character written is returned */
      return c;
   }

   /* Unknown output? */
   /* If a writing error occurs, EOF is returned */
   return EOF;
}
/*****************************************************************************
 End of function fputc
 *****************************************************************************/

uint32_t debug_get_char(signed char *pcRxedChar, TickType_t xBlockTime )
{

	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( osReceiveQueue( &xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}

static void prvRXI_Handler( uint32_t ulUnusedParameter )
{
	unsigned char ucRxedByte;
	long lHigherPriorityTaskWoken = pdFALSE;

	/* The parameter is not used.  It is only present because Renesas drivers
	are used to install the interrupt handlers, and the drivers expect the
	parameter to be present. */
	( void ) ulUnusedParameter;

	/* Note the comments at the top of this file regarding the use of queues in
	this manner. */
	while( ( SCIFA4.FDR.WORD & 0x1F ) != 0 )
	{
		ucRxedByte = SCIFA4.FRDR.BYTE;
		osSendQueueISR( &xRxedChars, &ucRxedByte, &lHigherPriorityTaskWoken );
	}

	SCIFA4.FSR.WORD &= ~SCIFA_FSR_RDF;

	/* If sending to the queue has caused a task to unblock, and the unblocked
	task has a priority equal to or higher than the currently running task (the
	task this ISR interrupted), then lHigherPriorityTaskWoken will have
	automatically been set to pdTRUE within the queue send function.
	portYIELD_FROM_ISR() will then ensure that this ISR returns	directly to the
	higher priority unblocked task. */
	osSwitchTaskISR( lHigherPriorityTaskWoken );
}

static void prvTXI_Handler( uint32_t ulUnusedParameter )
{

}

