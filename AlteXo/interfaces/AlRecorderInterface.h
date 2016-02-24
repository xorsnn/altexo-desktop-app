#ifndef ALRECORDERINTERFACE_H
#define ALRECORDERINTERFACE_H

#include <QObject>
#include <QApplication>

class AlRecorderInterface
{
public:
    virtual ~AlRecorderInterface() {}

    virtual void init(QApplication* app) = 0;
    virtual void run() = 0;
    virtual QObject* getObj() = 0;
    virtual QObject* getAppSrcRef() = 0;
};


QT_BEGIN_NAMESPACE

#define AlRecorderInterface_iid "org.altexo.AlRecorderInterface"

Q_DECLARE_INTERFACE(AlRecorderInterface, AlRecorderInterface_iid)

QT_END_NAMESPACE
#endif // ALRECORDERINTERFACE_H
