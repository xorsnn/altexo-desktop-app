#ifndef CUSTOMSOCKETSERVER_H
#define CUSTOMSOCKETSERVER_H
#include "alcallback.h"
#include "conductor/conductor.h"
#include "webrtc/base/physicalsocketserver.h"
#include <iostream>

class CustomSocketServer : public rtc::PhysicalSocketServer {
public:
  CustomSocketServer(rtc::Thread *thread)
      : m_thread(thread), m_conductor(NULL), m_alCallback(NULL), m_debug(true),
        width(0), height(0) {}
  virtual ~CustomSocketServer() {}

  void set_conductor(rtc::scoped_refptr<Conductor> conductor) {
    m_conductor = conductor;
  }
  void setAlCallback(AlCallback *alCallback) { m_alCallback = alCallback; }

  // TODO - introduce callback!!!
  // Override so that we can also pump the GTK message loop.
  virtual bool Wait(int cms, bool process_io) {
    // std::cout << "ping!" << std::endl;

    // Pump GTK events.
    // TODO(henrike): We really should move either the socket server or UI to a
    // different thread.  Alternatively we could look at merging the two loops
    // by implementing a dispatcher for the socket server and/or use
    // g_main_context_set_poll_func.
    // TODO make a callback for this
    //        qtApp_->processEvents();
    // TODO false is temporary solution
    // if (m_alCallback) {
    //   m_alCallback->processUiEventsCb();
    // }
    // if (false && !m_conductor->connection_active()) {
    //   m_thread->Quit();
    // }

    if (m_alCallback->ifNewMessage()) {
      std::pair<int, std::vector<char>> msg = m_alCallback->degueueMessage();
      int msgType = msg.first;
      switch (msgType) {
      case AlCallback::SdkMessageType::INIT_SM: {
        if (m_conductor->InitializePeerConnection()) {
          if (m_debug) {
            std::cout << "CustomSocketServer CreateOffer" << std::endl;
          }
          m_conductor->m_peerConnection->CreateOffer(m_conductor, NULL);
        } else {
          std::cout << "Error. Failed to initialize PeerConnection"
                    << std::endl;
        }
      } break;
      case AlCallback::SdkMessageType::SDP_SM: {
        if (m_debug) {
          std::cout << "AlCallback::SdkMessageType::SDP_SM" << std::endl;
        }
        m_conductor->OnMessageFromPeer("", msg.second);
      } break;
      case AlCallback::SdkMessageType::CANDIDATE_SM: {
        m_conductor->OnMessageFromPeer("", msg.second);
      } break;
      case AlCallback::SdkMessageType::NEW_FRAME_SM: {
        m_conductor->m_dataManager->m_alVideoCapturer->setImageData(
            *(m_alCallback->getFrameRef()), width, height);
      } break;
      case AlCallback::SdkMessageType::UPDATE_RESOLUTION_SM: {
        width = m_alCallback->getWidth();
        height = m_alCallback->getHeight();
      } break;
      }
    }

    return rtc::PhysicalSocketServer::Wait(cms == -1 ? 1 : cms, process_io);
  }

  Conductor *getConductor() { return m_conductor; }

protected:
  rtc::Thread *m_thread;
  rtc::scoped_refptr<Conductor> m_conductor;
  AlCallback *m_alCallback;
  bool m_debug;
  int width;
  int height;
};

#endif // CUSTOMSOCKETSERVER_H
