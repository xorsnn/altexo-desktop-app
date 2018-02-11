
#include <gst/gst.h>
// #include <gst/gstbuffer.h>
#include <gst/app/gstappsrc.h>

#include <stdint.h>

int want = 1;

uint16_t b_white[385 * 288];
uint16_t b_black[385 * 288];

static void prepare_buffer(GstAppSrc *appsrc) {

  static gboolean white = FALSE;
  static GstClockTime timestamp = 0;
  GstBuffer *buffer;
  guint size;
  GstFlowReturn ret;

  if (!want)
    return;
  want = 0;

  size = 385 * 288 * 2;

  buffer = gst_buffer_new_wrapped_full(GST_MEMORY_FLAG_READONLY,
                                       (gpointer)(white ? b_white : b_black),
                                       size, 0, size, NULL, NULL);

  white = !white;

  GST_BUFFER_PTS(buffer) = timestamp;
  GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_SECOND, 4);

  timestamp += GST_BUFFER_DURATION(buffer);

  ret = gst_app_src_push_buffer(appsrc, buffer);

  if (ret != GST_FLOW_OK) {
    /* something wrong, stop pushing */
    // g_main_loop_quit (loop);
  }
}

static void cb_need_data(GstElement *appsrc, guint unused_size,
                         gpointer user_data) {
  // prepare_buffer((GstAppSrc*)appsrc);
  want = 1;
}

gint main(gint argc, gchar *argv[]) {

  GstElement *pipeline, *appsrc, *conv, *videosink, *videoEnc, *videorate;

  for (int i = 0; i < 385 * 288; i++) {
    b_black[i] = 0;
    b_white[i] = 0xFFFF;
  }

  /* init GStreamer */
  gst_init(&argc, &argv);

  /* setup pipeline */
  pipeline = gst_pipeline_new("pipeline");
  appsrc = gst_element_factory_make("appsrc", "source");
  conv = gst_element_factory_make("videoconvert", "conv");
  // videosink = gst_element_factory_make("xvimagesink", "videosink");
  videosink = gst_element_factory_make("filesink", "videosink");
  videoEnc = gst_element_factory_make("x264enc", "enc");
  videorate = gst_element_factory_make("videorate", "videorate");

  /* setup */
  g_object_set(G_OBJECT(appsrc), "caps",
               gst_caps_new_simple("video/x-raw", "format", G_TYPE_STRING,
                                   "RGB16", "width", G_TYPE_INT, 384, "height",
                                   G_TYPE_INT, 288, "framerate",
                                   GST_TYPE_FRACTION, 0, 1, NULL),
               NULL);

  g_object_set(G_OBJECT(videosink), "location", "test.mp4", NULL);
  //  x264enc quantizer=0 pass=5 speed-preset=1
  g_object_set(G_OBJECT(videoEnc), "quantizer", 0, "pass", 5, "speed-preset", 1,
               NULL);

  gst_bin_add_many(GST_BIN(pipeline), appsrc, videorate, conv, videoEnc,
                   videosink, NULL);
  gst_element_link_many(appsrc, videorate, conv, videoEnc, videosink, NULL);

  /* setup appsrc */
  g_object_set(G_OBJECT(appsrc), "stream-type", 0, // GST_APP_STREAM_TYPE_STREAM
               "format", GST_FORMAT_TIME, "is-live", TRUE, "do-timestamp", TRUE,
               NULL);
  g_signal_connect(appsrc, "need-data", G_CALLBACK(cb_need_data), NULL);

  /* play */
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  while (1) {
    prepare_buffer((GstAppSrc *)appsrc);
    g_main_context_iteration(g_main_context_default(), FALSE);
  }

  /* clean up */
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(GST_OBJECT(pipeline));

  return 0;
}
