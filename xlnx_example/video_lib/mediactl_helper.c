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
* @file mediactl_helper.c
*
* This file implements the  helper functions for media library.
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


#include "common.h"
#include "mediactl_helper.h"
#include "video.h"

#define MEDIA_G_DVTIMINGS_RETRY_CNT 10
#define MEDIA_G_DVTIMINGS_RETRY_DLY_USEC 100000
#define MEDIA_FMT "\"%s\":%d [fmt:%s/%dx%d field:none]"

void media_set_fmt_str(char *set_fmt, const char *entity, unsigned int pad,
                       const char *fmt, unsigned int width, unsigned int height)
{
        sprintf(set_fmt, MEDIA_FMT, entity, pad, fmt, width, height);
}



/*Print media device details */
void print_media_info(const struct media_device_info *info)
{
	vlib_info("Media controller API version %u.%u.%u\n\n",
						   (info->media_version << 16) & 0xff,
						   (info->media_version << 8) & 0xff,
						   (info->media_version << 0) & 0xff);
	vlib_info("Media device information\n"
						   "------------------------\n"
						   "driver			%s\n"
						   "model			%s\n"
						   "serial			%s\n"
						   "bus info		%s\n"
						   "hw revision 	0x%x\n"
						   "driver version	%u.%u.%u\n\n",
						   info->driver, info->model,
						   info->serial, info->bus_info,
						   info->hw_revision,
						   (info->driver_version << 16) & 0xff,
						   (info->driver_version << 8) & 0xff,
						   (info->driver_version << 0) & 0xff);
}

int set_media_control(struct video_pipeline *s, media_node node)
{
	struct media_device *media;
	struct v4l2_dv_timings timings;
	struct media_pad *pad;
	char media_devname[DEV_NAME_LEN];
	char media_formats[100];
	int ret = -1;
	char fmt_str[100];
	
	/* TODO: obtain media node based on model instead of number */
	sprintf(media_devname, "/dev/media%d", node);

	/* Set media device node */
	media = media_device_new(media_devname);
	ASSERT(!media, "Failed to create media device %s\n", media_devname);

	/* Enumerate entities, pads and links */
	if((ret = media_device_enumerate(media)) != VLIB_SUCCESS)
	{
		vlib_err("Failed to enumerate %s\n", media_devname);
		media_device_unref(media);
		return ret;
	}
#ifdef INFO_MODE
	const struct media_device_info *info = media_get_info(media);
	print_media_info(info);
#endif

	memset(media_formats, 0, sizeof (media_formats));

	switch (node) {
	 case MEDIA_NODE_0:
                        /* Set TPG input resolution */
                        sprintf(media_formats, MEDIA_TPG_FMT, 0, s->w, s->h);
                       // sprintf(media_formats, MEDIA_TPG_FMT, 0, 1280, 720);
                        printf("Setting %s\n", media_formats);
                        ret = v4l2_subdev_parse_setup_formats(media, media_formats);
                        ASSERT(ret, "Unable to setup formats: %s (%d)\n", strerror(-ret), -ret);

                        break;

		/* VCAP HDMI */
		case MEDIA_NODE_1:
			/* Skip initialization if FMC is not present */
			if (!s->hdmii_present)
				break;

#if 1
			/* Get Xilinx HDMI RX pad */
			pad = media_parse_pad(media, MEDIA_HDMI_PAD, NULL);
			ASSERT(pad == NULL, "Pad '%s' not found\n", MEDIA_HDMI_PAD);
			ret = v4l2_subdev_query_dv_timings(pad->entity, &timings);
			//printf("v4l2_subdev_query_dv_timings() = %d\n", ret);
			/* setting the scaler format */
			memset(fmt_str, 0, sizeof(fmt_str));
        	        media_set_fmt_str(fmt_str, MEDIA_SCALER_ENTITY, 0, MEDIA_SCALER_FMT_IN,
	                                  timings.bt.width,timings.bt.height);
                	ret = v4l2_subdev_parse_setup_formats(media, fmt_str);
		 	if(ret)
			printf("Unable to set the format, for SINK pad \n");
        	        memset(fmt_str, 0, sizeof(fmt_str));
			//printf("before media_set_fmt_str, for SRC pad\n");
                	media_set_fmt_str(fmt_str, MEDIA_SCALER_ENTITY, 1,
                                  MEDIA_SCALER_FMT_OUT,
                                  s->w,s->h);
              		  ret = v4l2_subdev_parse_setup_formats(media, fmt_str);
			//if(ret)
                        //printf("Unable to set the format, for SRC pad \n");
#else
			/* Get Xilinx HDMI RX pad */
                        pad = media_parse_pad(media, MEDIA_HDMI_PAD, NULL);
                        ASSERT(pad == NULL, "Pad '%s' not found\n", MEDIA_HDMI_PAD);
                        ret = v4l2_subdev_query_dv_timings(pad->entity, &timings);
                        printf("v4l2_subdev_query_dv_timings() = %d\n", ret);


                        memset(fmt_str, 0, sizeof(fmt_str));
                        printf("before media_set_fmt_str, for SRC pad\n");
                        media_set_fmt_str(fmt_str, MEDIA_SCALER_ENTITY, 1,
                                  MEDIA_SCALER_FMT_OUT,
                                  s->w,s->h);
                          ret = v4l2_subdev_parse_setup_formats(media, fmt_str);
                        if(ret)
                        printf("Unable to set the format, for SRC pad \n");


			 /* setting the scaler format */
			printf("set format for SINK pad........................\n");
                        memset(fmt_str, 0, sizeof(fmt_str));
                        media_set_fmt_str(fmt_str, MEDIA_SCALER_ENTITY, 0, MEDIA_SCALER_FMT_IN,
                                          timings.bt.width,timings.bt.height);
                        ret = v4l2_subdev_parse_setup_formats(media, fmt_str);
                        if(ret)
                        printf("Unable to set the format, for SINK pad \n");
#endif
			/* DEFAULT */
			default:
				break;
		}

	if (media)
		media_device_unref(media);

	return ret;
}

