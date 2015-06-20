#include "mainwindow.h"
#include "alkinectinterface.h"
#include "alrecorder.h"
#include <QApplication>
#include <QThread>
//#include <QObject>
#include <QGst/Init>
#include <QGlib/Error>
#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        QGst::init(&argc, &argv);
    } catch (QGlib::Error err) {
        qDebug() << err;
    }

    MainWindow w;
    w.show();

//    qDebug() << "dkdlaskldska";

//    QThread* alRThread = new QThread();
//    alRecorder->moveToThread(alRThread);

//    QObject::connect(&w, SIGNAL(stopRecorder()), alRecorder, SLOT(stop()));
//    QObject::connect(alRThread, SIGNAL(started()), alRecorder, SLOT(startSlot()));
//    alRThread->start();

    //old recorder
//    Recorder* recorder = new Recorder(&w);
//    QThread* thread = new QThread();
//    recorder->moveToThread(thread);
//    QObject::connect(&w, SIGNAL(stopRecorder()), recorder, SLOT(stop()));
//    QObject::connect(thread, SIGNAL(started()), recorder, SLOT(start()));
//    thread->start();

    ALRecorder* alRecorder = new ALRecorder();
//    alRecorder->setMainWindow(&w);
//    QObject::connect(alRecorder->si)


//    QTimer::singleShot(10000, alRecorder, SLOT(stopSlot()));

    //kinect interface
    ALKinectInterface * kinectInterface = new ALKinectInterface();
    QThread* kinectThread = new QThread();
    kinectInterface->moveToThread(kinectThread);
    kinectThread->start();
    QTimer::singleShot(0, kinectInterface, SLOT(init()));
//    QTimer::singleShot(0, kinectInterface, SLOT(start()));
    //~kinect interface

    QObject::connect(alRecorder->getAppSrcRef(), SIGNAL(needDataSignal()), kinectInterface, SLOT(needDataSlot()));
    QObject::connect(kinectInterface, SIGNAL(newFrameSignal(QImage)), alRecorder->getAppSrcRef(), SLOT(newFrameSlot(QImage)));
    //main win
    QObject::connect(kinectInterface, SIGNAL(newFrameSignal(QImage)), w.getVideoSurfaceRef(), SLOT(newFrameSlot(QImage)));

    alRecorder->startSlot();

    return a.exec();

}
