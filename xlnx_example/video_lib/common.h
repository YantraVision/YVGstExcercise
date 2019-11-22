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
* @file common.h
*
* This file defines common macros/defines for video library.
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
#ifndef COMMON_H
#define COMMON_H

/* Common defines / utils for video_lib */

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>
#include <linux/videodev2.h>
#include <linux/media.h>
#include <mediactl/mediactl.h>
#include <mediactl/v4l2subdev.h>

#include "helper.h"
#include "video.h"
#include "video_int.h"

#if 0
/* Media Pipeline configuration */
#if defined(PLATFORM_ZCU106)
	#define MEDIA_TPG_ENTITY "a3c30000.tpg"
	#define MEDIA_HLS_ENTITY "a3c10000.hls"
	#define MEDIA_SCALER_ENTITY "a0080000.scaler"
	//#define MEDIA_SCALER_FMT_OUT     "UYVY"
	#define MEDIA_SCALER_FMT_OUT     "VYYUYY8"
	//#define MEDIA_SCALER_FMT_OUT     "0x2026"
	#define MEDIA_BUS_FMT_VYYUYY8_1X24 0x2026
	#define MEDIA_SCALER_FMT_IN     "VUY24"
	#define MEDIA_HDMI_ENTITY "a0000000.hdmi_rx_ss"
	#define MEDIA_TPG_FMT "\"a00e0000.tpg\":%d [fmt:UYVY/%dx%d]"
	#define MEDIA_HLS_FMT "\"a3c10000.hls\":%d [fmt:UYVY/%dx%d]"
	#define MEDIA_RGB2YUV_FMT "\"a0030000.rgb2yuv\":%d [fmt:RBG/%dx%d field:none]"
	#define MEDIA_CRESAMPLER_FMT "\"a0040000.cresample\":%d [fmt:VUY24/%dx%d field:none]"
	#define MEDIA_SCALER_FMT "\"a3c40000.scaler\":%d [fmt:UYVY/%dx%d]"
	//#define MEDIA_HDMI_FMT "\"a0000000.v_hdmi_rx_ss\":%d [fmt:UYVY/%dx%d]"
	#define MEDIA_HDMI_FMT "\"a0000000.v_hdmi_rx_ss\":%d [fmt:RBG24/%dx%d]"
	#define MEDIA_HDMI_PAD  "\"a0000000.hdmi_rx_ss\":0"
	#define MEDIA_TPG_PAD  "\"a00e0000.tpg\":0"
	#define MEDIA_SCALER_PAD "\"a0080000.scaler\":1"
#elif defined(PLATFORM_ZC1751_DC1)
	#define MEDIA_TPG_ENTITY "a3c30000.tpg"
	#define MEDIA_HLS_ENTITY "a3c10000.hls"
	#define MEDIA_SCALER_ENTITY "a3c40000.scaler"
	#define MEDIA_HDMI_ENTITY "adv7611 12-004c"
	#define MEDIA_TPG_FMT "\"a3c30000.tpg\":%d [fmt:UYVY/%dx%d]"
	#define MEDIA_HLS_FMT "\"a3c10000.hls\":%d [fmt:UYVY/%dx%d]"
	#define MEDIA_SCALER_FMT "\"a3c40000.scaler\":%d [fmt:UYVY/%dx%d]"
	#define MEDIA_HDMI_FMT "\"adv7611 12-004c\":%d [fmt:UYVY/%dx%d]"
	#define MEDIA_HDMI_PAD  "\"adv7611 12-004c\":1"
#else
	#define MEDIA_CRESAMPLER_FMT	"%d%d%d" // unused
	#define MEDIA_HDMI_PAD			" "
	#define MEDIA_HDMI_ENTITY		" "
	#define MEDIA_HLS_ENTITY		" " // unused
	#define MEDIA_HLS_FMT 			"%d%d%d" // unused
	#define MEDIA_RGB2YUV_FMT 		"%d%d%d" // unused
	#define MEDIA_SCALER_FMT 		"%d%d%d" // unused
	#define MEDIA_TPG_ENTITY 		" "
	#define MEDIA_TPG_FMT 			"%d%d%d" // unused
#endif

#else

