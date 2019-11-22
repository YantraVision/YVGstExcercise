/*--------------------------               LAB2(a)           -----------------------------------------------------
* This is the main application to demonstrate video coding ( encoding ).
* In this application, HDMI source is used to capture the video.
* The captured video is encoded by VCU encoder (omxh264enc), and muxed the compressed data into output mp4 file. 
* It implements the following pipeline.
	gst-launch-1.0 v4l2src device=/dev/video0 io-mode=4 num-buffers=600 \
					! video/x-raw, format=NV12, width=1920, height=1080, framerate=60/1 \
					! omxh264enc target-bitrate=20000 gop-length=60 b-frames=0 control-rate=2 \
					! h264parse \
					! qtmux \
					! filesink location=test_h264.m4
*------------------------------------------------------------------------------------------------------------*/

/* Include Header Files */
#include <gst/gst.h>
#include "video.h"
#include "v4l2_helper.h"

/* Macros, Constants */
#define MENU_NUMERIC_BASE 1000

int main (int argc, char *argv[])
{
	/* Gstreamer related variables */
	GstElement *bin, *hdmivideosrc, *hdmisrc_capsfilter, *encoder,  *parser, *muxer, *filesink;
	GstCaps *hdmisrc_caps;
	gboolean done = FALSE;
	GstBus *bus;

	/* V4L2 input resolution */
	int width=1920,height=1080, ret;

	/* Video input source Configuration */
	vlib_config config = {
	//.src = VIDEO_SRC_TPG,
	.src = VIDEO_SRC_HDMII,
	};

	if(argc !=2 )
	{
		printf("Invalid arguments:\n\n\tUsage: Executable file_path\n\n");
		return -1;
	}

	/* Configure the media pipeline with apropriate resolutions * 
	* and input/output formats at each media entity            */	
	ret = setup_media_pipe(config.src,width,height);
	if(ret != 0)
	{
		printf("setup_media_pipe failed, ret: %d\n", ret);
		return -1;
	}
	printf("Configured the media pipeline...\n");

	/* Set the NV12 format to v4l2 device node */
	ret = set_v4l2_format();	
	if(ret != 0)
	{
		printf("set_v4l2_format failed, ret: %d\n", ret);
		return -1;
	}
	printf("Set the NV12 format...\n");
	
	/* Initialize Gstreamer library. It'll also parse the GStreamer-specific command line options */
	gst_init (&argc, &argv);
	
	printf("Gstreamer initialization done...\n");	

	/* Create a new bin to hold the elements */
	bin = gst_pipeline_new ("pipeline");
	g_assert (bin);

	//---------------------------------------------------------------------------------------------
	/* Create a video capture (HDMI source) element */
	hdmivideosrc = gst_element_factory_make ("v4l2src", "hdmivideosrc");
	g_assert (hdmivideosrc);
	/* Set the parameters of video capture element */
	g_object_set(G_OBJECT(hdmivideosrc), "device", "/dev/video0", NULL); 
	g_object_set(G_OBJECT(hdmivideosrc), "io-mode", 4, NULL);
	g_object_set(G_OBJECT(hdmivideosrc), "num-buffers", 600, NULL);
	//---------------------------------------------------------------------------------------------

	/* Create caps filter for capture element */
	hdmisrc_capsfilter = gst_element_factory_make("capsfilter", "hdmisrc_caps");
	hdmisrc_caps = gst_caps_from_string("video/x-raw, format=NV12, width=1920, height=1080, framerate=60/1");
	g_object_set (hdmisrc_capsfilter, "caps", hdmisrc_caps, NULL);

	//---------------------------------------------------------------------------------------------
	/* Create VCU encoder (omxh264enc) element */
	encoder = gst_element_factory_make ("omxh264enc", "omxh264enc");
	g_assert (encoder);
	/* Set the parameters (target-bitrate=20000, gop-length=60, 
	                       b-frames=0, control-rate=2) of video encoder element */
	g_object_set(G_OBJECT(encoder), "target-bitrate", 20000, NULL); 
	g_object_set(G_OBJECT(encoder), "gop-length", 60, NULL); 
	g_object_set(G_OBJECT(encoder), "b-frames", 0, NULL); 
	g_object_set(G_OBJECT(encoder), "control-rate", 2, NULL); 
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	/* Create parser (h264parse) element */
	parser = gst_element_factory_make ("h264parse", "h264parse");
	g_assert (parser);
	//---------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------
	/* Create mp4 muxer (qtmux) element */
	muxer = gst_element_factory_make ("qtmux", "Mp4Muxer");
	g_assert (muxer);
	//---------------------------------------------------------------------------------------------	

	//---------------------------------------------------------------------------------------------
	/* Create a file sink (filesink) element */
	filesink = gst_element_factory_make ("filesink", "filesink");
	g_assert (filesink);
	/* Set the parameters of file sink element */
	//g_object_set(G_OBJECT(filesink), "location", "test_h264_1min.mp4", NULL);
	g_object_set(G_OBJECT(filesink), "location", argv[1], NULL);
	//---------------------------------------------------------------------------------------------

	/* Add elemenets to pipeline. It accepts a list of elements to be added, ending with NULL */
	gst_bin_add_many (GST_BIN (bin), hdmivideosrc, hdmisrc_capsfilter, encoder, parser, muxer, filesink, NULL);

	/* Link the elements */
	gst_element_link_many (hdmivideosrc, hdmisrc_capsfilter, encoder, parser, muxer, filesink, NULL);

	g_print ("Starting pipeline...\n");

	/* Change the state of pipeline to play. The data processing starts in this state. */
	gst_element_set_state (bin, GST_STATE_PLAYING);

	/* Get the bus of pipeline which informs the application of any special events that occur in the pipeline */
	bus = gst_pipeline_get_bus(GST_PIPELINE(bin));

	/* while loop to query the messages like EOS (end of stream) or any error, from bus */	
	while (!done) {
		GstMessage *message;

		/* Get a message from the bus, waiting up to the specified timeout */
		message = gst_bus_timed_pop (bus, -1);

		switch (GST_MESSAGE_TYPE (message)) {
		  case GST_MESSAGE_ERROR:
			g_print ("Whoa! some error occured\n");
			done = TRUE;
			break;
		  case GST_MESSAGE_EOS:
			g_print ("got EOS!\n");
			done = TRUE;
			break;
		  default:		
			break;
		}
		gst_message_unref (message);
	}

	g_print ("Stopping pipeline...\n");

	/* Change the state of pipeline to NULL */
	gst_element_set_state (bin, GST_STATE_NULL);

	/* Unreffing the bin will clean up all its children too */
	gst_object_unref (bin);

	return 0;
}
