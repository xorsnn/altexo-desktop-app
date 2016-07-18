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
#include <queue>

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
  // void sendMessageToPeer(std::vector<char> peerId, std::vector<char> msg) {
  //   if (m_wsClient != NULL) {
  //     m_wsClient->sendMessageToPeer(peerId, msg);
  //   }
  // }
  // void sendMessageToPeer(std::vector<char> msg) {
  //   // std::string msgStr(msg.begin(), msg.end());
  //   if (m_wsClient) {
  //     std::vector<char> peerIdVec(m_peerId.begin(), m_peerId.end());
  //     m_wsClient->sendMessageToPeer(peerIdVec, msg);
  //   }
  // };
  void onSdp(std::vector<char> sdp);
  void onCandidate(std::vector<char> candidate);

  void initConnection(std::string mode);
  void onMessageFromPeer(boost::property_tree::ptree msgPt);

  void handleMessages();

  boost::shared_ptr<AlSdkAPI> m_sdk;
  std::vector<CONTACT> contactList;

private:
  boost::shared_ptr<AlWsAPI> m_wsClient;
  boost::shared_ptr<AlSensorAPI> m_sensor;
  boost::thread m_frameThread;

  std::string m_id = "";
  std::string m_peerId = "-1";
  std::string m_videoDeviceName = "";

  std::string m_localSdp = "";
  bool m_sentLocalSdp = false;
  std::queue<std::string> m_localCandidates;
  std::string m_remoteSdp = "";
  bool m_sentRemoteSdp = false;
  std::queue<std::string> m_remoteCandidates;

  boost::signals2::signal<void(std::vector<char>, std::vector<char>)>
      newMessageToSdk;

  bool m_debug;
};

#endif // MANAGER_H
