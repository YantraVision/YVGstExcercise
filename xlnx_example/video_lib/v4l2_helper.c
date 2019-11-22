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
* @file v4l2_helper.c
*
* This file implements the helper functions for v4l2 video devices.
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

#include "v4l2_helper.h"
#include "mediactl_helper.h"


int v4l2_parse_node(const char *cardname, char *devnode)
{
	int fd,i=0,ret=0;
	struct v4l2_capability cap;
	char newdev[DEV_NAME_LEN];

	while(1) {
		snprintf (newdev, sizeof (newdev), "/dev/video%u", i);
		if ((fd=open(newdev,O_RDWR))<0)	{
			ret=-ENOENT;
			break;
		}
		if (ioctl(fd,VIDIOC_QUERYCAP,&cap)<0){
			ret=-errno;
			break;
		}
		if(cardname)
		{
			/* Compare card name */
			if (!strcmp((const char *) cap.card, cardname))	{
				strcpy (devnode, newdev);
				break;
			}
			/* Compare driver name */
			if (!strcmp((const char *) cap.driver, cardname))	{
				strcpy (devnode, newdev);
				break;
			}
		} else {
			/* Blindly follow the first one */
			strcpy (devnode, newdev);
			break;
		}
		i++;
		close(fd);
	}
	if (ret != -ENOENT)
		close(fd);
	return ret;
}
/**/
int v4l2_init(struct v4l2_dev *dev, unsigned int num_buffers)
{
	struct v4l2_capability caps;
	struct v4l2_format fmt;
	//struct v4l2_requestbuffers rqbufs; // damoder commented
	int ret;
	dev->fd = open(dev->devname, O_RDWR);
	if(dev->fd <0) {
		vlib_err("failed to open %s: %s\n", dev->devname, ERRSTR);
		return VLIB_ERROR_INTERNAL;
	} else {
		// TODO caps unused ?
		memset(&caps, 0, sizeof caps);
		ret = ioctl(dev->fd, VIDIOC_QUERYCAP, &caps);
		ASSERT(ret, "VIDIOC_QUERYCAP failed: %s\n", ERRSTR);
		/*Set the video device format */
		v4l2_get_format(dev, &fmt, &dev->format, &dev->format.pixelformat);
		// check if pixel format is supported
		if (fmt.fmt.pix.pixelformat != dev->format.pixelformat) {
			sprintf(vlib_errstr, "Requested pixel format '%.4s' is not supported by device '%s' which supports '%.4s'",
				(char *)&fmt.fmt.pix_mp.pixelformat, dev->devname, (char *)&dev->format.pixelformat);
		}
		// check if resolution is supported
		if (fmt.fmt.pix_mp.width != dev->format.width ||
			fmt.fmt.pix_mp.height != dev->format.height) {
			sprintf(vlib_errstr, "Requested resolution '%dx%d' is not supported by device '%s'",
				dev->format.width, dev->format.height, dev->devname);
			return VLIB_ERROR_CAPTURE;
		}
		// check if stride is supported
		if (fmt.fmt.pix_mp.plane_fmt[0].bytesperline != dev->format.plane_fmt[0].bytesperline) {
			sprintf(vlib_errstr, "Requested stride '%d' is not supported by device '%s'",
				dev->format.plane_fmt[0].bytesperline, dev->devname);
			return VLIB_ERROR_CAPTURE;
		}
		
		return VLIB_SUCCESS;
	}
}

/* Initialize video device format */
int v4l2_get_format(struct v4l2_dev *dev, struct v4l2_format *fmt, struct v4l2_pix_format_mplane *reqFmt, uint32_t *reqFourCC)
{
	int ret;

	fmt->type = dev->buf_type;
	ret = ioctl(dev->fd, VIDIOC_G_FMT, fmt);
	ASSERT(ret < 0, "VIDIOC_G_FMT failed: %s\n", ERRSTR);
	if((reqFmt != NULL) || (reqFourCC != NULL))
	{
		if(reqFmt != NULL)
		{
			fmt->fmt.pix_mp = *reqFmt;
			ret = ioctl(dev->fd, VIDIOC_S_FMT, fmt);
			if(ret < 0)
			{
				vlib_err("VIDIOC_S_FMT failed: %s New pixel-format not set\n", ERRSTR);
				/* Restore format */
				ret = ioctl(dev->fd, VIDIOC_G_FMT, fmt);
				reqFmt = NULL;
			} else {
				reqFourCC = NULL;
			}
		}
		if(reqFourCC != NULL)
		{
			fmt->fmt.pix_mp.pixelformat = *reqFourCC;
			ret = ioctl(dev->fd, VIDIOC_S_FMT, fmt);
			if(ret < 0)
			{
				vlib_err("VIDIOC_S_FMT failed: %s New pixel-format fourCC not set\n", ERRSTR);
			}
		}
		ret = ioctl(dev->fd, VIDIOC_G_FMT, fmt);
		ASSERT(ret < 0, "VIDIOC_G_FMT failed: %s\n", ERRSTR);
	}
	return VLIB_SUCCESS;
}

