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
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : CLI_commands.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2
 * Description  : Create a Console task to process the command from user input
 *
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 14.03.2018 1.00 First Release
 ******************************************************************************/
#include <r_bsp.h>
#include <string.h>
#include <stdio.h>
#include <CLI_commands.h>

/*
 * Implements the run-time-stats command.
 */
static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static portBASE_TYPE prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

#define Version_Major  (1)
#define Version_Minor  (0)
static BaseType_t prvVersionCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

static const CLI_Command_Definition_t xCommandList[] =
{
#if 0
		{
			"query-heap", /* The command string to type. */
			"\r\nquery-heap:\r\n Displays the free heap space, and minimum ever free heap space.\r\n",
			prvQueryHeapCommand, /* The function to run. */
			0 /* No parameters are expected. */
		},
		/* Structure that defines the "task-stats" command line command.  This generates
		a table that gives information on each task in the system. */
		{
			"task-stats", /* The command string to type. */
			"\r\ntask-stats:\r\n Displays a table showing the state of each FreeRTOS task\r\n",
			prvTaskStatsCommand, /* The function to run. */
			0 /* No parameters are expected. */
		},
#endif
		{
			"A2000000", /* The command string to type. */
			"\r\nVersion inquiry.\r\n",
			prvVersionCommand, /* The function to run. */
			0 /* No parameters are expected. */
		}
};

static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	sprintf( pcWriteBuffer, "Current free heap %d bytes, minimum ever free heap %d bytes\r\n", ( int ) xPortGetFreeHeapSize(), ( int ) xPortGetMinimumEverFreeHeapSize() );

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

static portBASE_TYPE prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *const pcHeader = "Task                   State  Priority  Stack	#\r\n************************************************\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Generate a table of task stats. */
	strcpy( pcWriteBuffer, pcHeader );
	vTaskList( pcWriteBuffer + strlen( pcHeader ) );

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

static BaseType_t prvVersionCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* A2: MCU, 40: read version, 0004: 4bytes data, 0100: data(Version_Major+Version_Minor) */
	sprintf( pcWriteBuffer, "A2800400%02d%02d\r\n", Version_Major, Version_Minor);

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

void vRegisterCLICommands( void )
{
	uint32_t i;

	for (i=0;i<sizeof(xCommandList)/sizeof(CLI_Command_Definition_t);i++)
	{
		FreeRTOS_CLIRegisterCommand(&xCommandList[i]);
	}


	#if( configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1 )
	{
		FreeRTOS_CLIRegisterCommand( & xStartStopTrace );
	}
	#endif
}

