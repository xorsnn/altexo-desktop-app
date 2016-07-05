
#ifndef ALKINECTPLUGIN_H
#define ALKINECTPLUGIN_H

#include "AL_API/sensor_api.hpp"
#include "alkinectinterface.h"
#include <boost/config.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <string>

class AlCallback;
class AlManager;

class AlKinectPlugin : public AlSensorAPI {
public:
  AlKinectPlugin();
  ~AlKinectPlugin();

  void init(AlSensorCb *alSensorCb);
  void requestNewFrame();

private:
  void threadMain();
  ALKinectInterface *m_kinectInterface;
  boost::thread m_internalThread;
  AlSensorCb *m_sensorCb;

  boost::signals2::signal<void()> m_needNewFrameSignal;
};

extern "C" BOOST_SYMBOL_EXPORT AlKinectPlugin plugin;
AlKinectPlugin plugin;

#endif // ALKINECTPLUGIN_H
