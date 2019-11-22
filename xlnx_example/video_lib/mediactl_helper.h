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
* @file mediactl_helper.h
*
* This file defines the  helper functions for media library.
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


#ifndef MEDIA_CTL_H
#define MEDIA_CTL_H

#include "common.h"

/* Display media device info */
void print_media_info(const struct media_device_info *info);
/* Configure media device entities */
int set_media_control(struct video_pipeline *s, media_node dev);
/* Return entity connected/disconnected state */
int get_entity_state(media_node dev, const char *name);
/*Retrieve the detected digital video timings */
int query_entity_dv_timings(media_node dev, char *name, struct v4l2_dv_timings *timings);
int query_entity_active_format(media_node node ,char *entity_name, struct v4l2_mbus_framefmt *format);
/*Returns the full path and name to the device node */
int get_entity_devname(media_node dev, char *name, char *subdev_name);

void media_set_fmt_str(char *set_fmt, const char *entity, unsigned int pad,
                       const char *fmt, unsigned int width, unsigned int height);

#endif
