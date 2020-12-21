@/*******************************************************************************
@* DISCLAIMER
@* This software is supplied by Renesas Electronics Corporation and is only
@* intended for use with Renesas products. No other uses are authorized. This
@* software is owned by Renesas Electronics Corporation and is protected under
@* all applicable laws, including copyright laws.
@* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
@* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
@* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
@* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
@* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
@* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
@* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
@* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
@* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
@* Renesas reserves the right, without notice, to make changes to this software
@* and to discontinue the availability of this software. By using this software,
@* you agree to the additional terms and conditions found by accessing the
@* following link:
@* http://www.renesas.com/disclaimer
@* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
@*******************************************************************************/
.section ".data"
@In this test case, we will load too many DRP libs, in order to reduce memory,
@we allocate DRP lib into flash, but need more time to load (1.x vs internal RAM)


@********Below libraries are allocated in Flash to reduce bss mem size *************
.section ".rodata"

.balign 32
.global g_drp_lib_bayer2grayscale
g_drp_lib_bayer2grayscale:
.incbin "r_drp_bayer2grayscale.dat"

.balign 32
.global g_drp_lib_binarization_fixed
g_drp_lib_binarization_fixed:
.incbin "r_drp_binarization_fixed.dat"

.balign 32
.global g_drp_lib_resize_bilinear
g_drp_lib_resize_bilinear:
.incbin "r_drp_resize_bilinear.dat"

.balign 32
.global g_drp_lib_resize_nearest
g_drp_lib_resize_nearest:
.incbin "r_drp_resize_nearest.dat"

.end
