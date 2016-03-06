#include "alwsplugin.h"

void AlWsPlugin::init(QString address) {
    this->init_(address);
}

QObject* AlWsPlugin::getObj()
{
    return this;
}

