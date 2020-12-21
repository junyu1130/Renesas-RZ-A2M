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
* Copyright (C) 2012 - 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_bsc_lld_rza2m.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Common driver
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "../../inc/r_bsc_drv_api.h"         /* Common Driver Header */
#include "iodefine.h"
#include "iobitmask.h"

#ifdef __CC_ARM
#pragma arm section code   = "CODE_BASIC_SETUP"
#pragma arm section rodata = "CONST_BASIC_SETUP"
#pragma arm section rwdata = "DATA_BASIC_SETUP"
#pragma arm section zidata = "BSS_BASIC_SETUP"
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
	/* The final step is to set the SDRAM Mode Register by written to a
	   specific address (the data value is ignored) */
	/* Check the hardware manual (table 8.11) if your settings differ */
	/*   Burst Length = 1 (fixed)
	 *   Burst Type = Sequential (fixed)
	 *   CAS Latency = 2 or 3 (see table 8.11)
	 *   Write Burst Mode = [burst read/single write] or [burst read/burst write] (see table 8.15)
	 */

	//#define SDRAM_MODE_CS2 0x1F001440	/* CS2: CAS=2, single write, 16bit bus */
	//#define SDRAM_MODE_CS3 0x1F002440	/* CS3: CAS=2, single write, 16bit bus */

	//#define SDRAM_MODE_CS2 0x1F001460	/* CS2: CAS=3, single write, 16bit bus */
	//#define SDRAM_MODE_CS3 0x1F002460	/* CS3: CAS=3, single write, 16bit bus */

	//#define SDRAM_MODE_CS2 0x1F001040	/* CS2: CAS=2, burst write, 16bit bus */
	//#define SDRAM_MODE_CS3 0x1F002040	/* CS3: CAS=2, burst write, 16bit bus */

#define SDRAM_MODE_CS2 0x1F001060	/* CS2: CAS=3, burst write, 16bit bus */
#define SDRAM_MODE_CS3 0x1F002060	/* CS3: CAS=3, burst write, 16bit bus */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ==== User-defined functions ==== */
static void Userdef_BSC_CS0Init(void);
static void Userdef_BSC_CS1Init(void);
static void Userdef_BSC_CS2Init(void);
static void Userdef_BSC_CS3Init(void);
static void Userdef_BSC_CS4Init(void);
static void Userdef_BSC_CS5Init(void);

/******************************************************************************
* Function Name: Userdef_BSC_CS0Init
* Description  : This is the user-defined function called by the R_BSC_Init 
*              : function. The setting for initialization of the BSC in the CS0 
*              : space is required. In this sample code, the setting to use 
*              : the NOR flash memory in the CS0 space is executed. Sets the BSC 
*              : to connect the Spansion NOR flash memory S29GL512S10T to the 
*              : CS0 space with 16-bit bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Userdef_BSC_CS0Init(void)
{
    /* ---- CS0BCR settings ---- */
    BSC.CS0BCR.LONG = 0x10000C00uL;
                                    /* Idle Cycles between Write-read Cycles    */
                                    /*  and Write-write Cycles : 1 idle cycle   */
                                    /* Data Bus Size: 16-bit                    */

    /* ---- CS0WCR settings ----  */
    BSC.CS0WCR.CS0WCR_0.LONG = 0x00000B40uL;
                                    /* Number of Delay Cycles from Address,     */
                                    /*  CS0# Assertion to RD#,WEn Assertion     */
                                    /*  : 1.5 cycles                            */
                                    /* Number of Access Wait Cycles: 6 cycles   */
                                    /* Delay Cycles from RD,WEn# negation to    */
                                    /*  Address,CSn# negation: 0.5 cycles       */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS1Init
