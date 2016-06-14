#ifndef SENSORPLUGIN_H
#define SENSORPLUGIN_H

#include "AlSensorInterface.h"
#include "alkinectinterface.h"

class SensorPlugin : public QObject, AlSensorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.altexo.AlSensorInterface")
    Q_INTERFACES(AlSensorInterface)

public:
    void init(int argc, char *argv[]) Q_DECL_OVERRIDE;
    void run() Q_DECL_OVERRIDE;
    QObject* getObj() Q_DECL_OVERRIDE;

private:
    ALKinectInterface *kinectInterface;
};

#endif // SENSORPLUGIN_H
