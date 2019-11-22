#source "$dir_build/settings.sh"
#VIDEO_LIB_PATH = ../

PLNX_AARCH_PATH = /media/workspace1/savan/november/18thNov/xilinx-vcu-trd-zcu106-v2019.1-final/images/linux/sdk/sysroots/aarch64-xilinx-linux

#include $(VIDEO_LIB_PATH)/video_lib/video_lib.mk

WARNING_CFLAGS =  -Wall -Wdeclaration-after-statement -Wvla -Wpointer-arith -Wmissing-declarations -Wredundant-decls -Wundef -Wwrite-strings -Wformat-nonliteral -Wformat-security -Winit-self -Wmissing-include-dirs -Waddress -Waggregate-return -Wno-multichar -Wnested-externs -O2 -Wall -fpic -Wdiscarded-qualifiers -Wunused-variable

GST_OPTION_CFLAGS = $(WARNING_CFLAGS)  $(DEBUG_CFLAGS)
GLIB_EXTRA_CFLAGS  =  -DG_THREADS_MANDATORY -DG_DISABLE_CAST_CHECKS

GST_BASE_CFLAGS = -I$(PLNX_AARCH_PATH)/usr/include/gstreamer-1.0 -I$(PLNX_AARCH_PATH)/usr/lib/gstreamer-1.0/include -I$(PLNX_AARCH_PATH)/usr/include/glib-2.0 -I$(PLNX_AARCH_PATH)/usr/lib/glib-2.0/include -pthread

GST_CFLAGS = -I$(PLNX_AARCH_PATH)/usr/include/glib-2.0 -I$(PLNX_AARCH_PATH)/usr/lib/glib-2.0/include -pthread -I$(PLNX_AARCH_PATH)/usr/include/gstreamer-1.0 -I$(PLNX_AARCH_PATH)/usr/lib/gstreamer-1.0/include -I$(PLNX_AARCH_PATH)/usr/include/glib-2.0 -I$(PLNX_AARCH_PATH)/usr/lib/glib-2.0/include $(GLIB_EXTRA_CFLAGS) $(GST_OPTION_CFLAGS) -pthread

CC = aarch64-linux-gnu-gcc --sysroot=$(PLNX_AARCH_PATH)

export PKG_CONFIG_PATH=$(PLNX_AARCH_PATH)/usr/lib/pkgconfig/
LDFLAGS += `pkg-config --libs glib-2.0 gstreamer-1.0`
LDFLAGS += -lpthread -ldl -lrt $(VIDEOLIB_LDFLAGS) -lmediactl -lv4l2subdev  -Wl,-rpath-link, -Wl,-O1 -Wl,--hash-style=gnu -Wl,--allow-multiple-definition
LDFLAGS += -lgstvideo-1.0
#LDFLAGS += -lpthread -ldrm -ldl -lrt $(VIDEOLIB_LDFLAGS) -lmediactl -lv4l2subdev  -Wl,-rpath-link

#SOURCES = $(wildcard *.c) $(VIDEOLIB_SRCS)
SOURCES = $(wildcard *.c) 
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

#@echo $(MEDIACTL_CFLAGS)

CFLAGS = $(GST_BASE_CFLAGS) $(GST_CFLAGS) $(MEDIACTL_CFLAGS) $(VIDEOLIB_CFLAGS) -DPLATFORM_ZCU106

all: gst_yv_app

gst_yv_app: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(wildcard *.o) gst_yv_app 
	

#clean:
#	rm -f $(wildcard *.o) lab2a_capture_encode_mux_filedump
#	rm -f $(VIDEO_LIB_PATH)/video_lib/*.o



