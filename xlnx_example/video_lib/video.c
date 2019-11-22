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
 * @file video_lib.c
 *
 *  This file implements video library interface.
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


#include "video.h"
#include "video_int.h"
#include "common.h"
#include "mediactl_helper.h"
#define DEBUG_MODE
/* global variables */
char vlib_errstr[VLIB_ERRSTR_SIZE];
int width_g = MAX_WIDTH;
int height_g = MAX_HEIGHT;
void *(*process_thread_fptr)(void *);

static struct video_pipeline *video_setup;


static struct {
	video_src src;
	const char *display_text;
	const char *entity_name;
	unsigned int enabled;
	int index;
} video_srcs[] = {
	{ VIDEO_SRC_TPG, "Test Pattern Generator", V4L2_ENTITY_TPG, 1, -1 },
	{ VIDEO_SRC_HDMII, "HDMI Input", V4L2_ENTITY, 1, -1 },
};

/***************************************************************************************************************************/
const char *vlib_video_src_display_text(video_src src)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(video_srcs); ++i) {
		if (video_srcs[i].src == src)
			return video_srcs[i].display_text;
	}

	return "Invalid";
}
/***************************************************************************************************************************/

const char *vlib_video_src_entity_name(video_src src)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(video_srcs); ++i) {
		if (video_srcs[i].src == src)
			return video_srcs[i].entity_name;
	}

	return "Invalid";
}
/***************************************************************************************************************************/

unsigned int vlib_video_src_get_enabled(video_src src)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(video_srcs); ++i) {
		if (video_srcs[i].src == src)
			return video_srcs[i].enabled;
	}

	return VLIB_SUCCESS;
}
/***************************************************************************************************************************/

void vlib_video_src_set_index(video_src src, int index)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(video_srcs); ++i) {
		if (video_srcs[i].src == src) {
			video_srcs[i].index = index;
			break;
		}
	}
}
/***************************************************************************************************************************/

int vlib_video_src_get_index(video_src src)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(video_srcs); ++i) {
		if (video_srcs[i].src == src)
			return video_srcs[i].index;
	}
	sprintf(vlib_errstr, "Invald parameter");
	return VLIB_ERROR_INVALID_PARAM;
}

/***************************************************************************************************************************/
video_src vlib_video_src_from_index(int index)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(video_srcs); ++i) {
		if (video_srcs[i].index == index)
			return video_srcs[i].src;
	}
	sprintf(vlib_errstr, "Invald parameter");
	return VLIB_ERROR_INVALID_PARAM;
}

/***************************************************************************************************************************/
int setup_media_pipe(int mnode,int width, int height)
{
	/* Allocate video_setup struct and zero out memory */
	video_setup = calloc (1, sizeof (*video_setup));
	video_setup->app_state = MODE_INIT;
	video_setup->in_fourcc = INPUT_PIX_FMT;
	video_setup->out_fourcc = OUTPUT_PIX_FMT;
	video_setup->pr_enable =0 ;
	video_setup->w = width;
	video_setup->h = height;
	/* Detect Xilinx HDMI status */
	if (get_entity_state(MEDIA_NODE_1, MEDIA_HDMI_ENTITY)){
		video_setup->hdmii_present = 1;
		printf("Media input found.\n");
	}
	else
	{
		printf("Media input not found.\n");
	}

	set_media_control(video_setup, mnode);
	return VLIB_SUCCESS;
}
/***************************************************************************************************************************/

/***************************************************************************************************************************/

int vlib_pipeline_stop()
{
	int ret = VLIB_ERROR_OTHER;
	vlib_dbg("vlib_pipeline_stop\n");
	/* Add cleanup code */
	if(video_setup->eventloop != 0) {
		/* Set application state */
		video_setup->app_state = MODE_EXIT;
		ret = pthread_cancel(video_setup->eventloop);
		ret = pthread_join(video_setup->eventloop,NULL);
		video_setup->eventloop = 0;
		vlib_dbg("vlib_pipeline_stop (pthread_join):: %d \n",ret);
	}
	/* Disable video layer on pipeline stop */
	sprintf(vlib_errstr, "Other Error");
	return ret;
}

/***************************************************************************************************************************/
int vlib_uninit()
{
	free (video_setup);
	return VLIB_SUCCESS;
}

/***************************************************************************************************************************/


/***************************************************************************************************************************/

int  initialize_v4l2()
{

	int ret=0;
	/*initializet the capture pipeline*/
	//ret = init_s2m_pipeline(video_setup);
	if (ret != VLIB_SUCCESS)
	{
		printf("init_s2m_pipeline error %i: %s\n", ret, vlib_errstr);
		return ret;
	}
return VLIB_SUCCESS;

}

