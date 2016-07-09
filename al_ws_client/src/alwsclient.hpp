#ifndef ALWSCLIENT_H
#define ALWSCLIENT_H

#include "boost/thread.hpp"
#include <boost/signals2/signal.hpp>
#include <iostream>
#include <libwebsockets.h>

// void usage() {
//   fprintf(stderr, "Usage: libwebsockets-test-client "
// 	  "<server address> [--port=<p>] "
// 	  "[--ssl] [-k] [-v <ver>] "
// 	  "[-d <log bitfield>] [-l]\n");
// }
class WsCb {
public:
  virtual ~WsCb() {}
  virtual void onWsMessageCb(std::vector<char> msg) = 0;
};

class AlWsClient {
public:
  AlWsClient();
  ~AlWsClient();

  int init(std::string path, int port, WsCb *cb) {
    m_path = path;
    m_port = port;
    m_cb = cb;
    newMessageSignal.connect(boost::bind(&WsCb::onWsMessageCb, cb, _1));
    return 1;
  }

  int run();

  // Callbacks
  int cbDumbIncrement(struct lws *wsi, enum lws_callback_reasons reason,
                      void *user, void *in, size_t len);
  // int cbLwsMirror(struct lws *wsi, enum lws_callback_reasons reason,
  // 	    void *user, void *in, size_t len);

private:
  int threadMain();
  int m_useSsl = 2;  /* 2 = allow selfsigned */
  int m_port = 8888; // TODO: move to parameters
  int m_ietfVersion = -1;
  int m_denyDeflate = 1;
  std::string m_path;

  // list of supported protocols and callbacks
  struct lws_protocols m_protocols[2];
  boost::thread m_internalThread;
  WsCb *m_cb;
  boost::signals2::signal<void(std::vector<char>)> newMessageSignal;
};

#endif // ALWsCLIENT_H
