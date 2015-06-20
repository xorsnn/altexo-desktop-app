#ifndef ALRECORDER_H
#define ALRECORDER_H

#include <QObject>
#include <QMessageBox>
#include <QDir>
#include <QDialog>
#include <QApplication>

#include <QGlib/Error>
#include <QGlib/Connect>
#include <QGst/Init>
#include <QGst/ElementFactory>
#include <QGst/ChildProxy>
#include <QGst/Pipeline>
#include <QGst/Pad>
#include <QGst/Event>
#include <QGst/Message>
#include <QGst/Bus>
#include <QGst/Utils/ApplicationSource>
#include <QGst/Buffer>

#include <gst/app/gstappsrc.h>

#ifdef Q_WS_X11
# include <QX11Info>
#endif

#include "mainwindow.h"
#include "alvideoappsrc.h"

class ALRecorder : public QObject
{
    Q_OBJECT
public:
    explicit ALRecorder(QObject *parent = 0);

signals:

public slots:
    void startSlot();
    void stopSlot();

    void setMainWindow(MainWindow * mw) {
//        this->m_src.setMainWindow(mw);
//        QObject::connect(&this->m_src, SIGNAL(sigNeedData(uint,char*)), mw->getALGLWidget(), SLOT(needDataSlot(uint,char*)));
//        QObject::connect(mw->getALGLWidget(), SIGNAL(pushDataSignal()), &this->m_src, SLOT(pushVideoBuffer()));
    }

    ALVideoAppSrc * getAppSrcRef() {
        return &(this->m_src);
    }

private:
    enum Device { AudioSrc, VideoSrc };

    QGst::BinPtr createAudioSrcBin();
    QGst::BinPtr createVideoSrcBin();

    void start();
    void stop();

    void onBusMessage(const QGst::MessagePtr & message);

    QGst::PipelinePtr m_pipeline;

    QString statusText;
    QString outputFilePath;

//    MySrc m_src;
    ALVideoAppSrc m_src;
//    void needData(GstAppSrc *src, guint length, gpointer user_data);

};

#endif // ALRECORDER_H
