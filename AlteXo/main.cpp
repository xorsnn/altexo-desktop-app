#include "mainwindow.h"
//#include "alkinectinterface.h"


#include <QApplication>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QPluginLoader>
#include "interfaces/AlRecorderInterface.h"
#include "interfaces/AlStreamerInterface.h"
#include "interfaces/AlSensorInterface.h"
#include "interfaces/AlWsInterface.h"
#include "extra/alstreamerconnector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    // **
    // * KINECT
    // *

    AlSensorInterface* sensorInterface = NULL;
    QPluginLoader sensorPluginLoader("/home/xors/workspace/QT/al_build/source/build/libkinect_source.so");
    QObject *sensorPlugin = sensorPluginLoader.instance();
    if (sensorPlugin) {
        qDebug() << "sensor loaded";
        sensorInterface = qobject_cast<AlSensorInterface *>(sensorPlugin);
        if (sensorInterface) {
            qDebug() << "sensor inctanced";
            sensorInterface->init(argc, argv);
        }
    }

    QObject::connect(sensorInterface->getObj(), SIGNAL(newWFrameSignal(QImage)), w.getVideoSurfaceRef(), SLOT(newFrameSlot(QImage)));
    QObject::connect(&w, SIGNAL(requestNewFrameSignal()), sensorInterface->getObj(), SLOT(needWDataSlot()));

    //broadcaster
    QObject::connect(&w, SIGNAL(settingsChangedSignal()), sensorInterface->getObj(), SLOT(updateSettingsSlot()));

    // **
    // * WEBSOCKET CLIENT
    // *

    AlWsInterface* wsInterface = NULL;
    QObject* client = NULL;
    QPluginLoader wsPluginLoader("/home/xors/workspace/QT/al_build/streamer/build/libmsg-handler.so");
    QObject *wsPlugin = wsPluginLoader.instance();
    if (wsPlugin) {
        qDebug() << "ws loaded";
        wsInterface = qobject_cast<AlWsInterface *>(wsPlugin);
        if (wsInterface) {
            qDebug() << "ws inctanced";
            wsInterface->init();
            client = wsInterface->getObj();
        }
    }
    QObject::connect(client, SIGNAL(closed()), &a, SLOT(quit()));


    // **
    // * RECORDER
    // *

    QPluginLoader recorderPluginLoader("/home/xors/workspace/QT/al_build/recorder/build/librecorder.so");
    QObject *recorderPlugin = recorderPluginLoader.instance();
    if (recorderPlugin) {
        qDebug() << "recorder loaded";
        AlRecorderInterface* recorderInterface = qobject_cast<AlRecorderInterface *>(recorderPlugin);
        if (recorderInterface) {
            recorderInterface->init(argc, argv);
            QObject* alRecorder = recorderInterface->getObj();

            QObject::connect(recorderInterface->getAppSrcRef(), SIGNAL(needDataSignal()), sensorInterface->getObj(), SLOT(needDataSlot()));
            QObject::connect(sensorInterface->getObj(), SIGNAL(newFrameSignal(QImage)), recorderInterface->getAppSrcRef(), SLOT(newFrameSlot(QImage)));

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
    QPluginLoader streamerPluginLoader("/home/xors/workspace/QT/al_build/streamer/build/libal-one2one-streamer.so");
    QObject *streamerPlugin = streamerPluginLoader.instance();
    if (streamerPlugin) {
        qDebug() << "Streamer plugin loaded";
        AlStreamerInterface* streamerInterface = qobject_cast<AlStreamerInterface *>(streamerPlugin);
        if (streamerInterface) {
            qDebug() << "Streamer interface loaded";
            streamerInterface->initStreamer(&a);

            //TODO tmp connect for now
            QObject::connect(streamerInterface->getConductor(), SIGNAL(onInitSignal()), streamerInterface->getConductor(), SLOT(startInitialExchangeSlot()));

            QObject::connect(&w, SIGNAL(signalStartButton_clicked()), streamerInterface->getConductor(), SLOT(StartAll()));
            QObject::connect(&w, SIGNAL(signalProcessAnswerButton_clicked(QString)), streamerInterface->getConductor(), SLOT(slotProcessAnswer(QString)));
            QObject::connect(&w, SIGNAL(signalProcessRemoteICEButton_clicked(QString)), streamerInterface->getConductor(), SLOT(slotProcessRemoteICE(QString)));
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalSDPText(QString)), &w, SLOT(slotSDPText(QString)));
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalOnLocalIceCandidate(QString)), &w, SLOT(slotOnLocalIceCandidate(QString)));

            //socket
            QObject::connect(streamerInterface->getConductor(), SIGNAL(signalSDPText(QString)), client, SLOT(sendSdpSlot(QString)));
            QObject::connect(&w, SIGNAL(sendIceCandidatesSignal(QString)), client, SLOT(sendIceCandidatesSlot(QString)));
            QObject::connect(&w, SIGNAL(sendTextMessageSignal(QString)), client, SLOT(sendTextMessageSlot(QString)));

            QObject::connect(client, SIGNAL(onTextMessageReceivedSignal(QString)), streamerInterface->getConductor(), SLOT(onJsonMsgSlot(QString)));
            QObject::connect(client, SIGNAL(onTextMessageReceivedSignal(QString)), &w, SLOT(onJsonMsgSlot(QString)));

            connector.setStreamignObj(sensorInterface->getObj());
            connector.setConductor(streamerInterface->getConductor());
            QObject::connect(&w, SIGNAL(readyToStreamSignal()), &connector, SLOT(start()));

            streamerInterface->runStreamer();
        }
    }

    return a.exec();

}
