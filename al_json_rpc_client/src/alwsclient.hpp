#ifndef ALWSCLIENT_H
#define ALWSCLIENT_H

#include "AL_CB/al_ws_cb.hpp"
#include "altextmessage.hpp"
#include "boost/thread.hpp"
#include <boost/signals2/signal.hpp>
#include <iostream>
#include <libwebsockets.h>
#include <queue>
#include <utility>

// supposed to be just a transport
class AlWsClient {
public:
  AlWsClient();
  ~AlWsClient();

  int init(std::string path, AlWsCb *cb) {
    m_path = path;
    m_cb = cb;
    iceCandidateSignal.connect(boost::bind(&AlWsCb::onIceCandidateCb, cb, _1));
    sdpOfferSignal.connect(boost::bind(&AlWsCb::onSdpOfferCb, cb, _1));
    sdpAnswerSignal.connect(boost::bind(&AlWsCb::onSdpAnswerCb, cb, _1));
    initCallSignal.connect(boost::bind(&AlWsCb::onInitCall, cb));
    return 1;
  }

  int run();

  // Callbacks
  int cbDumbIncrement(struct lws *wsi, enum lws_callback_reasons reason,
                      void *user, void *in, size_t len);

  void sendMessageToPeer(std::vector<char> peerId, std::vector<char> msg) {}
  void sendMessage(AlTextMessage msg);

  // kind of parent implementation
  virtual void onMessage(AlTextMessage msg) = 0;

protected:
  boost::signals2::signal<void(AlTextMessage)> iceCandidateSignal;
  boost::signals2::signal<void(const char *)> sdpOfferSignal;
  boost::signals2::signal<void(AlTextMessage)> sdpAnswerSignal;
  boost::signals2::signal<void()> initCallSignal;

private:
  int threadMain();
  void _onMessageReceived(AlTextMessage msg);

  int m_useSsl = 2; /* 2 = allow selfsigned */
  // int m_port = 8888; // TODO: move to parameters
  int m_ietfVersion = -1;
  int m_denyDeflate = 1;
  std::string m_path;

  unsigned int rl_dumb = 0;
  struct lws_context_creation_info info;
  struct lws_client_connect_info m_i;
  struct lws_context *m_context;
  const char *prot, *p;
  char path[300];
  // list of supported protocols and callbacks
  struct lws_protocols m_protocols[2];
  boost::thread *m_internalThread;
  AlWsCb *m_cb;

  // std::queue<std::pair<std::string, std::string>> m_messageQueue;
  std::queue<AlTextMessage> m_messageQueue;
  // char *m_writable;

  bool m_debug;
  // bool m_tmpFlag = false;
};

#endif // ALWsCLIENT_H
