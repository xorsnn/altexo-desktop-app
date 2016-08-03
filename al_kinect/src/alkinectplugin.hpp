
#ifndef ALKINECTPLUGIN_H
#define ALKINECTPLUGIN_H

#include "AL_API/sensor_api.hpp"
#include "alfreenectdevice.h"
#include <boost/config.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <string>

class AlManager;

class AlKinectPlugin : public AlSensorAPI {
public:
  AlKinectPlugin();
  ~AlKinectPlugin();

  void threadMain();

  void init(AlSensorCb *alSensorCb);
  void requestNewFrame();

  void start();
  void stop();

private:
  ALFreenectDevice *m_device;
  Freenect::Freenect *m_freenect;

  boost::thread *m_internalThread;

  AlSensorCb *m_sensorCb;
  boost::signals2::signal<void(std::vector<uint8_t> rgbFrame,
                               std::vector<uint16_t> depthFrame)>
      m_newFrameSignal;

  bool m_initFinished;
  bool m_debug;
};

extern "C" BOOST_SYMBOL_EXPORT AlKinectPlugin plugin;
AlKinectPlugin plugin;

#endif // ALKINECTPLUGIN_H
