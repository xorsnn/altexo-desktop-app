#ifndef MANAGER_H
#define MANAGER_H

#include "AL_API/sdk_api.hpp"
#include "AL_API/sensor_api.hpp"
#include "AL_API/ws_client_api.hpp"
#include <boost/dll/import.hpp>
#include <boost/thread.hpp>

class Manager : public AlSensorCb {
public:
  Manager();
  ~Manager();

  // sensor cb
  void newFrame(std::vector<uint8_t> rgbFrame);

  void frameThread();

  void initSensor(AlSensorCb *sensorCb);
  void initWsConnection();

private:
  boost::shared_ptr<AlWsClientInterface> m_wsClient;
  boost::shared_ptr<AlSensorAPI> m_sensor;
  boost::thread m_frameThread;
};

#endif // MANAGER_H
