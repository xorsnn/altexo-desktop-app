#include "recorder.h"
#include "mainwindow.h"
#include <QApplication>

Recorder::Recorder(QWidget* mainWin, QObject *parent) :
    QObject(parent)
{
    this->mainWin = mainWin;
    this->loop = NULL;
}

Recorder::~Recorder() {
    qDebug() << "destruction";
}

void Recorder::start() {
    gboolean res;
    GstBus *bus;
    GstMessage *msgg;
    /* init GStreamer */
    gst_init (NULL, NULL);
//    this->loop = g_main_loop_new (NULL, FALSE);

    /* setup pipeline */
    this->pipeline = gst_pipeline_new ("pipeline");

//    webmmux name=mux ! filesink location=test.webm
    this->webmmux = gst_element_factory_make ("webmmux", "mux");
    this->video_0 = gst_element_factory_make ("mux.video_0", "mux.video_0");
//    this->audio_0 = gst_element_factory_make ("mux.audio_0", "mux.audio_0");
    this->queue = gst_element_factory_make ("queue", "queue");
    this->appsrc = gst_element_factory_make ("appsrc", "source");
    this->conv = gst_element_factory_make ("videoconvert", "conv");
    this->videoflip = gst_element_factory_make ("videoflip", "videoflip");
    this->videosink = gst_element_factory_make ("xvimagesink", "videosink");
    this->vp_enc = gst_element_factory_make ("vp8enc", "vp8enc");

    this->filesink = gst_element_factory_make ("filesink", "filesink");
//    this->filesink = gst_element_factory_make ("ximagesink", "filesink");


//     pulsesrc ! audioconvert ! vorbisenc ! queue ! mux.audio_0
    this->pulsesrc = gst_element_factory_make ("pulsesrc","pulsesrc");
    this->audioconvert = gst_element_factory_make ("audioconvert","audioconvert");
    this->vorbisenc = gst_element_factory_make ("vorbisenc", "vorbisenc");
    this->audio_queue = gst_element_factory_make ("queue", "audio_queue");
    this->audio_0 = gst_element_factory_make ("mux.audio_0", "mux.audio_0");


    /* setup */
    g_object_set (G_OBJECT (this->appsrc), "caps",
                  gst_caps_new_simple ("video/x-raw",
                                       "format", G_TYPE_STRING, "RGBA",
                                       "width", G_TYPE_INT, this->window_width,
                                       "height", G_TYPE_INT, this->window_height,
                                       "framerate", GST_TYPE_FRACTION, 25, 1,
                                       NULL), NULL);

    g_object_set(G_OBJECT(this->appsrc), "is-live", TRUE, NULL);
    g_object_set(G_OBJECT(this->appsrc), "block", TRUE, NULL);
    g_object_set(G_OBJECT(this->appsrc), "do-timestamp", TRUE, NULL);
    g_object_set(G_OBJECT(this->vp_enc), "threads", 3, NULL);
//    g_object_set(G_OBJECT(this->vp_enc), "target-bitrate", 256000*8, NULL);
    g_object_set(G_OBJECT(this->vp_enc), "deadline", 1, NULL);
    g_object_set(G_OBJECT(this->vp_enc), "cpu-used", 15, NULL);
    g_object_set(G_OBJECT(this->videoflip), "method", 5, NULL);
    g_object_set(G_OBJECT(webmmux), "name", "mux", NULL);

    g_object_set(G_OBJECT (filesink), "location", "newfile.webm", NULL);

    gst_bin_add_many (
                GST_BIN (pipeline),
                this->appsrc,
                this->conv,
                this->videoflip,
                this->vp_enc,
                this->queue,

                this->pulsesrc,
                this->audioconvert,
                this->vorbisenc,
                this->audio_queue,

                this->webmmux,
                this->filesink,

                NULL);

    gst_element_link_many (
                this->appsrc,
                this->conv,
                this->videoflip,
                this->vp_enc,
                this->queue,

//                this->pulsesrc,
//                this->audioconvert,
//                this->vorbisenc,
//                this->audio_queue,

                this->webmmux,


                NULL);
    gst_element_link_many (
//                this->appsrc,
//                this->conv,
//                this->videoflip,
//                this->vp_enc,
//                this->queue,

                this->pulsesrc,
                this->audioconvert,
                this->vorbisenc,
                this->audio_queue,

                this->webmmux,


                NULL);
    gst_element_link(this->webmmux, this->filesink);



    /* setup appsrc */
    g_object_set (G_OBJECT (this->appsrc),
                  "stream-type", 0,
                  "format", GST_FORMAT_TIME, NULL);
    g_signal_connect (this->appsrc, "need-data", G_CALLBACK (cbNeedData), this);

    /* play */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    qDebug() << "1";
    //fixme don't know if needed
//        g_main_loop_run (this->loop);


//    /* Wait until error or EOS */
//    bus = gst_element_get_bus (pipeline);
//    qDebug() << "2";
//    msgg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
//    qDebug() << "3";

////    /* Free resources */
//    if (msgg != NULL)
//        gst_message_unref (msgg);
//    gst_object_unref (bus);
//    gst_element_set_state (pipeline, GST_STATE_NULL);
//    gst_object_unref (pipeline);
//    QApplication::quit();
}

void Recorder::stop() {
    qDebug() << "stop fired";
    GstBus *bus;
    GstMessage *msgg;
    /* clean up */
//    gst_element_set_state (pipeline, GST_STATE_PAUSED);
//    gst_element_send_event(this->pipeline, gst_event_new_eos());
    qDebug() << "1";
    msgg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
    if (msgg != NULL) {

        gst_message_unref (msgg);
        qDebug() << "2";
    }
    gst_object_unref (bus);
    qDebug() << "3";
    gst_element_set_state (pipeline, GST_STATE_NULL);
    qDebug() << "4";
    gst_object_unref (pipeline);
    qDebug() << "5";
    QApplication::quit();
//    gst_element_set_state (pipeline, GST_STATE_NULL);
//    gst_object_unref (GST_OBJECT (pipeline));
//    QApplication::quit();
//    g_main_loop_unref (this->loop);
}

QWidget* Recorder::getMainWindowLink() {
    return this->mainWin;
}

void Recorder::cbNeedData(GstElement *appsrc_,
                          guint       unused_size,
                          Recorder  * user_data)
{
    static long num_frame = 0;
    guint  size;
    GstFlowReturn ret;

    GstBuffer *buffer;

    //todo verify size
    size = 1280 * 480 * 4;

    buffer = gst_buffer_new_allocate (NULL, size, NULL);

    (qobject_cast<MainWindow*>(user_data->getMainWindowLink()))->gstBufferFill(buffer, size);

    g_signal_emit_by_name (appsrc_, "push-buffer", buffer, &ret);

    gst_buffer_unref (buffer);
    if (ret != GST_FLOW_OK) {
//        /* something wrong, stop pushing */
//        //        g_main_loop_quit (loop);
        qDebug() << "fuck";
    }
}