/* Media Pipeline configuration */
        #define MEDIA_TPG_ENTITY "a3c30000.tpg"
        #define MEDIA_HLS_ENTITY "a3c10000.hls"
        #define MEDIA_SCALER_ENTITY "a0080000.scaler"
        //#define MEDIA_SCALER_FMT_OUT     "UYVY"
        #define MEDIA_SCALER_FMT_OUT     "VYYUYY8"
        //#define MEDIA_SCALER_FMT_OUT     "0x2026"
        #define MEDIA_BUS_FMT_VYYUYY8_1X24 0x2026
        #define MEDIA_SCALER_FMT_IN     "VUY24"
        #define MEDIA_HDMI_ENTITY "a0000000.hdmi_rx_ss"
        #define MEDIA_TPG_FMT "\"a00e0000.tpg\":%d [fmt:UYVY/%dx%d]"
        #define MEDIA_HLS_FMT "\"a3c10000.hls\":%d [fmt:UYVY/%dx%d]"
        #define MEDIA_RGB2YUV_FMT "\"a0030000.rgb2yuv\":%d [fmt:RBG/%dx%d field:none]"
        #define MEDIA_CRESAMPLER_FMT "\"a0040000.cresample\":%d [fmt:VUY24/%dx%d field:none]"
        #define MEDIA_SCALER_FMT "\"a3c40000.scaler\":%d [fmt:UYVY/%dx%d]"
        //#define MEDIA_HDMI_FMT "\"a0000000.v_hdmi_rx_ss\":%d [fmt:UYVY/%dx%d]"
        #define MEDIA_HDMI_FMT "\"a0000000.hdmi_rx_ss\":%d [fmt:RBG24/%dx%d]"
        #define MEDIA_HDMI_PAD  "\"a0000000.hdmi_rx_ss\":0"
        #define MEDIA_TPG_PAD  "\"a00e0000.tpg\":0"
        #define MEDIA_SCALER_PAD "\"a0080000.scaler\":1"

#endif


#define DEV_NAME_LEN 32
#define POLL_TIMEOUT_MSEC 250
int warn(char *args, ... );
#define ARRAY_SIZE(a)  (sizeof(a)/sizeof((a)[0]))
#define VDMA_SKIP_FRM_INDEX 0
#define MAX_EVENT_CNT 4

typedef enum
{
	MODE_INIT,
	MODE_CHANGE,
	MODE_EXIT
} app_state;

/*  media configuration */
typedef enum
{
	MEDIA_NODE_0,
	MEDIA_NODE_1,
	MEDIA_NODE_2
} media_node;

struct event_counter {
	char *name;
	int counter_val;
	int sampled_val;
};

struct video_format {
	unsigned int w;
	unsigned int h;
	unsigned int stride;
	unsigned int fourcc;
};

/* global setup for all modes */
struct video_pipeline {
	unsigned int w, h;
	unsigned int w_out, h_out;
	unsigned int fps;
	unsigned int num_planes;
	unsigned int vtotal, htotal;
	unsigned int frame_rate;
	unsigned int stride;
	unsigned int in_fourcc;
	unsigned int out_fourcc;
	int app_state;
	int hdmii_present;
	video_src vid_src;
	pthread_t eventloop;
	int pr_enable;
	struct event_counter events[MAX_EVENT_CNT];
	int active_events;
	int hdmi_in_width;
	int hdmi_in_height;
};



