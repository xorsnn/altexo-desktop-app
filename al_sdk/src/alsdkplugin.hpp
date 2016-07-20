#ifndef ALSDKPLUGIN_H
#define ALSDKPLUGIN_H

#include "AL_API/sdk_api.hpp"
#include "AL_CB/al_sdk_cb.hpp"
#include "alcallback.h"
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

  // void DeletePeerConnection();
  //
  // // TODO: union the two methods
  // void OnMessageFromPeer(std::string peer_id, const std::string &message);
  // AlWsCb
  // void onMessageFromPeer(std::vector<char> peerId, std::vector<char>
  // message);
  // void onWsMessageCb(std::vector<char> msg) {}

  // bool isClientConnected();
  // void clientConnect(const std::string &server, int port,
  //                    const std::string &client_name);
  // void clientSignOut();
  void setDesiredDataSource(int dataSource);
  // void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);
  void threadMain();

  // AlCallback implementation
  // void sendToPeerCb(const std::string &message) {
  //   if (m_debug) {
  //     std::cout << "sendToPeerCb" << std::endl;
  //   }
  //   std::vector<char> msg(message.begin(), message.end());
  //   m_sdkCb->sendMessageToPeer(msg);
  // }

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
  void onDevicesListChangedCb(std::vector<std::string> device_names) {
    if (m_debug) {
      std::cout << "onDevicesListChangedCb" << std::endl;
    }
  }
  void updateFrameCb(const uint8_t *image, int width, int height) {
    if (m_debug) {
      // std::cout << "updateFrameCb" << std::endl;
    }
  }
  //    TODO move to AlManager
  std::string getVideoDeviceName() { return ""; }

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

private:
  AlManager *m_manager;
  AlSDKCb *m_sdkCb;
  boost::thread m_internalThread;

  // signals
  boost::signals2::signal<void()> initPeerConnectionSignal;

  bool m_debug;

  boost::mutex m_mtx;

  std::queue<std::pair<int, std::vector<char>>> m_messageQueue;
  uint8_t *m_pImageBytes;
};

extern "C" BOOST_SYMBOL_EXPORT AlSdkPlugin plugin;
AlSdkPlugin plugin;

#endif // ALSDKPLUGIN_H
