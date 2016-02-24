#include "mainwindow.h"
#include "alkinectinterface.h"

#include <QApplication>
#include <QThread>
//#include <QGst/Init>
//#include <QGlib/Error>
#include <QTimer>
#include <QDebug>
#include <QPluginLoader>
#include "interfaces/AlRecorderInterface.h"
#include "interfaces/AlStreamerInterface.h"
#include "alwsclient.h"
#include "extra/alstreamerconnector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    //kinect interface
    ALKinectInterface * kinectInterface = new ALKinectInterface();
    QThread* kinectThread = new QThread();
    kinectInterface->moveToThread(kinectThread);
    kinectThread->start();
    QTimer::singleShot(0, kinectInterface, SLOT(init()));
    //~kinect interface


    //main win
//    QObject::connect(kinectInterface, SIGNAL(newFrameSignal(QImage)), w.getVideoSurfaceRef(), SLOT(newFrameSlot(QImage)));

    QObject::connect(kinectInterface, SIGNAL(newWFrameSignal(QImage)), w.getVideoSurfaceRef(), SLOT(newFrameSlot(QImage)));
    QObject::connect(&w, SIGNAL(requestNewFrameSignal()), kinectInterface, SLOT(needWDataSlot()));

    //broadcaster
    QObject::connect(&w, SIGNAL(settingsChangedSignal()), kinectInterface, SLOT(updateSettingsSlot()));

//==================
    AlWsClient client(true);
    QObject::connect(&client, &AlWsClient::closed, &a, &QApplication::quit);


    // **
    // * RECORDER
    // *

    QPluginLoader pluginLoader("/home/xors/workspace/QT/al_build/recorder/build/librecorder.so");
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        qDebug() << "recorder loaded";
        AlRecorderInterface* recorderInterface = qobject_cast<AlRecorderInterface *>(plugin);
        if (recorderInterface) {
            recorderInterface->init(argc, argv);
            QObject* alRecorder = recorderInterface->getObj();

            QObject::connect(recorderInterface->getAppSrcRef(), SIGNAL(needDataSignal()), kinectInterface, SLOT(needDataSlot()));
            QObject::connect(kinectInterface, SIGNAL(newFrameSignal(QImage)), recorderInterface->getAppSrcRef(), SLOT(newFrameSlot(QImage)));

            QObject::connect(&w, SIGNAL(startRecorderSignal()), alRecorder, SLOT(startSlot()));
            QObject::connect(&w, SIGNAL(stopRecorderSignal()), alRecorder, SLOT(stopSlot()));
            qDebug() << "recorder initiated";
        }
    }

    // TODO sometimes open settins just doesn't work
    SettingsDialog sDialog;
    sDialog.setModal(true);
    int ret = sDialog.exec();

    // **
    // * streamer
    // *

    AlStreamerConnector connector;
    QPluginLoader streamerPluginLoader("/home/xors/workspace/QT/al_build/streamer/build/libal-live-streamer.so");
    QObject *streamerPlugin = streamerPluginLoader.instance();
    if (streamerPlugin) {
        qDebug() << "Streamer plugin loaded";
        AlStreamerInterface* streamerInterface = qobject_cast<AlStreamerInterface *>(streamerPlugin);
        if (streamerInterface) {
            qDebug() << "Streamer interface loaded";
            streamerInterface->initStreamer(&a);

            QObject::connect(&w, SIGNAL(signalStartButton_clicked()), streamerInterface->getConductor(), SLOT(StartAll()));
            QObject::connect(&w, SIGNAL(signalProcessAnswerButton_clicked(QString)), streamerInterface->getConductor(), SLOT(slotProcessAnswer(QString)));
            QObject::connect(&w, SIGNAL(signalProcessRemoteICEButton_clicked(QString)), streamerInterface->getConductor(), SLOT(slotProcessRemoteICE(QString)));
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalSDPText(QString)), &w, SLOT(slotSDPText(QString)));
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalOnLocalIceCandidate(QString)), &w, SLOT(slotOnLocalIceCandidate(QString)));

            //socket
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalSDPText(QString)), &client, SLOT(sendSdpSlot(QString)));
            QObject::connect(&w, SIGNAL(sendIceCandidatesSignal(QString)), &client, SLOT(sendIceCandidatesSlot(QString)));
            QObject::connect(&w, SIGNAL(sendTextMessageSignal(QString)), &client, SLOT(sendTextMessageSlot(QString)));

            QObject::connect(&client, SIGNAL(onTextMessageReceivedSignal(QString)), streamerInterface->getConductor(), SLOT(onJsonMsgSlot(QString)));
            QObject::connect(&client, SIGNAL(onTextMessageReceivedSignal(QString)), &w, SLOT(onJsonMsgSlot(QString)));

            connector.setStreamignObj(kinectInterface);
            connector.setConductor(streamerInterface->getConductor());
            QObject::connect(&w, SIGNAL(readyToStreamSignal()), &connector, SLOT(start()));

            streamerInterface->runStreamer();
        }
    }

    return a.exec();

}
