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
* @file v4l2_helper.h
*
* This file defines the helper functions for v4l2 video devices.
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
#ifndef V4L2_HELPER_H
#define V4L2_HELPER_H

#include <linux/xilinx-v4l2-controls.h>
#include "common.h"

struct buffer {
	unsigned int index;
	int dmabuf_fd[4];
	unsigned int offsets[4];
	unsigned int lengths[4];
	unsigned char *v4l2_buff[4];
	unsigned int v4l2_buff_length;
	unsigned int num_planes;
};

/* video device */
struct v4l2_dev {
	char devname[DEV_NAME_LEN];		/* device name */
	int fd;					/* device node fd */
	enum v4l2_buf_type buf_type;   /* type of buffer */
	enum v4l2_memory mem_type;  /* type of memory */
	struct v4l2_pix_format_mplane format;
	struct buffer  vid_buf[ MAX_BUFFER_CNT];
	struct video_pipeline *setup_ptr;
};

/* Return video node for card name */
int v4l2_parse_node(const char *cardname, char *devnode);
/* Initialize v4l2 video device */
int v4l2_init(struct v4l2_dev *, unsigned int );
/* Initialize v4l2 video device */
int v4l2_get_format(struct v4l2_dev *dev, struct v4l2_format *fmt, struct v4l2_pix_format_mplane *reqFmt, uint32_t *reqFourCC);
/* Queue buffer to video device */
void v4l2_queue_buffer(struct v4l2_dev *, const struct buffer *);
/* Dequeue buffer from video device */
struct buffer *v4l2_dequeue_buffer(struct v4l2_dev *, struct buffer *);
/* Start the capture or output process during streaming */
int  v4l2_device_off(struct v4l2_dev *);
/*Stop the capture or output process during streaming */
int  v4l2_device_on(struct v4l2_dev *);

#endif /* V4L2_HELPER_H */
