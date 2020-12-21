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
 * File Name    : trace.h
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * H/W Platform : Stream it! v2
 * Description  : Debugging facilities
 *
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 21.07.2016 1.00 First Release
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
 * @version 1.6.1
 **/

#ifndef _TRACE_H
#define _TRACE_H


#include "r_typedefs.h"

/* Trace level definitions */
#define TRACE_LEVEL_OFF      (0)
#define TRACE_LEVEL_FATAL    (1)
#define TRACE_LEVEL_ERROR    (2)
#define TRACE_LEVEL_WARNING  (3)
#define TRACE_LEVEL_INFO     (4)
#define TRACE_LEVEL_DEBUG    (5)

/* Default trace level */
#ifndef TRACE_LEVEL
    #define TRACE_LEVEL             (TRACE_LEVEL_DEBUG)
#endif

/* Deprecated definitions */
#define TRACE_LEVEL_NO_TRACE        (TRACE_LEVEL_OFF)

/* Trace output redirection */
#ifndef TRACE_PRINTF
    #define TRACE_PRINTF(...)       (fprintf(stderr, __VA_ARGS__))
#endif

#ifndef TRACE_ARRAY
    #define TRACE_ARRAY(p, a, n)    (debugDisplayArray(stderr, (p), (a), (n)))
#endif

#ifndef TRACE_MPI
    #define TRACE_MPI(p, a)         (mpiDump(stderr, (p), (a)))
#endif

/* Debugging macros */
#if (TRACE_LEVEL >= TRACE_LEVEL_FATAL)
    #define TRACE_FATAL(...)        (TRACE_PRINTF(__VA_ARGS__))
#else
   #define TRACE_FATAL(...)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_ERROR)
   #define TRACE_ERROR(...) (TRACE_PRINTF(__VA_ARGS__))
#else
   #define TRACE_ERROR(...)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_WARNING)
   #define TRACE_WARNING(...) (TRACE_PRINTF(__VA_ARGS__))
#else
   #define TRACE_WARNING(...)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_INFO)
    #define TRACE_INFO(...)                     (TRACE_PRINTF(__VA_ARGS__))
    #define TRACE_INFO_ARRAY(p, a, n)           (TRACE_ARRAY((p), (a), (n)))
    #define TRACE_INFO_NET_BUFFER(p, b, o, n)
    #define TRACE_INFO_MPI(p, a)                (TRACE_MPI((p), (a)))
#else
    #define TRACE_INFO(...)
    #define TRACE_INFO_ARRAY(p, a, n)
    #define TRACE_INFO_NET_BUFFER(p, b, o, n)
    #define TRACE_INFO_MPI(p, a)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
    #define TRACE_DEBUG(...)                     (TRACE_PRINTF(__VA_ARGS__))
    #define TRACE_DEBUG_ARRAY(p, a, n)           (TRACE_ARRAY((p), (a), (n)))
    #define TRACE_DEBUG_NET_BUFFER(p, b, o, n)
    #define TRACE_DEBUG_MPI(p, a)                (TRACE_MPI((p), (a)))
#else
    #define TRACE_DEBUG(...)
    #define TRACE_DEBUG_ARRAY(p, a, n)
    #define TRACE_DEBUG_NET_BUFFER(p, b, o, n)
    #define TRACE_DEBUG_MPI(p, a)
#endif


#endif
