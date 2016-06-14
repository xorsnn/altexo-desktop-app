#ifndef ALSENSORINTERFACE_H
#define ALSENSORINTERFACE_H

#include <QObject>
#include <QApplication>

class AlSensorInterface
{
public:
    virtual ~AlSensorInterface() {}

    virtual void init(int argc, char *argv[]) = 0;
    virtual void run() = 0;
    virtual QObject* getObj() = 0;
};


QT_BEGIN_NAMESPACE

#define AlSensorInterface_iid "org.altexo.AlSensorInterface"

Q_DECLARE_INTERFACE(AlSensorInterface, AlSensorInterface_iid)

QT_END_NAMESPACE

#endif // ALSENSORINTERFACE_H
