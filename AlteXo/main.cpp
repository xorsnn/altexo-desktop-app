#include "mainwindow.h"
#include "alkinectinterface.h"
#include "alrecorder.h"
#include <QApplication>
#include <QThread>
#include <QGst/Init>
#include <QGlib/Error>
#include <QTimer>
#include <QDebug>
#include <QPluginLoader>
#include "interfaces/AlStreamerInterface.h"
#include "alwsclient.h"
#include "extra/alstreamerconnector.h"

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

    QObject::connect(&w, SIGNAL(startRecorderSignal()), alRecorder, SLOT(startSlot()));
    QObject::connect(&w, SIGNAL(stopRecorderSignal()), alRecorder, SLOT(stopSlot()));
    QObject::connect(&w, SIGNAL(settingsChangedSignal()), kinectInterface, SLOT(updateSettingsSlot()));

//==================
    AlWsClient client(true);
//    AlWsClient client(QUrl(QStringLiteral("ws://127.0.0.1:8889/ws")), true);
    QObject::connect(&client, &AlWsClient::closed, &a, &QApplication::quit);
//    QObject::connect(&w, SIGNAL(sendTextMsgSignal(QString)), &client, SLOT(sendTextMessageSlot(QString)));



//==================
//==========streamer
//==================
    AlStreamerConnector connector;
    QPluginLoader pluginLoader("/home/xors/workspace/QT/al_build/streamer/build/libal-live-streamer.so");
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        qDebug() << "yahoo!!! 1";
        AlStreamerInterface* streamerInterface = qobject_cast<AlStreamerInterface *>(plugin);
        if (streamerInterface) {
            qDebug() << "yahoo!!! 2";
            streamerInterface->initStreamer(&a);

            QObject::connect(&w, SIGNAL(signalStartButton_clicked()), streamerInterface->getConductor(), SLOT(StartAll()));
            QObject::connect(&w, SIGNAL(signalProcessAnswerButton_clicked(QString)), streamerInterface->getConductor(), SLOT(slotProcessAnswer(QString)));
            QObject::connect(&w, SIGNAL(signalProcessRemoteICEButton_clicked(QString)), streamerInterface->getConductor(), SLOT(slotProcessRemoteICE(QString)));
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalSDPText(QString)), &w, SLOT(slotSDPText(QString)));
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalOnLocalIceCandidate(QString)), &w, SLOT(slotOnLocalIceCandidate(QString)));

            //socket
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalSDPText(QString)), &client, SLOT(sendSdpSlot(QString)));
            QObject::connect(&w, SIGNAL(sendIceCandidatesSignal(QString)), &client, SLOT(sendIceCandidatesSlot(QString)));

            QObject::connect(&client, SIGNAL(onTextMessageReceivedSignal(QString)), streamerInterface->getConductor(), SLOT(onJsonMsgSlot(QString)));

            QObject::connect(&client, SIGNAL(onTextMessageReceivedSignal(QString)), &w, SLOT(onJsonMsgSlot(QString)));

//            QObject::connect(kinectInterface, SIGNAL(newWFrameSignal(QImage)), streamerInterface->getConductor(), SLOT(slotSetNewFrame(QImage)));
            connector.setStreamignObj(kinectInterface);
            connector.setConductor(streamerInterface->getConductor());
            QObject::connect(&w, SIGNAL(readyToStreamSignal()), &connector, SLOT(start()));

            streamerInterface->runStreamer();
        }
    }



    return a.exec();

}
