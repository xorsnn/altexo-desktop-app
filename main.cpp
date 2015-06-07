#include "mainwindow.h"
#include "recorder.h"
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
    alRecorder->setMainWindow(&w);
//    QObject::connect(alRecorder->si)

    alRecorder->startSlot();

    QTimer::singleShot(20000, alRecorder, SLOT(stopSlot()));

    return a.exec();

}
