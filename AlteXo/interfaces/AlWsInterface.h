#ifndef ALWSINTERFACE
#define ALWSINTERFACE

#include <QObject>

class AlWsInterface
{
public:
    virtual ~AlWsInterface() {}

    virtual void init(QString address = "") = 0;
    virtual QObject* getObj() = 0;
};


QT_BEGIN_NAMESPACE

#define AlWsInterface_iid "org.altexo.AlWsInterface"

Q_DECLARE_INTERFACE(AlWsInterface, AlWsInterface_iid)
QT_END_NAMESPACE

#endif // ALWSINTERFACE

