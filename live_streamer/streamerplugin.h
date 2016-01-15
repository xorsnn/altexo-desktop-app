#ifndef STREAMERPLUGIN_H
#define STREAMERPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "interfaces/AlStreamerInterface.h"

//! [0]
class StreamerPlugin : public QObject, AlStreamerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.altexo.AlStreamerInterface")
    Q_INTERFACES(StreamerPlugin)

public:
    QString echo(const QString &message) Q_DECL_OVERRIDE;
};

#endif // STREAMERPLUGIN_H