#if 0
const struct {
	const char *name;
	enum v4l2_mbus_pixelcode code;
} mbus_formats[] = {
	{ "Y8", MEDIA_BUS_FMT_Y8_1X8},
	{ "Y10", MEDIA_BUS_FMT_Y10_1X10 },
	{ "Y12", MEDIA_BUS_FMT_Y12_1X12 },
	{ "YUYV", MEDIA_BUS_FMT_YUYV8_1X16 },
	{ "YUYV1_5X8", MEDIA_BUS_FMT_YUYV8_1_5X8 },
	{ "YUYV2X8", MEDIA_BUS_FMT_YUYV8_2X8 },
    { "UYVY", MEDIA_BUS_FMT_UYVY8_1X16 },
	{ "UYVY1_5X8", MEDIA_BUS_FMT_UYVY8_1_5X8 },
	{ "UYVY2X8", MEDIA_BUS_FMT_UYVY8_2X8 },
	{ "VUY24", MEDIA_BUS_FMT_VUY8_1X24 },
	{ "SBGGR8", MEDIA_BUS_FMT_SBGGR8_1X8 },
	{ "SGBRG8", MEDIA_BUS_FMT_SGBRG8_1X8 },
	{ "SGRBG8", MEDIA_BUS_FMT_SGRBG8_1X8 },
	{ "SRGGB8", MEDIA_BUS_FMT_SRGGB8_1X8 },
	{ "SBGGR10", MEDIA_BUS_FMT_SBGGR10_1X10 },
	{ "SGBRG10", MEDIA_BUS_FMT_SGBRG10_1X10 },
	{ "SGRBG10", MEDIA_BUS_FMT_SGRBG10_1X10 },
	{ "SRGGB10", MEDIA_BUS_FMT_SRGGB10_1X10 },
	{ "SBGGR10_DPCM8", MEDIA_BUS_FMT_SBGGR10_DPCM8_1X8 },
	{ "SGBRG10_DPCM8", MEDIA_BUS_FMT_SGBRG10_DPCM8_1X8 },
	{ "SGRBG10_DPCM8", MEDIA_BUS_FMT_SGRBG10_DPCM8_1X8 },
	{ "SRGGB10_DPCM8", MEDIA_BUS_FMT_SRGGB10_DPCM8_1X8 },
	{ "SBGGR12", MEDIA_BUS_FMT_SBGGR12_1X12 },
	{ "SGBRG12", MEDIA_BUS_FMT_SGBRG12_1X12 },
	{ "SGRBG12", MEDIA_BUS_FMT_SGRBG12_1X12 },
	{ "SRGGB12", MEDIA_BUS_FMT_SRGGB12_1X12 },
	{ "AYUV32", MEDIA_BUS_FMT_AYUV8_1X32 },
	{ "RBG24", MEDIA_BUS_FMT_RBG888_1X24 },
	{ "RGB32", MEDIA_BUS_FMT_RGB888_1X32_PADHI },
	{ "ARGB32", MEDIA_BUS_FMT_ARGB8888_1X32 },
	{ "VYYUYY8", MEDIA_BUS_FMT_VYYUYY8_1X24},
};


const char *v4l2_subdev_pixelcode_to_string(enum v4l2_mbus_pixelcode code)
{
	unsigned int i;
	//printf("ENTER v4l2_subdev_pixelcode_to_string\n");

	for (i = 0; i < ARRAY_SIZE(mbus_formats); ++i) {
		//printf("code: %d, name: %s\n",  code, mbus_formats[i].name);
		if (mbus_formats[i].code == code)
		{
			//printf("matched code: %d\n", code);
			return mbus_formats[i].name;
		}
	}

	return "unknown";
}

enum v4l2_mbus_pixelcode v4l2_subdev_string_to_pixelcode(const char *string, unsigned int length)
{
    	unsigned int i;
	//printf("input string...........................................: %s\n", string);

	for (i = 0; i < ARRAY_SIZE(mbus_formats); ++i) {
		//printf("mbus_name:...........................................: %s\n", mbus_formats[i].name);
		if (strncmp(mbus_formats[i].name, string, length) == 0)
			break;
	}

	//printf("i: %d, array_size: %d\n", i,  ARRAY_SIZE(mbus_formats));

	if (i == ARRAY_SIZE(mbus_formats))
		return (enum v4l2_mbus_pixelcode)-1;

	//printf("code.................................................................: %d\n",  mbus_formats[i].code);
	return mbus_formats[i].code;
}

#endif

#if 0
struct media_entity *media_get_entity_by_name(struct media_device *media,
					      const char *name)
{
	unsigned int i;

	for (i = 0; i < media->entities_count; ++i) {
		struct media_entity *entity = &media->entities[i];
		printf("\n\nentity info name: %s, name: %s\n", entity->info.name, name);

		if (strncmp(entity->info.name, name, strlen(name)) == 0)
		{
			return entity;
		}
	}

	return NULL;
}
#endif

#endif
