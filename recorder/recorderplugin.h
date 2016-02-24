#ifndef RECORDERPLUGIN_H
#define RECORDERPLUGIN_H

#include "AlRecorderInterface.h"
#include "alrecorder.h"

class RecorderPlugin : public AlRecorder, AlRecorderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.altexo.AlRecorderInterface")
    Q_INTERFACES(AlRecorderInterface)

public:
    void init(int argc, char *argv[]) Q_DECL_OVERRIDE;
    void run() Q_DECL_OVERRIDE;
    QObject* getObj() Q_DECL_OVERRIDE;
    QObject* getAppSrcRef() Q_DECL_OVERRIDE;
};

#endif // RECORDERPLUGIN_H
