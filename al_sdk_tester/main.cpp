#include "mainwindow.h"
#include <QApplication>
#include "alconnclient.h"
#include "peerclientcallback.h"

#include <iostream>
#include <dlfcn.h>

//#include "alkinectinterface.h"
#include "interfaces/AlSensorInterface.h"
#include <QPluginLoader>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    PeerClientCallback cb(&a);

    MainWindow w;
    w.show();
    w.RegisterObserver(&cb);

    AlConnClient cl;
    w.setWsClient(&cl);

    // **
    // * WS_CLIENT-MAIN_WINDOW
    // *
    QObject::connect(&cl, SIGNAL(OnPeerConnectedSignal(QString, QString)),
                     &w, SLOT(OnPeerConnectedSlot(QString, QString)));
    QObject::connect(&w, SIGNAL(ConnectToPeerSignal(QString)), &cl, SLOT(ConnectToPeerSlot(QString)));

    // **
    // * WS_CLIENT-CONDUCTOR
    // *
    QObject::connect(&cl, SIGNAL(OnSignedInSignal()), &cb, SLOT(OnSignedInSlot()));

    QObject::connect(&cb, SIGNAL(SendToPeerSignal(QString, const QString&)),
                     &cl, SLOT(SendToPeerSlot(QString, const QString&)));

    QObject::connect(&cb, SIGNAL(SendHangUpSignal(QString)),
                     &cl, SLOT(SendHangUpSlot(QString)));
    // TODO move to client-window interaction
//    QObject::connect(&cl, SIGNAL(OnPeerConnectedSignal(QString, QString)),
//                     conductor, SLOT(OnPeerConnectedSlot(QString, QString)));
    QObject::connect(&cl, SIGNAL(OnMessageFromPeerSignal(std::string, std::string)),
                     &cb, SLOT(OnMessageFromPeerSlot(std::string, std::string)));
    QObject::connect(&cl, SIGNAL(ConnectToPeerSignal(QString)), &cb, SLOT(ConnectToPeerSlot(QString)));

    // **
    // * WS_CLIENT-MAIN_WIDOW
    // *

    QObject::connect(&cb, SIGNAL(newFrameSignal(QImage*)), &w, SLOT(onNewFrameSlot(QImage*)));
    QObject::connect(&cb, SIGNAL(onDevicesListChangedSignal(std::vector<std::string>)),
                     &w, SLOT(onDevicesListChangedSlot(std::vector<std::string>)));
    QObject::connect(&w, SIGNAL(selectVideoDeviceSignal(QString)), &cb, SLOT(selectVideoDeviceSlot(QString)));


    // **
    // * KINECT
    // *

    AlSensorInterface* sensorInterface = NULL;
    QPluginLoader sensorPluginLoader("/home/xors/workspace/QT/kinect_source_build/source/build/libkinect_source.so");
    QObject *sensorPlugin = sensorPluginLoader.instance();
    if (sensorPlugin) {
        qDebug() << "sensor loaded";
        sensorInterface = qobject_cast<AlSensorInterface *>(sensorPlugin);
        if (sensorInterface) {
            qDebug() << "sensor inctanced";
            sensorInterface->init(argc, argv);
        }
    }

    QObject::connect(sensorInterface->getObj(), SIGNAL(newWFrameSignal(QImage*)), &cb, SLOT(newFrameSlot(QImage*)));
    QObject::connect(&cb, SIGNAL(requestNewFrameSignal()), sensorInterface->getObj(), SLOT(needWDataSlot()));


////    broadcaster
////    QObject::connect(&w, SIGNAL(settingsChangedSignal()), sensorInterface->getObj(), SLOT(updateSettingsSlot()));

    //======================================
    //======================================
    //======================================
    //test external lib
    void* handle = dlopen("/home/xors/workspace/QT/build-al_manager-qt5-Release/libal_manager.so", RTLD_LAZY);

    AlManagerInterface* (*create)();
    void (*destroy)(AlManagerInterface*);

    create = (AlManagerInterface* (*)())dlsym(handle, "create_object");
    destroy = (void (*)(AlManagerInterface*))dlsym(handle, "destroy_object");

    AlManagerInterface* myClass = (AlManagerInterface*)create();
    cb.setManager(myClass);
    myClass->init(&cb);
    myClass->run();

    std::cout << "STOPPED" << std::endl;
    destroy( myClass );
    //~ test external lib
    //======================================
    //======================================
    //======================================

    int execReturn = 0;
    //    return a.exec();
    return execReturn;
}
