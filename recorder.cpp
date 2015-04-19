#include "recorder.h"

Recorder::Recorder(QWidget* mainWin, QObject *parent) :
    QObject(parent)
{
    this->mainWin = mainWin;
    this->loop = NULL;
}

void Recorder::start() {
    /* init GStreamer */
    gst_init (NULL, NULL);
    this->loop = g_main_loop_new (NULL, FALSE);

    /* setup pipeline */
    pipeline = gst_pipeline_new ("pipeline");
    this->appsrc = gst_element_factory_make ("appsrc", "source");
    conv = gst_element_factory_make ("videoconvert", "conv");
    //    videosink = gst_element_factory_make ("xvimagesink", "videosink");
    vp_enc = gst_element_factory_make ("vp8enc", "vp8enc");
    webmmux = gst_element_factory_make ("webmmux", "webmmux");
    filesink = gst_element_factory_make ("filesink", "filesink");

    /* setup */
    g_object_set (G_OBJECT (this->appsrc), "caps",
                  gst_caps_new_simple ("video/x-raw",
                                       "format", G_TYPE_STRING, "RGBA",
                                       "width", G_TYPE_INT, this->window_width,
                                       "height", G_TYPE_INT, this->window_height,
                                       "framerate", GST_TYPE_FRACTION, 25, 1,
                                       NULL), NULL);

    g_object_set(G_OBJECT(this->appsrc), "is-live", TRUE, NULL);
    g_object_set(G_OBJECT(this->appsrc), "block", FALSE, NULL);
    g_object_set(G_OBJECT(this->appsrc), "do-timestamp", TRUE, NULL);
    g_object_set(G_OBJECT (filesink), "location", "newfile.webm", NULL);

    gst_bin_add_many (GST_BIN (pipeline), this->appsrc, conv, vp_enc, webmmux, filesink, NULL);
    gst_element_link_many (this->appsrc, conv, vp_enc, webmmux, filesink, NULL);

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
    static gboolean white = FALSE;
    static GstClockTime timestamp = 0;

    guint size;
    GstFlowReturn ret;

    GstBuffer *buffer;



    QByteArray retVal;
    QMetaObject::invokeMethod(user_data->getMainWindowLink(), "getFrame", Qt::DirectConnection, Q_RETURN_ARG(QByteArray, retVal));

    size = 1280 * 480 * 4;


    buffer = gst_buffer_new_allocate (NULL, size, NULL);

    gst_buffer_fill (buffer, 0, retVal.data(), size);

    g_signal_emit_by_name (appsrc_, "push-buffer", buffer, &ret);

    if (ret != GST_FLOW_OK) {
        /* something wrong, stop pushing */
        //        g_main_loop_quit (loop);
        qDebug() << "fuck";
    }
}