* Description  : This is the user-defined function called by the R_BSC_Init 
*              : function. The setting for initialization of the BSC in the CS1 
*              : space is required. In this sample code, the setting to use 
*              : the NOR flash memory in the CS1 space is executed. Sets the BSC 
*              : to connect the Spansion NOR flash memory S29GL512S10T to the 
*              : CS1 space with 16-bit bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Userdef_BSC_CS1Init(void)
{
    /* ---- CS1BCR settings ---- */
    BSC.CS1BCR.LONG = 0x10000C00uL;
                                    /* Idle Cycles between Write-read Cycles    */
                                    /*  and Write-write Cycles : 1 idle cycle   */
                                    /* Data Bus Size: 16-bit                    */

    /* ---- CS1WCR settings ----  */
    BSC.CS1WCR_0.LONG = 0x00000B40uL;
                                    /* Number of Delay Cycles from Address,     */
                                    /*  CS1# Assertion to RD#,WEn Assertion     */
                                    /*  : 1.5 cycles                            */
                                    /* Number of Access Wait Cycles: 6 cycles   */
                                    /* Delay Cycles from RD,WEn# negation to    */
                                    /*  Address,CSn# negation: 0.5 cycles       */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS2Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the BSC in the CS2
*              : space is required. In this sample code, the setting to use
*              : the SDRAM in the CS2 space is executed. Sets the BSC to 
*              : connect the ISSI IS42S16320B-75 to the CS2 space with 16-bit
*              : bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Userdef_BSC_CS2Init(void)
{
    /* ==== CS2BCR settings ==== */
    BSC.CS2BCR.LONG = 0x00004C00uL;
                                /* Idle Cycles between Write-read Cycles  */
                                /* and Write-write Cycles : 0 idle cycles */
                                /* Memory type :SDRAM                     */
                                /* Data Bus Size : 16-bit                 */

    /* ==== CS2WCR settings ==== */
    BSC.CS2WCR.CS2WCR_0.LONG = 0x00000480uL;
                                /* CAS latency for Area 2 : 2 cycles */


    /* ==== Written in SDRAM Mode Register ==== */
    *(uint16_t *)SDRAM_MODE_CS2 = 0;
                                /* The writing data is arbitrary            */
                                /* SDRAM mode register setting CS2 space    */
                                /* Burst read (burst length 1)./Burst write */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS3Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the BSC in the CS3
*              : space is required. In this sample code, the setting to use
*              : the SDRAM in the CS3 space is executed. Sets the BSC to 
*              : connect the ISSI IS42S16320B-75 to the CS3 space with 16-bit
*              : bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Userdef_BSC_CS3Init(void)
{
    volatile int32_t cnt;

    /* NOTE: Secure wait time according to SDRAM specification. */
    cnt = 100;
    while (cnt-- > 0)
    {
        /* wait */
    	asm("nop");
    }

    /* ==== CS3BCR settings ==== */
    BSC.CS3BCR.LONG = 0x00004C00uL;
                                /* Idle Cycles between Write-read Cycles  */
                                /* and Write-write Cycles : 0 idle cycles */
                                /* Memory type :SDRAM                     */
                                /* Data Bus Size : 16-bit                 */

    /* ==== CS3WCR settings ==== */
    BSC.CS3WCR.CS3WCR_0.LONG = 0x00002D13uL;
				/* Number of precharge completion wait cycles: WTRP[1:0] = B'01; 1 cycle */
				/* Number of ACTV command �� READ (A) /WRIT (A) command interval wait */
				/* cycles: WTRCD[1:0] = B'11; 3 cycle */
				/* Area 3CAS latency: A3CL[1:0] = B'10; 3 cycle */
				/* Number of precharge start wait cycles: TRWL[1:0] = B'10; 2 cycle */
				/* Number of REF command/ self refresh cancel ��ACTV/REF/MRS command */
				/* interval idle cycles: WTRC[1:0] = B'11; 8 cycle */

    /* ==== SDCR settings ==== */
    BSC.SDCR.LONG = 0x00120812uL;
                                /* Row address for Area 2 : 13-bit    */
                                /* Column Address for Area 2 : 10-bit */
                                /* Refresh Control :Refresh           */
                                /* RMODE :Auto-refresh is performed   */
                                /* BACTV :Auto-precharge mode         */
                                /* Row address for Area 3 : 13-bit    */
                                /* Column Address for Area 3 : 10-bit */

    /* ==== RTCOR settings ==== */
    BSC.RTCOR = 0xA55A0040uL;
                              /* 1 cycle��121 nsec (132 MHz/16 = 8.25 MHz) */
				/* Required refresh interval of this SDRAM: 7.8125 ��sec / time */
				/* 7.8125 ��sec / 121 nsec = 64 (H'40) cycle/ number of refresh */

    /* ==== RTCSR settings ==== */
    BSC.RTCSR.LONG = 0xA55A0010uL;
                                /* Initialization sequence start */
                                /* Clock select CKIO-phy/16      */
                                /* Refresh count :Once           */

    cnt = 0x000033F1;
    while (cnt-- > 0)
    {
        /* wait */
    	asm("nop");
    }

    /* ==== ACADJ settings ==== */
    BSC.ACADJ.LONG = 0x0002000F;//0x0000000AuL;
					/* - Output characteristics adjustment bit: */
					/* SDRODLY = B��0000; Does not delay output delay time */
					/* - Input characteristics adjustment bit: */
					/* SDRIDLY = B��1010; Delays input data import timin */

    /* ==== Written in SDRAM Mode Register ==== */
//    *(uint16_t *)SDRAM_MODE_CS2 = 0;
    *(uint16_t *)SDRAM_MODE_CS3 = 0;

   	GPIO.PCKIO.BIT.CKIO_DRV = 2;
   	/* 12mA (recommended for 132-MHz operation) */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS4Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the CS4 space is 
*              : required. 
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Userdef_BSC_CS4Init(void)
{
    /* Add processing when using CS4 space */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS5Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function The setting for initialization of the CS5 space is 
*              : required.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Userdef_BSC_CS5Init(void)
{
    /* Add processing when using CS5 space */
}


/******************************************************************************
* Function Name: R_BSC_Init
* Description  : Executes the setting to initialize the BSC function.
*              : Sets the BSC channels by calling the Userdef_BSC_CSnInit of
*              : the user-defined function.
* Arguments    : uint8 area : Specifies the CS space to be initialized
*              :     <Bit 0> 1 : Initialize CS0 space
*              :             0 : Do not initialize CS0 space
*              :     <Bit 1> 1 : Initialize CS1 space
*              :             0 : Do not initialize CS1 space
*              :     <Bit 2> 1 : Initialize CS2 space
*              :             0 : Do not initialize CS2 space
*              :     <Bit 3> 1 : Initialize CS3 space
*              :             0 : Do not initialize CS3 space
*              :     <Bit 4> 1 : Initialize CS4 space
*              :             0 : Do not initialize CS4 space
*              :     <Bit 5> 1 : Initialize CS5 space
*              :             0 : Do not initialize CS5 space
*              :     <Bit 6-7> Reserved
* Return Value : none
******************************************************************************/
void R_BSC_Init(uint8_t area)
{
    /* WARNING:                                                           */
    /* The section area for the RW data or the ZI data is not initialized */
    /* because this function is called by the Peripheral_BasicInit        */
    /* function. Do not use the variables allocated to the section area   */
    /* for the RW data or the ZI data within this function and the user-  */
    /* defined function called by this function.                          */

    /* ==== BSC initialize ==== */
    if ((area & BSC_AREA_CS0) != 0)     /* Initialize CS0 space */
    {
        Userdef_BSC_CS0Init();
    }
    if ((area & BSC_AREA_CS1) != 0)     /* Initialize CS1 space */
    {
        Userdef_BSC_CS1Init();
    }
    if ((area & BSC_AREA_CS2) != 0)     /* Initialize CS2 space */
    {
        Userdef_BSC_CS2Init();
    }
    if ((area & BSC_AREA_CS3) != 0)     /* Initialize CS3 space */
    {
        Userdef_BSC_CS3Init();
    }
    if ((area & BSC_AREA_CS4) != 0)     /* Initialize CS4 space */
    {
        Userdef_BSC_CS4Init();
    }
    if ((area & BSC_AREA_CS5) != 0)     /* Initialize CS5 space */
    {
        Userdef_BSC_CS5Init();
    }
}


/* End of File */

