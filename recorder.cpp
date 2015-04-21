#include "recorder.h"
#include "mainwindow.h"

Recorder::Recorder(QWidget* mainWin, QObject *parent) :
    QObject(parent)
{
    this->mainWin = mainWin;
    this->loop = NULL;
}

void Recorder::start() {
    gboolean res;
    /* init GStreamer */
    gst_init (NULL, NULL);
//    this->loop = g_main_loop_new (NULL, FALSE);

    /* setup pipeline */
    this->pipeline = gst_pipeline_new ("pipeline");
    this->appsrc = gst_element_factory_make ("appsrc", "source");
    conv = gst_element_factory_make ("videoconvert", "conv");
    queue_before_mux = gst_element_factory_make ("queue", "queue_before_mux"); //???
    queue_before_enc = gst_element_factory_make ("queue", "queue_before_enc"); //???
    queue_before_file = gst_element_factory_make ("queue", "queue_before_file"); //???
    videoflip = gst_element_factory_make ("videoflip", "videoflip");
    videosink = gst_element_factory_make ("xvimagesink", "videosink");
    vp_enc = gst_element_factory_make ("vp8enc", "vp8enc");
//    vp_enc = gst_element_factory_make ("x264enc", "x264enc");
//    res = gst_preset_load_preset (GST_PRESET (video_enc), video_preset);

    webmmux = gst_element_factory_make ("webmmux", "webmmux");
//    webmmux = gst_element_factory_make ("mp4mux", "mp4mux");
    filesink = gst_element_factory_make ("filesink", "filesink");

    /* setup */
    g_object_set (G_OBJECT (this->appsrc), "caps",
                  gst_caps_new_simple ("video/x-raw",
                                       "format", G_TYPE_STRING, "RGBA",
                                       "width", G_TYPE_INT, this->window_width,
                                       "height", G_TYPE_INT, this->window_height,
                                       "framerate", GST_TYPE_FRACTION, 25, 1,
//                                       "framerate", GST_TYPE_FRACTION_RANGE, 0, 1, 30, 1,
                                       NULL), NULL);

    g_object_set(G_OBJECT(this->appsrc), "is-live", TRUE, NULL);
    g_object_set(G_OBJECT(this->appsrc), "block", TRUE, NULL);
    g_object_set(G_OBJECT(this->appsrc), "do-timestamp", TRUE, NULL);

    g_object_set(G_OBJECT(this->vp_enc), "threads", 3, NULL);
    g_object_set(G_OBJECT(this->vp_enc), "target-bitrate", 256000*8, NULL);
    g_object_set(G_OBJECT(this->vp_enc), "deadline", 1, NULL);
    g_object_set(G_OBJECT(this->vp_enc), "cpu-used", 15, NULL);

    g_object_set(G_OBJECT(this->videoflip), "method", 5, NULL);

//    g_object_set(G_OBJECT(this->vp_enc), "keyframe-max-dist", 20, NULL);

//    g_object_set(G_OBJECT(this->vp_enc), "static-threshold", 1000, NULL);
//    g_object_set(G_OBJECT(this->vp_enc), "token-partitions", 2, NULL);
//    g_object_set(G_OBJECT(this->vp_enc), "max-quantizer", 30, NULL);
//    g_object_set(G_OBJECT(this->vp_enc), "cq-level", 60, NULL);

    g_object_set(G_OBJECT (filesink), "location", "newfile.webm", NULL);
//    g_object_set(G_OBJECT (filesink), "location", "newfile.mp4", NULL);

    gst_bin_add_many (GST_BIN (pipeline), this->appsrc, conv, this->videoflip, queue_before_enc, vp_enc, queue_before_mux, webmmux, queue_before_file, filesink, NULL);
    gst_element_link_many (this->appsrc, conv, this->videoflip, queue_before_enc, vp_enc, queue_before_mux, webmmux, queue_before_file, filesink, NULL);
//    gst_bin_add_many (GST_BIN (pipeline), this->appsrc, conv, vp_enc, webmmux, filesink, NULL);
//    gst_element_link_many (this->appsrc, conv, vp_enc, webmmux, filesink, NULL);
//    gst_bin_add_many (GST_BIN (pipeline), this->appsrc, conv, webmmux, filesink, NULL);
//    gst_element_link_many (this->appsrc, conv, webmmux, filesink, NULL);
//    gst_bin_add_many (GST_BIN (pipeline), this->appsrc, conv, vp_enc, videosink, NULL);
//    gst_element_link_many (this->appsrc, conv, vp_enc, videosink, NULL);

    /* setup appsrc */
    g_object_set (G_OBJECT (this->appsrc),
                  "stream-type", 0,
                  "format", GST_FORMAT_TIME, NULL);
    g_signal_connect (this->appsrc, "need-data", G_CALLBACK (cbNeedData), this);
    //    g_signal_connect (this->appsrc, "need-data", G_CALLBACK (cbNeedData_), NULL);

    /* play */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    //fixme don't know if needed
    //    g_main_loop_run (this->loop);
}

void Recorder::stop() {
    qDebug() << "stop fired";
    /* clean up */
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_main_loop_unref (this->loop);
}

QWidget* Recorder::getMainWindowLink() {
    return this->mainWin;
}

void Recorder::cbNeedData(GstElement *appsrc_,
                          guint       unused_size,
                          Recorder  * user_data)
{
//    static gboolean white = FALSE;
//    static GstClockTime timestamp = 0;
    static long num_frame = 0;
    guint  size;
    GstFlowReturn ret;

    GstBuffer *buffer;

//    QByteArray retVal;
//    QMetaObject::invokeMethod(user_data->getMainWindowLink(), "getFrame", Qt::DirectConnection, Q_RETURN_ARG(QByteArray, retVal));

    size = 1280 * 480 * 4;

    buffer = gst_buffer_new_allocate (NULL, size, NULL);

    (qobject_cast<MainWindow*>(user_data->getMainWindowLink()))->gstBufferFill(buffer, size);

//    gst_buffer_fill (buffer, 0, retVal.data(), size);

//    GST_BUFFER_PTS (buffer) = timestamp;
//    GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 30);

//    timestamp += GST_BUFFER_DURATION (buffer);
//    GST_BUFFER_TIMESTAMP(buffer) = (GstClockTime)((num_frame / 30.0) * 1e9);

    g_signal_emit_by_name (appsrc_, "push-buffer", buffer, &ret);

//    num_frame +=1;
//    if (num_frame >= 30*5) {
//        gst_element_set_state (user_data->pipeline, GST_STATE_NULL);
//        gst_object_unref (GST_OBJECT (user_data->pipeline));
////        g_main_loop_unref (this->loop);
//    }

    gst_buffer_unref (buffer);
    if (ret != GST_FLOW_OK) {
//        /* something wrong, stop pushing */
//        //        g_main_loop_quit (loop);
        qDebug() << "fuck";
    }
}