/* Helper function to search for an entity with a name equal to @name*/
int get_entity_state(media_node node, const char *name)
{
	struct media_entity *entity;
	struct media_device *media;
	int status = 0, ret = 0;
	char dev_name[DEV_NAME_LEN];

	//printf("name: %s\n", name);

	/* Set media device node */
	sprintf(dev_name, "/dev/media%d", node);
	printf("device name: %s\n", dev_name);
	media = media_device_new(dev_name);
	ASSERT(!media, "Failed to create media device device\n");

	/* Enumerate entities, pads and links */
	ret = media_device_enumerate(media);
	if(ret < 0)
	{
		printf("Failed to enumerate %s (%d)\n", dev_name, ret);
		return 0;
	}


	/* Check if entity is present */
	//entity = media_get_entity_by_name(media, name, strlen(name));
	entity = media_get_entity_by_name(media, name);
	if (entity)
		status = 1;

	if (media)
		media_device_unref(media);

	//printf("status: %d\n", status);

	return status;
}

/* Helper function that returns the full path and name to the device node
 * corresponding to the given entity i.e (/dev/v4l-subdev* )  */
int get_entity_devname(media_node node, char *name, char *subdev_name)
{
	struct media_device *media;
	struct media_entity *entity;
	const char *entity_node_name;
	char dev_name[DEV_NAME_LEN];
	int ret;

	/* Set media device node */
	sprintf(dev_name, "/dev/media%d", node);
	media = media_device_new(dev_name);
	ASSERT(!media, "Failed to create media device\n");

	/* Enumerate entities, pads and links */
	if((ret = media_device_enumerate(media)) >= 0)
	{
		ret = VLIB_ERROR_INTERNAL;
		//entity = media_get_entity_by_name(media, name, strlen(name));
		entity = media_get_entity_by_name(media, name);
		if(entity)
		{
			entity_node_name = media_entity_get_devname(entity);
			if(entity_node_name)
			{
				strcpy(subdev_name, entity_node_name);
				ret = VLIB_SUCCESS;
			} else {
				printf("Entity '%s' has no device node name\n", name);
			}
		} else {
			printf("Entity '%s' not found\n", name);
		}

	} else {
		printf("Failed to enumerate '%s'\n", dev_name);
		ret = VLIB_ERROR_INTERNAL;
	}
	media_device_unref(media);
	return ret;
}

/* Helper function that retrieve the detected digital video timings for the currently
     selected input of @entity_name and store them in the @timings structure */
int  query_entity_dv_timings(media_node node ,char *entity_name, struct v4l2_dv_timings *timings)
{
	struct media_device *media;
	struct media_pad *pad;
	int ret = 0;
	char dev_name[DEV_NAME_LEN];

	/* Set media device node */
	sprintf(dev_name, "/dev/media%d", node);
	media = media_device_new(dev_name);
	ASSERT(!media, "Failed to create media device device\n");

	/* Enumerate entities, pads and links */
	ret = media_device_enumerate(media);
	ASSERT(ret < 0, "Failed to enumerate %s (%d)\n", dev_name, ret);

	pad = media_parse_pad(media, entity_name, NULL);
	ASSERT(!pad ,"Pad '%s' not found\n", entity_name);
	ret = v4l2_subdev_query_dv_timings(pad->entity, timings);

	if (media)
		media_device_unref(media);
	vlib_dbg("v4l2_subdev_query_dv_timings = %d\n", ret);
	return ret;
}

/* Helper function that retrieve the detected mediua bus format for the currently
     selected input of @entity_name and store them in the @timings structure */
int  query_entity_active_format(media_node node ,char *entity_name, struct v4l2_mbus_framefmt *format)
{
	struct media_device *media;
	struct media_pad *pad;
	int ret = 0;
	char dev_name[DEV_NAME_LEN];

	/* Set media device node */
	sprintf(dev_name, "/dev/media%d", node);
	media = media_device_new(dev_name);
	ASSERT(!media, "Failed to create media device device\n");

	/* Enumerate entities, pads and links */
	ret = media_device_enumerate(media);
	ASSERT(ret < 0, "Failed to enumerate %s (%d)\n", dev_name, ret);

	pad = media_parse_pad(media, entity_name, NULL);
	ASSERT(!pad ,"Pad '%s' not found\n", entity_name);

	ret = v4l2_subdev_get_format(pad->entity, format, 0, V4L2_SUBDEV_FORMAT_ACTIVE);

	if (media)
		media_device_unref(media);
	vlib_dbg("v4l2_subdev_get_format = %d\n", ret);
	return ret;
}
