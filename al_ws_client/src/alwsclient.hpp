#ifndef ALWSCLIENT_H
#define ALWSCLIENT_H

#include <iostream>
#include <libwebsockets.h>

// void usage() {
//   fprintf(stderr, "Usage: libwebsockets-test-client "
// 	  "<server address> [--port=<p>] "
// 	  "[--ssl] [-k] [-v <ver>] "
// 	  "[-d <log bitfield>] [-l]\n");
// }

class AlWsClient
{
public:
  AlWsClient();
  ~AlWsClient();

  int init(std::string path, int port) {
    m_path = path;
    m_port = port;
  }

  int run();

  // Callbacks
  int cbDumbIncrement(struct lws *wsi, enum lws_callback_reasons reason,
  			void *user, void *in, size_t len);
  // int cbLwsMirror(struct lws *wsi, enum lws_callback_reasons reason,
	// 	    void *user, void *in, size_t len);

private:
  int m_useSsl = 2; /* 2 = allow selfsigned */
  int m_port = 8888; // TODO: move to parameters
  int m_longlived = 1;
  int m_ietfVersion = -1;
  int m_denyDeflate = 1;
  int m_denyMux = 1;
  std::string m_path;

  // TODO: not sure what this is about
  int m_mirrorLifetime;


  // list of supported protocols and callbacks
  struct lws_protocols m_protocols[2];
};

#endif // ALWsCLIENT_H
