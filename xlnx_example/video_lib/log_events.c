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
* @file log_events.c
*
* This file implements helper functions for logging video pipeline events.
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a RSP   05/06/15 Initial release
* </pre>
*
* @note
*
******************************************************************************/

#include "common.h"
#include "video.h"
#include "log_events.h"

int init_pipeline_events(void *ptr)
{

	struct video_pipeline *v_pipe = (struct video_pipeline *) ptr;
	memset(v_pipe->events, 0, sizeof(v_pipe->events));
	v_pipe->events[CAPTURE].name = "Capture";
	v_pipe->events[DISPLAY].name = "Display";
	v_pipe->events[PROCESS_IN].name = "Filter-In";
	v_pipe->events[PROCESS_OUT].name = "Filter-Out";

	return VLIB_SUCCESS;

}

int set_pipeline_events_cnt(void *ptr , int count)
{

	struct video_pipeline *v_pipe = (struct video_pipeline *) ptr;
	v_pipe->active_events = count;

	return VLIB_SUCCESS;
}

void *capture_pipeline_events(void *ptr)
{

	int i=0;
	struct video_pipeline *v_pipe = (struct video_pipeline *) ptr;

	while (1) {
		vlib_dbg("-------------\n");
		pthread_testcancel();
		for ( i = 0 ; i < v_pipe->active_events; i++) {
			v_pipe->events[i].sampled_val = v_pipe->events[i].counter_val;
			v_pipe->events[i].counter_val = 0;
			vlib_dbg("%s :: %d \n", v_pipe->events[i].name,v_pipe->events[i].sampled_val);
		}
		sleep(1);
	}
}

int reset_log_event_counters(void *ptr)
{
	int i;
	struct video_pipeline *v_pipe = (struct video_pipeline *) ptr;
	/*Reset sample counters on exit */
	for ( i = 0 ; i < v_pipe->active_events; i++) {
		v_pipe->events[i].sampled_val = 0;
	}

	return VLIB_SUCCESS;
}
