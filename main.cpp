#include "mainwindow.h"
#include "recorder.h"
#include "alkinectinterface.h"
#include <QApplication>
#include <QThread>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    Recorder* recorder = new Recorder(&w);
    QThread* thread = new QThread();
    recorder->moveToThread(thread);
    QObject::connect(&w, SIGNAL(stopRecorder()), recorder, SLOT(stop()));
    QObject::connect(thread, SIGNAL(started()), recorder, SLOT(start()));
    thread->start();
    return a.exec();
}
