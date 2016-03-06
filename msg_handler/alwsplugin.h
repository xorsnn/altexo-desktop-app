#ifndef ALWSPLUGIN_H
#define ALWSPLUGIN_H

#include <QObject>
#include "alwsclient.h"
#include "AlWsInterface.h"

class AlWsPlugin : public AlWsClient, AlWsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.altexo.AlWsInterface")
    Q_INTERFACES(AlWsInterface)

public:
    QObject* getObj() Q_DECL_OVERRIDE;
    void init(QString address="") Q_DECL_OVERRIDE;
};

#endif // ALWSPLUGIN_H
