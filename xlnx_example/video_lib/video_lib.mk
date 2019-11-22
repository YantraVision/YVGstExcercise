#VIDEO_LIB_PATH = /scratch/project/tsc_2018/final/Labs
#LDFLAGS_PKG_CONFIG = `pkg-config --libs libdrm`

VIDEOLIB_CFLAGS = -I$(LDFLAGS_PKG_CONFIG)/ -I$(VIDEO_LIB_PATH)/video_lib/
VIDEOLIB_LDFLAGS = $(LDFLAGS_PKG_CONFIG)

VIDEOLIB_SRCS = $(VIDEO_LIB_PATH)/video_lib/log_events.c \
	$(VIDEO_LIB_PATH)/video_lib/mediactl_helper.c \
	$(VIDEO_LIB_PATH)/video_lib/v4l2_helper.c \
	$(VIDEO_LIB_PATH)/video_lib/video.c
