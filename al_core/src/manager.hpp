#ifndef MANAGER_H
#define MANAGER_H

#include "AL_API/al_plugin_test_api.hpp"
#include "AL_API/alwebrtcpluginapi.hpp"
#include "AL_API/sdk_api.hpp"
#include "AL_API/sensor_api.hpp"
#include "AL_API/ws_client_api.hpp"
#include "AL_CB/al_sdk_cb.hpp"
#include "allogger.hpp"
#include "contact.hpp"
#include "scenerenderercb.hpp"
#include <boost/dll/import.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <queue>
#include "al_core_export.h"

typedef boost::shared_ptr<AlWsAPI>(WsClientFactotry)();

class AL_CORE_EXPORT Manager : public AlWsCb, public AlSDKCb {
public:
  enum SensorType {
    KINECT_1 = 1,
    FAKE_SENSOR,
    REALSENSE,
  };

  struct VideoSetting {
    bool isOn;
    al::VideoMode videoMode;
  };

  Manager();
  ~Manager();

  void frameThread();
  void sdkThread();

  // plugins init
  void initSensor(AlSensorCb *sensorCb, SensorType sensorType);
  void initWsConnection(AlWsCb *alWsCb);
  void initSdk();

  // ws cb
  void onIceCandidateCb(AlTextMessage msg);
  void onSdpAnswerCb(AlTextMessage msg);
  void onSdpOfferCb(const char *msg);
  void onInitCall();

  // sdk cb
  void onLocalSdpCb(AlTextMessage sdp);
  void onLocalIceCandidateCb(AlTextMessage candidate);
  void onNewCaptureDeciceCb(const char *newDeviceName);
  void updateFrameCb(const uint8_t *image, int width, int height); // REMOTE
  void updateLocalFrameCb(const uint8_t *image, int width, int height); // LOCAL

  void initConnection(std::string peerId);
  void setConnectionMode(std::string mode);

  void handleMessages();
  void initHoloRenderer(SceneRendererCb *holoRenderer);

  void setDeviceName(alMsg deviceNama, AlSdkAPI::DesiredVideoSource deviceType);
  int getDeviceType() { return m_videoDeviceType; }

  void toggleVideo();

  boost::shared_ptr<AlSdkAPI> m_sdk;


  boost::shared_ptr<AlWsAPI> m_wsClient;
  boost::function<WsClientFactotry> m_wsClientFactory;

  // TODO remove I think
  boost::shared_ptr<AlPluginTestAPI> m_plugin_test;

  std::vector<CONTACT> contactList;
  std::vector<alMsg> webcamList;
  std::vector<alMsg> sensorList;

  bool connectionInitialized = false;

private:
  void _initVideoDevice();

  boost::shared_ptr<AlSensorAPI> m_sensor;

  boost::thread *m_frameThread;
  boost::thread *m_sdkThread;

  boost::shared_ptr<AlWebRtcPluginApi> m_sdkPlugin;

  // TODO: remove deprecated
  std::string m_id;
  std::string m_peerId;

  std::string m_videoDeviceName;
  AlSdkAPI::DesiredVideoSource m_videoDeviceType;

  // NOTE: video mode
  VideoSetting m_videoSetting;

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

  SceneRendererCb *m_holoRenderer;

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