void v4l2_queue_buffer(struct v4l2_dev *dev, const struct buffer *buffer)
{
	struct v4l2_buffer buf;
	struct v4l2_plane planes[8];
	int i = 0;

	memset(&buf, 0, sizeof buf);
	memset(&planes, 0, sizeof planes);

	buf.type = dev->buf_type;
	buf.index = buffer->index;
	buf.memory = dev->mem_type;

	if ((dev->mem_type == V4L2_MEMORY_DMABUF) || (dev->mem_type == V4L2_MEMORY_USERPTR)) {
		if (buf.type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
			buf.m.planes = planes;
			buf.length = buffer->num_planes/*n_planes, 2 for NV12*/;
		}
	}

	if (dev->mem_type == V4L2_MEMORY_DMABUF) {
		/* V4L2 single-planar capture API? */
		if (buf.type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
			buf.m.fd = buffer->dmabuf_fd[0];
		/* V4L2 multi-planar capture API? */
		} else if (buf.type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
			buf.m.planes = planes;
			vlib_dbg("VIDIOC_QBUF num_planes = %u\n", dev->format.num_planes);
			buf.length = buffer->num_planes/*n_planes, 2 for NV12*/;
			memset(&planes, 0, sizeof planes);
			for (i = 0; i < buf.length/*n_planes*/; ++i) {
				buf.m.planes[i].m.fd = buffer->dmabuf_fd[i];
				buf.m.planes[i].data_offset = buffer->offsets[i];
				buf.m.planes[i].bytesused =
				buf.m.planes[i].length = buffer->lengths[i];
			}
		}
	} else if (dev->mem_type == V4L2_MEMORY_USERPTR) {
		if (buf.type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
			buf.m.planes = planes;
		
			buf.length = buffer->num_planes/*n_planes, 2 for NV12*/;
			memset(&planes, 0, sizeof planes);
			for (i = 0; i < buf.length/*n_planes*/; ++i) {
				buf.m.planes[i].m.userptr = (unsigned long)buffer->v4l2_buff[i];
				buf.m.planes[i].length = buffer->lengths[i];
			}
		}
	}

	if (dev->buf_type == V4L2_BUF_TYPE_VIDEO_OUTPUT)
		buf.bytesused = dev->format.plane_fmt[0].sizeimage;

	if(ioctl(dev->fd, VIDIOC_QBUF, &buf))
	{
		vlib_err("VIDIOC_QBUF(index = %d) failed: %s\n", buffer->index, ERRSTR);
	}
}

struct buffer *v4l2_dequeue_buffer(struct v4l2_dev *dev, struct buffer *buffers)
{
	struct v4l2_buffer buf;
	struct v4l2_plane planes[8];
	int ret;
	memset(&buf, 0, sizeof (buf));
	buf.type = dev->buf_type;
	buf.memory = dev->mem_type;

	if (buf.type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
		buf.m.planes = planes;
		buf.length = 8/*n_planes available*/;
		memset(&planes, 0, sizeof planes);
	}

	ret = ioctl(dev->fd, VIDIOC_DQBUF, &buf);
	if (ret == -EAGAIN) {
		vlib_err("ioctl(dev->fd, VIDIOC_DQBUF) == -EAGAIN\n");
		return NULL;
	}
	ASSERT(ret, "VIDIOC_DQBUF failed: %s\n", ERRSTR);
	return &buffers[buf.index];
}

/* turn off video device */
 int  v4l2_device_off(struct v4l2_dev *d)
{
	int ret;
	ret = ioctl(d->fd, VIDIOC_STREAMOFF, &d->buf_type);
	return ret;
}

/* turn on video device */
 int  v4l2_device_on(struct v4l2_dev *d)
{
	int ret;
	ret = ioctl(d->fd, VIDIOC_STREAMON, &d->buf_type);
	return ret;
}
