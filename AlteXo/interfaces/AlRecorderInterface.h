#ifndef ALRECORDERINTERFACE_H
#define ALRECORDERINTERFACE_H

#include <QObject>
#include <QApplication>

class AlRecorderInterface
{
public:
    virtual ~AlRecorderInterface() {}

    virtual void initStreamer(QApplication* app) = 0;
    virtual void runStreamer() = 0;
    virtual QObject* getConductor() = 0;
};


QT_BEGIN_NAMESPACE

#define AlRecorderInterface_iid "org.altexo.AlRecorderInterface"

Q_DECLARE_INTERFACE(AlRecorderInterface, AlRecorderInterface_iid)

QT_END_NAMESPACE
#endif // ALRECORDERINTERFACE_H
