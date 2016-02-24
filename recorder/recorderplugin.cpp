#include "recorderplugin.h"

void RecorderPlugin::init(int argc, char *argv[])
{
    qDebug() << "init streamer";
    try {
        QGst::init(&argc, &argv);
    } catch (QGlib::Error err) {
        qDebug() << err;
    }
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
