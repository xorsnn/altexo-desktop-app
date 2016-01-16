#include "streamerplugin.h"

void StreamerPlugin::initStreamer()
{
    qDebug() << "init streamer";
}

void StreamerPlugin::runStreamer()
{
    qDebug() << "run streamer";
}

QObject* StreamerPlugin::getConductor()
{
    return NULL;
}
