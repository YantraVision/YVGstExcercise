/******************************************************************************
 *
 * Copyright (C) 2018 Xilinx, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

/*****************************************************************************
*
* @file video_lib.h
*
* This file provides video library interface.
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a RSP   14/07/14 Initial release
* </pre>
*
* @note
*
******************************************************************************/

#ifndef VIDEO_INT_H
#define VIDEO_INT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Misc configuration */
#define HRES_2160P      3840
#define VRES_2160P      2160
#define HRES_1080P      1920
#define VRES_1080P      1080
#define HRES_720P       1280
#define VRES_720P        720

#define BYTES_PER_PIXEL    3

#define BUFFER_CNT         4
#define MAX_BUFFER_CNT     5

#define OUTPUT_PIX_FMT v4l2_fourcc('N','V','1','2')
//#define OUTPUT_PIX_FMT v4l2_fourcc('B','G','2','4')
//#define INPUT_PIX_FMT  v4l2_fourcc('Y','U','Y','V')

/* V4L2_PIX_FMT_RGB24 */
//#undef INPUT_PIX_FMT
//#define INPUT_PIX_FMT  v4l2_fourcc('V','U','2','4')
#define INPUT_PIX_FMT  v4l2_fourcc('N','V','1','2')


/* Platform specific configuration */
#if defined(PLATFORM_ZCU106) || defined(PLATFORM_ZC1751_DC1)
//#define MAX_HEIGHT           VRES_2160P
//#define MAX_WIDTH            HRES_2160P
#define MAX_HEIGHT           VRES_1080P
#define MAX_WIDTH            HRES_1080P

#define MAX_STRIDE           MAX_WIDTH
#define DRM_MODULE           DRM_MODULE_GENERIC
#define DRM_ALPHA_PROP       "alpha"
#define DRM_MAX_ALPHA        255
#define FSYNC_SEL_GPIO       2
#else
#define MAX_HEIGHT           VRES_1080P
#define MAX_WIDTH            HRES_1080P
#define MAX_STRIDE           2048
#define DRM_MODULE           DRM_MODULE_GENERIC
#define DRM_ALPHA_PROP       "transparency"
#define DRM_MAX_ALPHA        255
#define FSYNC_SEL_GPIO       1
#undef OUTPUT_PIX_FMT
#define OUTPUT_PIX_FMT v4l2_fourcc('X','R','2','4')
#endif

#ifdef __cplusplus
}
#endif
#endif