int set_v4l2_format()
{
	int ret, fd;
	struct v4l2_format fmt;
	struct v4l2_pix_format_mplane format;
	format.pixelformat = video_setup->in_fourcc;
	format.width = video_setup->w;
	format.height = video_setup->h;
	format.num_planes = video_setup->num_planes;
	format.plane_fmt[0].bytesperline = video_setup->stride;
	format.colorspace = V4L2_COLORSPACE_SRGB;

	fd=open("/dev/video1",O_RDWR);
	if(fd < 0) {
		vlib_err("failed to open /dev/video1 : %s\n", ERRSTR);
		return VLIB_ERROR_INTERNAL;
	}

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

	ret = ioctl(fd, VIDIOC_G_FMT, &fmt);
	ASSERT(ret < 0, "VIDIOC_G_FMT failed: %s\n", ERRSTR);

 	fmt.fmt.pix_mp = format;
        ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
        if(ret < 0)
        {
        	vlib_err("VIDIOC_S_FMT failed: %s New pixel-format not set\n", ERRSTR);
                /* Restore format */
                ret = ioctl(fd, VIDIOC_G_FMT, fmt);
        }
       
	fmt.fmt.pix_mp.pixelformat = video_setup->in_fourcc;
 	//printf("in_fourcc: %d\n", video_setup->in_fourcc);
        ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
        if(ret < 0)
        {
        	vlib_err("VIDIOC_S_FMT failed: %s New pixel-format fourCC not set\n", ERRSTR);
        }

	ret = ioctl(fd, VIDIOC_G_FMT, fmt);
	ASSERT(ret < 0, "VIDIOC_G_FMT failed: %s\n", ERRSTR);

	close(fd);

	return ret;
}

/***************************************************************************************************************************/

/***************************************************************************************************************************/

/***************************************************************************************************************************/

/***************************************************************************************************************************/

/** This function returns a constant NULL-terminated string with the ASCII name of a vlib
 *  error. The caller must not free() the returned string.
 *
 *  \param error_code The \ref vlib_error to return the name of.
 *  \returns The error name, or the string **UNKNOWN** if the value of
 *  error_code is not a known error.
 **/
const char *vlib_error_name(vlib_error error_code)
{
	switch (error_code) {
		case VLIB_ERROR_INTERNAL:
			return "VLIB Internal Error";
		case VLIB_ERROR_CAPTURE:
			return "VLIB Capture Error";
		case VLIB_ERROR_INVALID_PARAM:
			return "VLIB Invalid Parameter Error";
		case VLIB_ERROR_FILE_IO:
			return "VLIB File I/O Error";
		case VLIB_ERROR_OTHER:
			return "VLIB Other Error";
		case VLIB_SUCCESS:
			return "VLIB Success";
		default:
			return "VLIB Unknown Error";
	}
}

/***************************************************************************************************************************/

/** This function returns a string with a short description of the given error code.
 *  This description is intended for displaying to the end user.
 *
 *  The messages always start with a capital letter and end without any dot.
 *  The caller must not free() the returned string.
 *
 *  \returns a short description of the error code in UTF-8 encoding
 **/
char *vlib_strerror()
{
	return vlib_errstr;
}
void vlib_log_str(vlib_log_level level, const char * str)
{
	fputs(str, stdout);
	UNUSED(level);
}

/***************************************************************************************************************************/

void vlib_log_v(vlib_log_level level, const char *format, va_list args)
{
	const char *prefix = "";
	char buf[VLIB_LOG_SIZE];
	int header_len, text_len;

	switch (level) {
		case VLIB_LOG_LEVEL_INFO:
			prefix = "[vlib info] ";
			break;
		case VLIB_LOG_LEVEL_WARNING:
			prefix = "[vlib warning] ";
			break;
		case VLIB_LOG_LEVEL_ERROR:
			prefix = "[vlib error] ";
			break;
		case VLIB_LOG_LEVEL_DEBUG:
			prefix = "[vlib debug] ";
			break;
		case VLIB_LOG_LEVEL_NONE:
		default:
			return;
	}

	header_len = snprintf(buf, sizeof(buf), "%s", prefix);
	if (header_len < 0 || header_len >= (int)sizeof(buf)) {
		/* Somehow snprintf failed to write to the buffer,
		 * remove the header so something useful is output. */
		header_len = 0;
	}
	/* Make sure buffer is NULL terminated */
	buf[header_len] = '\0';

	text_len = vsnprintf(buf + header_len, sizeof(buf) - header_len, format, args);
	if (text_len < 0 || text_len + header_len >= (int)sizeof(buf)) {
		/* Truncated log output. On some platforms a -1 return value means
		 * that the output was truncated. */
		text_len = sizeof(buf) - header_len;
	}

	if (header_len + text_len >= sizeof(buf)) {
		/* Need to truncate the text slightly to fit on the terminator. */
		text_len -= (header_len + text_len) - sizeof(buf);
	}

	vlib_log_str(level, buf);
}


/***************************************************************************************************************************/
void vlib_log(vlib_log_level level, const char *format, ...)
{
	va_list args;

	va_start (args, format);
	vlib_log_v(level, format, args);
	va_end (args);
}


/***************************************************************************************************************************/
