#ifndef STREAMERPLUGIN_H
#define STREAMERPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "alstreamer.h"
#include "AlStreamerInterface.h"

class StreamerPlugin : public AlStreamer, AlStreamerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.altexo.AlStreamerInterface")
    Q_INTERFACES(AlStreamerInterface)

public:
    void initStreamer(QGuiApplication* app) Q_DECL_OVERRIDE;
    void runStreamer() Q_DECL_OVERRIDE;
    QObject* getConductor() Q_DECL_OVERRIDE;
};

#endif // STREAMERPLUGIN_H
