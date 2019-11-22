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

#ifndef VDF_LIB_H
#define VDF_LIB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdarg.h>

#define V4L2_ENTITY "vcap_hdmi output 0"
#define V4L2_ENTITY_TPG "vcap_tpg output 0"
/* Common interface  for video library*/
typedef enum {
	DRM_MODULE_XILINX,
	DRM_MODULE_GENERIC,
	DRM_MODULE_NONE = -1,
} drm_module;

#define VIDEO_SRC_CNT 5

typedef enum {
	VIDEO_SRC_TPG,
	VIDEO_SRC_HDMII,
} video_src;

typedef enum {
	VIDEO_CTRL_OFF,
	VIDEO_CTRL_ON,
	VIDEO_CTRL_AUTO
} video_ctrl;

typedef enum {
	CAPTURE,
	DISPLAY,
	PROCESS_IN,
	PROCESS_OUT,
	NUM_EVENTS
} pipeline_event;

typedef struct {
	video_src src;
	unsigned int type;
} vlib_config;

//#define TRUE    0
//#define FALSE   1

/**
 * Error codes. Most vlib functions return 0 on success or one of these
 * codes on failure.
 * User can call vlib_error_name() to retrieve a string representation of an
 * error code or vlib_strerror() to get an end-user suitable description of
 * an error code.
*/

/* Total number of error codes in enum vlib_error */
#define VLIB_ERROR_COUNT 6

typedef enum {
	VLIB_SUCCESS = 0,
	VLIB_ERROR_INTERNAL = -1,
	VLIB_ERROR_CAPTURE = -2,
	VLIB_ERROR_INVALID_PARAM = -3,
	VLIB_ERROR_FILE_IO = -4,
	VLIB_ERROR_OTHER = -99,
	VLIB_ERROR_EXIT = -100,
} vlib_error;

/* Character-array to store string-representation of the error-codes */
#define VLIB_ERRSTR_SIZE 256
extern char vlib_errstr[VLIB_ERRSTR_SIZE];

/**
 *  Log message levels.
 *  - VLIB_LOG_LEVEL_NONE (0)
 *  - VLIB_LOG_LEVEL_ERROR (1)
 *  - VLIB_LOG_LEVEL_WARNING (2)
 *  - VLIB_LOG_LEVEL_INFO (3)
 *  - VLIB_LOG_LEVEL_DEBUG (4)
 *  All the messages are printed on stdout.
 */
typedef enum {
	VLIB_LOG_LEVEL_NONE = 0,
	VLIB_LOG_LEVEL_ERROR,
	VLIB_LOG_LEVEL_WARNING,
	VLIB_LOG_LEVEL_INFO,
	VLIB_LOG_LEVEL_DEBUG,
} vlib_log_level;

/* Maximum number of bytes in a log line */
#define VLIB_LOG_SIZE 256

/* The following is used to silence warnings for unused variables */
#define UNUSED(var)		do { (void)(var); } while(0)

void vlib_log(vlib_log_level level, const char *format, ...);
void vlib_log_v(vlib_log_level level, const char *format, va_list args);

#define _vlib_log(level, ...) vlib_log(level, __VA_ARGS__)

#ifdef DEBUG_MODE
#define INFO_MODE
#define WARN_MODE
#define ERROR_MODE
#define vlib_dbg(...) _vlib_log(VLIB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define vlib_dbg(...) do {} while(0)
#endif

#ifdef INFO_MODE
#define WARN_MODE
#define ERROR_MODE
#define vlib_info(...) _vlib_log(VLIB_LOG_LEVEL_INFO, __VA_ARGS__)
#else
#define vlib_info(...) do {} while(0)
#endif

#ifdef WARN_MODE
#define ERROR_MODE
#define vlib_warn(...) _vlib_log(VLIB_LOG_LEVEL_WARNING, __VA_ARGS__)
#else
#define vlib_warn(...) do {} while(0)
#endif

#ifdef ERROR_MODE
#define vlib_err(...) _vlib_log(VLIB_LOG_LEVEL_ERROR, __VA_ARGS__)
#else
#define vlib_err(...) do {} while(0)
#endif

/* Platform specific setup */
extern int width_g;
extern int height_g;
extern drm_module module_g;

/* video source helper functions */
const char *vlib_video_src_display_text(video_src src);
const char *vlib_video_src_entity_name(video_src src);
unsigned int vlib_video_src_get_enabled(video_src src);
void vlib_video_src_set_index(video_src src, int index);
int vlib_video_src_get_index(video_src src);
video_src vlib_video_src_from_index(int index);

/* drm helper functions */
int vlib_drm_set_layer0_state(int);
int vlib_drm_set_layer0_transparency(int);
int vlib_drm_set_layer0_position(int, int);
int vlib_drm_get_module_type(const char *str);
const char *vlib_drm_get_module_name(drm_module module);

/* video resolution functions */
int vlib_get_active_height();
int vlib_get_active_width();
unsigned int vlib_get_output_fourcc();
void vlib_set_output_fourcc(unsigned int fourcc);

/* init/uninit functions */
int vlib_init(int pr_enable);
int vlib_uninit();
int vlib_drm_open(drm_module module);
int vlib_drm_init(int width, int height, drm_module module);
int vlib_drm_uninit();
int vlib_drm_close();

/* video pipeline control functions */
int vlib_pipeline_stop();
int vlib_change_mode(vlib_config config , int width ,int height);

int vlib_drm_list_modes(void);
int vlib_drm_set_mode_by_idx(int idx);
int vlib_drm_use_mode_by_idx(int idx);

/* set event-log function */
int vlib_set_event_log(int state);
/* Query pipeline events*/
int vlib_get_event_cnt(pipeline_event event);

/* return the string representation of the error code */
const char *vlib_error_name(vlib_error error_code);
/* return user-readable description of the error-code */
char *vlib_strerror();

int do_capture(int width, int height,unsigned nframes);
int setup_media_pipe(int mnode, int width, int height);
int video_config(vlib_config config,int width,int height);
int initialize_v4l2();
int set_v4l2_format();
int initialize_drm(vlib_config config,int width_out, int height_out);
int enable_streaming();


void vlib_log_str(vlib_log_level level, const char * str);

#ifdef __cplusplus
}
#endif

#endif /* VDF_LIB_H */
