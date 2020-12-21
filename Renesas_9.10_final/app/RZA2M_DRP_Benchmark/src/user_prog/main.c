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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : main.c
* $Rev: 8798 $
* $Date:: 2018-07-06 19:53:30 +0800#$
* Device(s)    : RZ/A2M
* Tool-Chain   : e2Studio Ver 7.6.0 or higher
*              : GCC ARM Embedded 7-2017-q4-major
* OS           : None
* H/W Platform : RZ/A2M Evaluation Board
* Description  : RZ/A2M DRP and openCV benchmark Demo
* Operation    :
*    +---------------------+
*	 | =       = = =       | ON
*	 |   = = =       = = = | OFF
*	 | 1 2 3 4 5 6 7 8 9 0 |
*	 +---------------------+
*	 Please make sure your sub-board SW6 matches these switch settings.
*	 SW6-1: ON to enable SDRAM function
*	 SW6-6/7: ON to enable VDC6 function
*
* Limitations  :
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "main.h"
#include "r_bsp.h"
#include "application_cfg.h"
#include <stdio.h>
#include <stdlib.h>
#include "r_bcd_main.h"

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* Note ucHeap is used in freeRTOS for fixed location fixed sized memory pool */
uint8_t ucHeap[ 128*1024 ] __attribute__ ((section(".freertos_heap")));	//heap on internal RAM

/* Notice OS Kernel where is the heap */
/* 1. In xHeapRegions array, region address should be sorted by ascending, couldn't be descending */
/* 2. Heap can cross memory devices, but must follow the rule of number 1, HyperRAM address < onchip RAM address */
HeapRegion_t xHeapRegions[] =
{
    /* Initialise default region in the function . */
	{ (void *)&ucHeap, 128*1024 },
	{ NULL, 0 },
	{ NULL, 0 },
	{ NULL, 0 },
    /* Terminates the array. */
    { NULL, 0 }
};

extern void r_test(void);

static void test_task( void *param )
{
	r_test();
}

/******************************************************************************
* Function Name: os_main_task_t
* Description  : FreeRTOS main task called by R_OS_KernelInit()
*              : FreeRTOS is now configured and R_OS_Abstraction calls
*              : can be used.
*              : From this point forward no longer use direct_xxx calls.
*              : For example
*              : in place of   direct_open("ostm2", O_RDWR);
*              : use           open(DEVICE_INDENTIFIER "ostm2", O_RDWR);
*              :
* Arguments    : none
* Return Value : 0
******************************************************************************/
int_t os_main_task_t(void)
{
//	OsTask *ptask_test;

//	ptask_test      = R_OS_TaskCreate("Test Task", test_task, NULL, R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, 7);

    sample_main();

    while(1)
    {
    	/* Do Nothing */
    }
}

/******************************************************************************
* Function Name: main
* Description  : C Entry point
*              : opens and configures cpg driver
*              : starts the freertos kernel
* Arguments    : none
* Return Value : 0
******************************************************************************/
int_t main(void)
{


    /* Start FreeRTOS */
    /* R_OS_InitKernel should never return */
    R_OS_RegistrMainTaskCb((void*)os_main_task_t);

    R_OS_KernelInit();

}

/* End of File */
