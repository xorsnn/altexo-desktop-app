#ifndef MANAGER_H
#define MANAGER_H

#include "AL_API/al_plugin_test_api.hpp"
#include "AL_API/sdk_api.hpp"
#include "AL_API/sensor_api.hpp"
#include "AL_API/ws_client_api.hpp"
#include "AL_CB/al_sdk_cb.hpp"
#include "contact.hpp"
#include "scenerenderer.hpp"
#include <boost/dll/import.hpp>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <queue>

class Manager : public AlWsCb, public AlSDKCb {
public:
  Manager();
  ~Manager();

  void frameThread();

  // plugins init
  void initSensor(AlSensorCb *sensorCb);
  void initWsConnection(AlWsCb *alWsCb);
  void initSdk();

  // ws cb
  void onIceCandidateCb(AlTextMessage msg);
  void onSdpAnswerCb(AlTextMessage msg);
  void onSdpOfferCb(AlTextMessage msg);
  void onInitCall();

  // sdk cb
  void onLocalSdpCb(AlTextMessage sdp);
  void onLocalIceCandidateCb(AlTextMessage candidate);
  void onDevicesListChangedCb(std::vector<AlTextMessage> deviceNames);
  void updateFrameCb(const uint8_t *image, int width, int height); // REMOTE
  void updateLocalFrameCb(const uint8_t *image, int width, int height); // LOCAL

  void initConnection(std::string peerId);
  void setConnectionMode(std::string mode);

  void handleMessages();
  void initHoloRenderer(SceneRenderer *holoRenderer);

  void setDeviceName(AlTextMessage deviceNama, int deviceType);
  int getDeviceType() { return m_videoDeviceType; }

  boost::shared_ptr<AlSdkAPI> m_sdk;
  boost::shared_ptr<AlPluginTestAPI> m_plugin_test;
  std::vector<CONTACT> contactList;
  std::vector<AlTextMessage> webcamList;
  std::vector<AlTextMessage> sensorList;

  bool connectionInitialized = false;

private:
  void _initVideoDevice();

  boost::shared_ptr<AlWsAPI> m_wsClient;
  boost::shared_ptr<AlSensorAPI> m_sensor;
  boost::thread *m_frameThread;

  std::string m_id;
  std::string m_peerId;
  std::string m_videoDeviceName;
  int m_videoDeviceType;

  std::string m_localSdp = "";
  bool m_sentLocalSdp = false;
  std::queue<std::string> m_localCandidates;
  std::string m_remoteSdp = "";
  bool m_sentRemoteSdp = false;
  std::queue<std::string> m_remoteCandidates;

  // NOTE: counters
  int m_localCandidatesCounter;
  int m_remoteCandidatesCounter;

  boost::signals2::signal<void(int, int)> updateResolutionSignal;

  SceneRenderer *m_holoRenderer;

  // TODO: temorary solution
  bool m_beenCalled;
  bool m_processingCandidates;

  // TODO move mode selection to UI
  int WIDTH = 320;
  int HEIGHT = 240;
  // int WIDTH = 640;
  // int HEIGHT = 480;
  bool m_calling;
};

#endif // MANAGER_H
