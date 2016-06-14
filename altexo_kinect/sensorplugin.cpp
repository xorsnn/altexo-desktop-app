#include "sensorplugin.h"
#include <QThread>
#include <QTimer>

void SensorPlugin::init(int argc, char *argv[])
{
    qDebug() << "init kinect";
//    TODO add destructor
    this->kinectInterface = new ALKinectInterface();
    QThread* kinectThread = new QThread();
    this->kinectInterface->moveToThread(kinectThread);
    kinectThread->start();
    QTimer::singleShot(0, this->kinectInterface, SLOT(init()));
}

void SensorPlugin::run()
{
    qDebug() << "run kinect";
}

QObject* SensorPlugin::getObj()
{
    return this->kinectInterface;
}
