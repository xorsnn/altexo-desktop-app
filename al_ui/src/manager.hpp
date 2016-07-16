#ifndef MANAGER_H
#define MANAGER_H

#include "AL_API/sdk_api.hpp"
#include "AL_API/sensor_api.hpp"
#include "AL_API/ws_client_api.hpp"
#include "AL_CB/al_sdk_cb.hpp"
#include "contact.hpp"
#include <boost/dll/import.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>

class Manager : public AlWsCb, public AlSDKCb {
public:
  Manager();
  ~Manager();

  void frameThread();

  void initSensor(AlSensorCb *sensorCb);
  void initWsConnection(AlWsCb *alWsCb);
  void initSdk();

  // ws cb
  void onMessageCb(std::vector<char> peerId, std::vector<char> msg) {}
  void onWsMessageCb(std::vector<char> msg);

  // sdk cb
  void sendMessageToPeer(std::vector<char> peerId, std::vector<char> msg) {
    if (m_wsClient != NULL) {
      m_wsClient->sendMessageToPeer(peerId, msg);
    }
  }
  void initConnection(std::vector<char> peerId, std::vector<char> mode);

  boost::shared_ptr<AlSdkAPI> m_sdk;
  std::vector<CONTACT> contactList;

private:
  boost::shared_ptr<AlWsAPI> m_wsClient;
  boost::shared_ptr<AlSensorAPI> m_sensor;
  boost::thread m_frameThread;

  std::string m_id = "";

  boost::signals2::signal<void(std::vector<char>, std::vector<char>)>
      newMessageToSdk;
};

#endif // MANAGER_H
