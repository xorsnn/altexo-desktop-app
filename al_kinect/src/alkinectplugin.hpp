
#ifndef ALKINECTPLUGIN_H
#define ALKINECTPLUGIN_H

#include "alkinectinterface.h"
#include "sensor_api.hpp"
#include <boost/config.hpp>
#include <boost/thread.hpp>
#include <string>

class AlCallback;
class AlManager;

class AlKinectPlugin : public AlSensorAPI {
public:
  AlKinectPlugin();
  ~AlKinectPlugin();

  void init(AlCallback *alCallback);

private:
  void threadMain();
  ALKinectInterface *m_kinectInterface;
  boost::thread m_internalThread;
};

extern "C" BOOST_SYMBOL_EXPORT AlKinectPlugin plugin;
AlKinectPlugin plugin;

#endif // ALKINECTPLUGIN_H
