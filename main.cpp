#include "mainwindow.h"
#include "alkinectinterface.h"
#include "alrecorder.h"
#include <QApplication>
#include <QThread>
#include <QGst/Init>
#include <QGlib/Error>
#include <QTimer>
#include <QDebug>

#include "broadcast/alsharedmemorybroadcaster.h"

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

    AlSharedMemoryBroadcaster* alBroadcaster = new AlSharedMemoryBroadcaster();

    ALRecorder* alRecorder = new ALRecorder();


    //kinect interface
    ALKinectInterface * kinectInterface = new ALKinectInterface();
    QThread* kinectThread = new QThread();
    kinectInterface->moveToThread(kinectThread);
    kinectThread->start();
    QTimer::singleShot(0, kinectInterface, SLOT(init()));
    //~kinect interface

    QObject::connect(alRecorder->getAppSrcRef(), SIGNAL(needDataSignal()), kinectInterface, SLOT(needDataSlot()));
    QObject::connect(kinectInterface, SIGNAL(newFrameSignal(QImage)), alRecorder->getAppSrcRef(), SLOT(newFrameSlot(QImage)));

    //main win
//    QObject::connect(kinectInterface, SIGNAL(newFrameSignal(QImage)), w.getVideoSurfaceRef(), SLOT(newFrameSlot(QImage)));
    QObject::connect(kinectInterface, SIGNAL(newWFrameSignal(QImage)), w.getVideoSurfaceRef(), SLOT(newFrameSlot(QImage)));
    QObject::connect(&w, SIGNAL(requestNewFrameSignal()), kinectInterface, SLOT(needWDataSlot()));

    //broadcaster
//    QObject::connect(kinectInterface, SIGNAL(newWFrameSignal(QImage)), alBroadcaster, SLOT(slotNewFrame(QImage)));

    QObject::connect(&w, SIGNAL(startRecorderSignal()), alRecorder, SLOT(startSlot()));
    QObject::connect(&w, SIGNAL(stopRecorderSignal()), alRecorder, SLOT(stopSlot()));
    QObject::connect(&w, SIGNAL(settingsChangedSignal()), kinectInterface, SLOT(updateSettingsSlot()));

    return a.exec();

}
