
#ifndef ALKINECTPLUGIN_H
#define ALKINECTPLUGIN_H

#include "AL_API/sensor_api.hpp"
#include <boost/config.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <librealsense/rs.hpp>
#include <string>

class AlManager;

class Mutex {
public:
  Mutex() { pthread_mutex_init(&m_mutex, NULL); }
  void lock() { pthread_mutex_lock(&m_mutex); }
  void unlock() { pthread_mutex_unlock(&m_mutex); }

  class ScopedLock {
    Mutex &_mutex;

  public:
    ScopedLock(Mutex &mutex) : _mutex(mutex) { _mutex.lock(); }
    ~ScopedLock() { _mutex.unlock(); }
  };

private:
  pthread_mutex_t m_mutex;
};

class AlRealsensePlugin : public AlSensorAPI {
public:
  AlRealsensePlugin();
  ~AlRealsensePlugin();

  void threadMain();

  void init(AlSensorCb *alSensorCb);
  void requestNewFrame();

  void start();
  void stop();

private:
  boost::thread *m_internalThread;

  AlSensorCb *m_sensorCb;

  Mutex m_mutex;
  rs::device *m_dev;

  bool m_running;
  bool m_hasNewFrame;

  uint16_t *m_depthImage;
  uint8_t *m_colorImage;

  std::vector<uint8_t> m_resRgb;
};

extern "C" BOOST_SYMBOL_EXPORT AlRealsensePlugin plugin;
AlRealsensePlugin plugin;

#endif // ALKINECTPLUGIN_H
