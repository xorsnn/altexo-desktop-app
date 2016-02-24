#include "recorderplugin.h"

void RecorderPlugin::init(QApplication* app)
{
    qDebug() << "init streamer";
//    this->initSlot(app);
//    qDebug() << "init streamer fin";
}

void RecorderPlugin::run()
{
    qDebug() << "run streamer";
//    this->runSlot();
//    qDebug() << "run streamer fin";
}

QObject* RecorderPlugin::getObj()
{
    return this;
}

QObject* RecorderPlugin::getAppSrcRef()
{
    return this->_getAppSrcRef();
}
