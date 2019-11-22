/*
 * Copyright 2008 Tungsten Graphics
 *   Jakob Bornecrantz <jakob@tungstengraphics.com>
 * Copyright 2008 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef UTIL_FORMAT_H
#define UTIL_FORMAT_H

#include <drm_fourcc.h>

#ifndef DRM_FORMAT_AVUY
#define DRM_FORMAT_AVUY         fourcc_code('A', 'V', 'U', 'Y') /* [31:0] A:Cr:Cb:Y 8:8:8:8 little endian */
#endif

#ifndef DRM_FORMAT_VUY888
#define DRM_FORMAT_VUY888       fourcc_code('V', 'U', '2', '4') /* [23:0] Cr:Cb:Y little endian */
#endif

#ifndef DRM_FORMAT_XVUY8888
#define DRM_FORMAT_XVUY8888     fourcc_code('X', 'V', '2', '4') /* [31:0] x:Cr:Cb:Y 8:8:8:8 little endian */
#endif

#ifndef DRM_FORMAT_XVUY2101010
#define DRM_FORMAT_XVUY2101010  fourcc_code('X', 'V', '3', '0') /* [31:0] x:Cr:Cb:Y 2:10:10:10 little endian */
#endif

#ifndef DRM_FORMAT_Y8
#define DRM_FORMAT_Y8           fourcc_code('G', 'R', 'E', 'Y') /* 8  Greyscale */
#endif

#ifndef DRM_FORMAT_Y10
#define DRM_FORMAT_Y10          fourcc_code('Y', '1', '0', ' ') /* 10 Greyscale */
#endif

#ifndef DRM_FORMAT_XV15
#define DRM_FORMAT_XV15         fourcc_code('X', 'V', '1', '5') /* 2x2 subsampled Cb:Cr plane 2:10:10:10 */
#endif

#ifndef DRM_FORMAT_XV20
#define DRM_FORMAT_XV20         fourcc_code('X', 'V', '2', '0') /* 2x1 subsampled Cb:Cr plane 2:10:10:10 */
#endif

struct util_color_component {
	unsigned int length;
	unsigned int offset;
};

struct util_rgb_info {
	struct util_color_component red;
	struct util_color_component green;
	struct util_color_component blue;
	struct util_color_component alpha;
};

enum util_yuv_order {
	YUV_YCbCr = 1,
	YUV_YCrCb = 2,
	YUV_YC = 4,
	YUV_CY = 8,
};

struct util_yuv_info {
	enum util_yuv_order order;
	unsigned int xsub;
	unsigned int ysub;
	unsigned int chroma_stride;
};

struct util_format_info {
	uint32_t format;
	const char *name;
	const struct util_rgb_info rgb;
	const struct util_yuv_info yuv;
};

uint32_t util_format_fourcc(const char *name);
const struct util_format_info *util_format_info_find(uint32_t format);

#endif /* UTIL_FORMAT_H */
