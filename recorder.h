#ifndef RECORDER_H
#define RECORDER_H

#include <gst/gst.h>
//#include <gst/videofilter/gstvideoflip.h>
#include <QObject>
#include <QDebug>
#include <QWidget>
#include <QMetaObject>
#include "mainwindow.h"

#define CHEESE_VIDEO_ENC_PRESET "Profile Realtime"

class Recorder : public QObject
{
    Q_OBJECT
public:
    explicit Recorder(QWidget* mainWin, QObject *parent = 0);

signals:

public slots:

    void start();
    void stop();

    QWidget* getMainWindowLink();


private:
    //    QGst::BinPtr createAudioSrcBin();
    //    QGst::BinPtr createVideoSrcBin();

    static void cbNeedData(GstElement *appsrc,
                           guint       unused_size,
                           Recorder   * user_data);

private:
    GstElement *pipeline;
    GstElement *appsrc;
    GstElement *conv;
    GstElement *videosink; //todo remove
    GstElement *vp_enc;
    GstElement *webmmux;
    GstElement *video_0;
//    GstElement *audio_0;
    GstElement *filesink;
    GstElement *queue_before_mux;
    GstElement *queue_before_enc;
    GstElement *queue_before_file;
    GstElement *queue;
    GstElement *videoflip;

    GstElement *pulsesrc;
    GstElement *audioconvert;
    GstElement *vorbisenc;
    GstElement *audio_queue;
    GstElement *audio_0;

    GMainLoop *loop;

    QWidget* mainWin;

    int window_width = 1280; // The width of our window
    int window_height = 480;

    unsigned char *output = new unsigned char[1280 * 480 * 4];


    //    void onBusMessage(const QGst::MessagePtr & message);
};

#endif // RECORDER_H
