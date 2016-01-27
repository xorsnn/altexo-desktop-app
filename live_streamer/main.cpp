#include <QApplication>
#include <QDebug>
#include <QTextEdit>



//#include "webrtc/base/thread.h"

#include "alinterface/almainwindow.h"
#include "alconductor/alconductor.h"
#include "alinterface/widgetstreamer.h"
#include "alstreamer.h"

int main(int argc, char *argv[])
{
    QApplication qtApp(argc, argv);
    QTextEdit w1;
    AlMainWindow w;

    w.show();
    w1.show();

    AlStreamer streamer;
    streamer.initSlot(&qtApp);
    qDebug() << "test 1";
//    w.setConductor(streamer.getConductorSlot());
    QObject::connect(&w, SIGNAL(signalStartButton_clicked()), streamer.getConductorSlot(), SLOT(StartAll()));
    QObject::connect(&w, SIGNAL(signalProcessAnswerButton_clicked(QString)), streamer.getConductorSlot(), SLOT(slotProcessAnswer(QString)));
    QObject::connect(&w, SIGNAL(signalProcessRemoteICEButton_clicked(QString)), streamer.getConductorSlot(), SLOT(slotProcessRemoteICE(QString)));
    QObject::connect(streamer.getConductorSlot(), SIGNAL(signalSDPText(QString)), &w, SLOT(slotSDPText(QString)));
    QObject::connect(streamer.getConductorSlot(), SIGNAL(signalOnLocalIceCandidate(QString)), &w, SLOT(slotOnLocalIceCandidate(QString)));

    WidgetStreamer conn(&w1, streamer.getConductorSlot());

    QObject::connect(&w, SIGNAL(signalStartButton_clicked()), &conn, SLOT(startSlot()));

    qDebug() << "test 2";

    streamer.runSlot();
    qDebug() << "test 3";
    return 0;

    //    return a.exec();
}
