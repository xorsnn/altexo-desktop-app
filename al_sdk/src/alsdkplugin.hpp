#ifndef ALSDKPLUGIN_H
#define ALSDKPLUGIN_H


#include "AL_API/sdk_api.hpp"
#include "AL_CB/al_sdk_cb.hpp"
#include "alcallback.h"
#include "altextmessage.hpp"
#include "boost/thread.hpp"
#include <boost/config.hpp>
#include <boost/signals2/signal.hpp>
#include <queue>
#include <string>

class AlManager;

class AlSdkPlugin : public AlSdkAPI, public AlCallback {
public:
  AlSdkPlugin();
  ~AlSdkPlugin();

  void init(AlSDKCb *alSdkCb);
  // AlWsCb *getWsCb();
  // void run();
  // bool hasConnections();
  // void InitializePeerConnection();
  void initializePeerConnection();
  void setRemoteSdp(std::vector<char> sdp);
  void setRemoteIceCandidate(std::vector<char> candidate);
  void setImageData(std::vector<unsigned char> imageBytes, int width,
                    int height);
  void updateResolution(int widht, int height);

  // void DeletePeerConnection();
  //
  // bool isClientConnected();
  // void clientConnect(const std::string &server, int port,
  //                    const std::string &client_name);
  // void clientSignOut();
  void setDesiredDataSource(int dataSource);
  // void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);
  void threadMain();

  void onSdpCb(const std::string &message) {
    std::vector<char> msg(message.begin(), message.end());
    m_sdkCb->onSdp(msg);
  }

  void onCandidateCb(const std::string &message) {
    std::vector<char> msg(message.begin(), message.end());
    m_sdkCb->onCandidate(msg);
  }

  void sendHangUpCb(std::string peer_id) {
    if (m_debug) {
      std::cout << "sendHangUpCb" << std::endl;
    }
  }

  void dequeueMessagesFromPeerCb() {
    if (m_debug) {
      std::cout << "dequeueMessagesFromPeerCb" << std::endl;
    }
  }
  void stopLocalRendererCb() {
    if (m_debug) {
      std::cout << "stopLocalRendererCb" << std::endl;
    }
  }
  void stopRemoteRendererCb() {
    if (m_debug) {
      std::cout << "stopRemoteRendererCb" << std::endl;
    }
  }
  void ensureStreamingUICb() {
    if (m_debug) {
      std::cout << "ensureStreamingUICb" << std::endl;
    }
  }
  void queueUIThreadCallbackCb(int msg_id, void *data) {
    if (m_debug) {
      std::cout << "queueUIThreadCallbackCb" << std::endl;
    }
  }
  void switchToPeerListCb(const Peers &peers) {
    if (m_debug) {
      std::cout << "switchToPeerListCb" << std::endl;
    }
  }
  void onDisconnectedCb() {
    if (m_debug) {
      std::cout << "onDisconnectedCb" << std::endl;
    }
  }
  void processUiEventsCb() {
    // if (m_debug) {
    // std::cout << "processUiEventsCb" << std::endl;
    // }
  }
  void SwitchToStreamingUI() {} // ????
  void onDevicesListChangedCb(std::vector<std::string> deviceNames) {
    if (m_debug) {
      std::cout << "onDevicesListChangedCb" << std::endl;
    }
    std::vector<AlTextMessage> deviceNamesMsg;
    for (int i = 0; i < deviceNames.size(); i++) {
      AlTextMessage newMsg(deviceNames[i]);
      deviceNamesMsg.push_back(newMsg);
    }
    m_sdkCb->onDevicesListChangedCb(deviceNamesMsg);
  }

  void updateFrameCb(const uint8_t *image, int width, int height);
  void updateLocalFrameCb(const uint8_t *image, int width, int height);
  void setDesiredVideDeviceName(AlTextMessage deviceName);

  //  TODO move to AlManager
  std::string getVideoDeviceName() {
    if (m_debug) {
      std::cout << "1111111111111111111111111111" << std::endl;
      std::cout << m_videoDeviceName << std::endl;
    }
    return m_videoDeviceName;
  }

  // getters
  bool ifNewMessage() {
    boost::lock_guard<boost::mutex> guard(m_mtx);
    return !m_messageQueue.empty();
  }
  std::pair<int, std::vector<char>> degueueMessage() {
    boost::lock_guard<boost::mutex> guard(m_mtx);
    std::pair<int, std::vector<char>> result = m_messageQueue.front();
    m_messageQueue.pop();
    return result;
  }

  std::vector<unsigned char> *getFrameRef() { return &m_imageBytes; }

  int getWidth() {
    boost::lock_guard<boost::mutex> guard(m_mtx);
    return WIDTH;
  }
  int getHeight() {
    boost::lock_guard<boost::mutex> guard(m_mtx);
    return HEIGHT;
  }

private:
  AlManager *m_manager;
  AlSDKCb *m_sdkCb;
  boost::thread *m_internalThread;

  // signals
  boost::signals2::signal<void()> initPeerConnectionSignal;

  bool m_debug;

  boost::mutex m_mtx;
  std::vector<unsigned char> m_imageBytes;

  std::queue<std::pair<int, std::vector<char>>> m_messageQueue;

  std::string m_videoDeviceName;

  int WIDTH;
  int HEIGHT;
};

extern "C" BOOST_SYMBOL_EXPORT AlSdkPlugin plugin;
AlSdkPlugin plugin;

#endif // ALSDKPLUGIN_H
