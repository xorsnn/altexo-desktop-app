#ifndef RECORDERPLUGIN_H
#define RECORDERPLUGIN_H

#include "AlRecorderInterface.h"
#include "alrecorder.h"

class RecorderPlugin : public AlRecorder, AlRecorderInterface
{
public:
    RecorderPlugin();
};

#endif // RECORDERPLUGIN_H
