#ifndef CUSTOMSOCKETSERVER_H
#define CUSTOMSOCKETSERVER_H

#include "alconductor.hpp"
#include "alsdkcallback.hpp"
#include "webrtc/base/physicalsocketserver.h"
#include <iostream>

class CustomSocketServer : public rtc::PhysicalSocketServer {
public:
  CustomSocketServer(rtc::Thread *thread)
      : m_thread(thread), m_conductor(NULL), m_alSdkCallback(NULL),
        m_debug(true), width(0), height(0) {

    f_initConnection = false;
  }

  virtual ~CustomSocketServer() {}

  void set_conductor(rtc::scoped_refptr<AlConductor> conductor) {
    m_conductor = conductor;
  }

  void setAlCallback(AlSdkCallback *alCallback) {
    m_alSdkCallback = alCallback;
  }

  // TODO - introduce callback!!!
  virtual bool Wait(int cms, bool process_io) {
    if (!f_initConnection) {
      alLogger() << "ping!";
      f_initConnection = true;
      m_conductor->InitializePeerConnection();
    }

    // if (m_alCallback->ifNewMessage()) {
    //   std::pair<int, std::vector<char>> msg = m_alCallback->degueueMessage();
    //   int msgType = msg.first;
    //   switch (msgType) {
    //   case AlCallback::SdkMessageType::INIT_SM: {
    //     std::cout << "AlCallback::SdkMessageType::INIT_SM" << std::endl;
    //     if (m_conductor->InitializePeerConnection()) {
    //       if (m_debug) {
    //         std::cout << "CustomSocketServer CreateOffer" << std::endl;
    //       }
    //       m_conductor->m_peerConnection->CreateOffer(m_conductor, NULL);
    //     } else {
    //       std::cout << "Error. Failed to initialize PeerConnection"
    //                 << std::endl;
    //     }
    //   } break;
    //   case AlCallback::SdkMessageType::SDP_SM: {
    //     if (m_debug) {
    //       std::cout << "AlCallback::SdkMessageType::SDP_SM" << std::endl;
    //     }
    //     m_conductor->OnMessageFromPeer("", msg.second);
    //   } break;
    //   case AlCallback::SdkMessageType::CANDIDATE_SM: {
    //     m_conductor->OnMessageFromPeer("", msg.second);
    //   } break;
    //   case AlCallback::SdkMessageType::NEW_FRAME_SM: {
    //     m_conductor->m_dataManager->m_alVideoCapturer->setImageData(
    //         *(m_alCallback->getFrameRef()), width, height);
    //   } break;
    //   case AlCallback::SdkMessageType::UPDATE_RESOLUTION_SM: {
    //     width = m_alCallback->getWidth();
    //     height = m_alCallback->getHeight();
    //   } break;
    //   case AlCallback::SdkMessageType::QUIT_SM: {
    //     if (m_debug) {
    //       std::cout << "AlCallback::SdkMessageType::QUIT_SM" << std::endl;
    //     }
    //     m_thread->Quit();
    //   } break;
    //   default: {
    //     // unhandled
    //   }
    //   }
    // }

    return rtc::PhysicalSocketServer::Wait(cms == -1 ? 1 : cms, process_io);
  }

  AlConductor *getConductor() { return m_conductor; }

protected:
  rtc::Thread *m_thread;
  rtc::scoped_refptr<AlConductor> m_conductor;
  AlSdkCallback *m_alSdkCallback;
  bool m_debug;
  int width;
  int height;

  // NOTE: FLAGS
  bool f_initConnection;
};

#endif // CUSTOMSOCKETSERVER_H
