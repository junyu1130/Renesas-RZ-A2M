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
* Rev: 1.00
* Description : IO define header
*******************************************************************************/

#ifndef OTP_IODEFINE_H
#define OTP_IODEFINE_H

struct st_otp
{
    char           wk0[2];
    char           wk1[2];
    char           wk2[2];
    char           wk3[2];
    char           wk4[2];
    char           wk5[2];
    char           wk6[2];
    char           wk7[2];
    char           wk8[2];
    char           wk9[2];
    char           wk10[2];
    char           wk11[2];
    char           wk12[2];
    char           wk13[2];
    char           wk14[2];
    char           wk15[2];
    char           wk16[2];
    char           wk17[2];
    char           wk18[2];
    char           wk19[2];
    char           wk20[2];
    char           wk21[2];
    char           wk22[2];
    char           wk23[34];
    char           wk24[2];
    char           wk25[2];
    char           wk26[2];
    char           wk27[2];
    char           wk28[2];
    char           wk29[2];
    char           wk30[2];
    char           wk31[2];
    char           wk32[2];
    char           wk33[2];
    char           wk34[2];
    char           wk35[2];
    char           wk36[2];
    char           wk37[16246];
    char           wk38[2];
    char           wk39[2];
    char           wk40[2];
    char           wk41[2];
    char           wk42[2];
    char           wk43[2];
    char           wk44[2];
    char           wk45[2];
    char           wk46[2];
    char           wk47[2];
    char           wk48[2];
    char           wk49[2];
    char           wk50[2];
    char           wk51[2];
    char           wk52[2];
    char           wk53[2];
    char           wk54[2];
    char           wk55[2];
    char           wk56[2];
    char           wk57[2];
    char           wk58[2];
    char           wk59[2];
    char           wk60[2];
    char           wk61[2];
    char           wk62[2];
    char           wk63[2];
    char           wk64[2];
    char           wk65[2];
    char           wk66[2];
    char           wk67[2];
    char           wk68[2];
    char           wk69[2];
    char           wk70[2];
    char           wk71[2];
    char           wk72[2];
    char           wk73[2];
    char           wk74[2];
    char           wk75[2];
    char           wk76[2];
};

#define OTP       (*(volatile struct st_otp       *)0xE8030020)

#endif
