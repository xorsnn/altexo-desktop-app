#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtGui/QGuiApplication>

#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformintegration.h>

#include <QtQuick/QQuickView>

#include "threadrenderer.h"

//#include <QDialog>
#include <QPluginLoader>
#include <QObject>
#include "AlStreamerInterface.h"
#include "AlWsInterface.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ThreadRenderer>("SceneGraphRendering", 1, 0, "Renderer");
    int execReturn = 0;

    {
        QQuickView view;

        // Rendering in a thread introduces a slightly more complicated cleanup
        // so we ensure that no cleanup of graphics resources happen until the
        // application is shutting down.
        view.setPersistentOpenGLContext(true);
        view.setPersistentSceneGraph(true);

        view.setResizeMode(QQuickView::SizeRootObjectToView);
        view.setSource(QUrl("qrc:/main.qml"));
        view.show();

        // **
        // * WS CLIENT
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
                wsInterface->init("ws://127.0.0.1:8889/alStreamer");
                client = wsInterface->getObj();
            }
        }

//        AlStreamerConnector connector;
        QPluginLoader streamerPluginLoader("/home/xors/workspace/QT/al_build/streamer2/build/libal-one2one-receiver.so");
        QObject *streamerPlugin = streamerPluginLoader.instance();
        if (streamerPlugin) {
            qDebug() << "Streamer plugin loaded";
            AlStreamerInterface* streamerInterface = qobject_cast<AlStreamerInterface *>(streamerPlugin);
            if (streamerInterface) {
                qDebug() << "Streamer interface loaded";
                streamerInterface->initStreamer(&app);
                QMetaObject::invokeMethod(streamerInterface->getConductor(), "StartAll", Qt::QueuedConnection);
//                QObject::connect(&w, SIGNAL(signalStartButton_clicked()), streamerInterface->getConductor(), SLOT(StartAll()));
//                QObject::connect(&w, SIGNAL(signalProcessAnswerButton_clicked(QString)), streamerInterface->getConductor(), SLOT(slotProcessAnswer(QString)));
//                QObject::connect(&w, SIGNAL(signalProcessRemoteICEButton_clicked(QString)), streamerInterface->getConductor(), SLOT(slotProcessRemoteICE(QString)));
//                QObject::connect(streamerInterface->getConductor(), SIGNAL(signalSDPText(QString)), &w, SLOT(slotSDPText(QString)));
//                QObject::connect(streamerInterface->getConductor(), SIGNAL(signalOnLocalIceCandidate(QString)), &w, SLOT(slotOnLocalIceCandidate(QString)));

                //socket
                QObject::connect(streamerInterface->getConductor(), SIGNAL(signalSDPText(QString)), client, SLOT(sendSdpSlot(QString)));
//                QObject::connect(&w, SIGNAL(sendIceCandidatesSignal(QString)), &client, SLOT(sendIceCandidatesSlot(QString)));
//                QObject::connect(&w, SIGNAL(sendTextMessageSignal(QString)), &client, SLOT(sendTextMessageSlot(QString)));

                QObject::connect(client, SIGNAL(onTextMessageReceivedSignal(QString)), streamerInterface->getConductor(), SLOT(onJsonMsgSlot(QString)));
//                QObject::connect(client, SIGNAL(onTextMessageReceivedSignal(QString)), &w, SLOT(onJsonMsgSlot(QString)));

//                connector.setStreamignObj(sensorInterface->getObj());
//                connector.setConductor(streamerInterface->getConductor());
//                QObject::connect(&w, SIGNAL(readyToStreamSignal()), &connector, SLOT(start()));

                streamerInterface->runStreamer();
            }
        }

        execReturn = app.exec();
    }

    // As the render threads make use of our QGuiApplication object
    // to clean up gracefully, wait for them to finish before
    // QGuiApp is taken off the heap.
    for (int i = 0; i < ThreadRenderer::threads.count(); i++) {
//    foreach (QThread *t, ThreadRenderer::threads) {
        QThread *t = ThreadRenderer::threads[i];
        t->wait();
        delete t;
    }
}
