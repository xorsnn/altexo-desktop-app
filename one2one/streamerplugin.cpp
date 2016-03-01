#include "streamerplugin.h"
#include <QDebug>

void StreamerPlugin::initStreamer(QApplication* app)
{
    qDebug() << "init streamer";
    this->initSlot(app);
    qDebug() << "init streamer fin";
}

void StreamerPlugin::runStreamer()
{
    qDebug() << "run streamer";
    this->runSlot();
    qDebug() << "run streamer fin";
}

QObject* StreamerPlugin::getConductor()
{
    return this->getConductorSlot();
}
