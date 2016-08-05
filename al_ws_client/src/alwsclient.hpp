#ifndef ALWSCLIENT_H
#define ALWSCLIENT_H

#include "AL_CB/al_ws_cb.hpp"
#include "boost/thread.hpp"
#include <boost/signals2/signal.hpp>
#include <iostream>
#include <libwebsockets.h>
#include <queue>
#include <utility>

// void usage() {
//   fprintf(stderr, "Usage: libwebsockets-test-client "
// 	  "<server address> [--port=<p>] "
// 	  "[--ssl] [-k] [-v <ver>] "
// 	  "[-d <log bitfield>] [-l]\n");
// }

class AlWsClient {
public:
  AlWsClient();
  ~AlWsClient();

  int init(std::string path, int port, AlWsCb *cb) {
    m_path = path;
    m_port = port;
    m_cb = cb;
    newMessageSignal.connect(boost::bind(&AlWsCb::onWsMessageCb, cb, _1));
    return 1;
  }

  int run();

  // Callbacks
  int cbDumbIncrement(struct lws *wsi, enum lws_callback_reasons reason,
                      void *user, void *in, size_t len);

  void sendMessageToPeer(std::vector<char> peerId, std::vector<char> msg);

private:
  int threadMain();
  int m_useSsl = 2;  /* 2 = allow selfsigned */
  int m_port = 8888; // TODO: move to parameters
  int m_ietfVersion = -1;
  int m_denyDeflate = 1;
  std::string m_path;

  unsigned int rl_dumb = 0;
  struct lws_context_creation_info info;
  struct lws_client_connect_info m_i;
  struct lws_context *context;
  const char *prot, *p;
  char path[300];
  // list of supported protocols and callbacks
  struct lws_protocols m_protocols[2];
  boost::thread m_internalThread;
  AlWsCb *m_cb;
  boost::signals2::signal<void(std::vector<char>)> newMessageSignal;

  std::queue<std::pair<std::string, std::string>> m_messageQueue;
  char *m_writable;

  bool m_debug;
};

#endif // ALWsCLIENT_H
