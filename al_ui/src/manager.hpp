#ifndef MANAGER_H
#define MANAGER_H

#include "AL_API/sdk_api.hpp"
#include "AL_API/sensor_api.hpp"
#include "AL_API/ws_client_api.hpp"
#include "contact.hpp"
#include <boost/dll/import.hpp>
#include <boost/thread.hpp>

class Manager : public AlWsCb {
public:
  Manager();
  ~Manager();

  void frameThread();

  void initSensor(AlSensorCb *sensorCb);
  void initWsConnection(AlWsCb *alWsCb);
  void initSdk();

  void onMessageCb(std::vector<char> peerId, std::vector<char> msg) {}
  void onWsMessageCb(std::vector<char> msg);

  // void initRouting();
  boost::shared_ptr<AlSdkAPI> m_sdk;
  std::vector<CONTACT> contactList;

private:
  boost::shared_ptr<AlWsAPI> m_wsClient;
  boost::shared_ptr<AlSensorAPI> m_sensor;
  boost::thread m_frameThread;

  std::string m_peerId = "";
};

#endif // MANAGER_H
