#include "alwsclient.hpp"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define random rand
#include "gettimeofday.h"
#else
#include <sys/time.h>
#include <syslog.h>
#include <unistd.h>
#endif

static struct lws *wsi_dumb;
static volatile int force_exit;

static AlWsClient *me;

static int cbDumbIncrementStatic(struct lws *wsi,
                                 enum lws_callback_reasons reason, void *user,
                                 void *in, size_t len) {
  return me->cbDumbIncrement(wsi, reason, user, in, len);
};

/*
* This demo shows how to connect multiple websockets simultaneously to a
* websocket server (there is no restriction on their having to be the same
* server just it simplifies the demo).
*
*  dumb-increment-protocol:  we connect to the server and print the number
*				we are given
*
*  lws-mirror-protocol: draws random circles, which are mirrored on to every
*				client (see them being drawn in every browser
*				session also using the test server)
*/

enum demo_protocols {

  PROTOCOL_DUMB_INCREMENT,
  PROTOCOL_LWS_MIRROR,

  /* always last */
  DEMO_PROTOCOL_COUNT
};

static const struct lws_extension exts[] = {
    {"permessage-deflate", lws_extension_callback_pm_deflate,
     "permessage-deflate; client_max_window_bits"},
    {"deflate-frame", lws_extension_callback_pm_deflate, "deflate_frame"},
    {NULL, NULL, NULL /* terminator */}};

void sighandler(int sig) { force_exit = 1; }

static int ratelimit_connects(unsigned int *last, unsigned int secs) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  if (tv.tv_sec - (*last) < secs)
    return 0;

  *last = tv.tv_sec;

  return 1;
}

AlWsClient::AlWsClient()
    : m_writable(NULL), m_internalThread(NULL), m_debug(true) {
  me = this;
  // lws_protocols pr1 = {
  //     "dumb-increment-protocol,fake-nonexistant-protocol",
  //     // boost::bind(&AlWsClient::cbDumbIncrement, this, _1, _2, _3, _4, _5),
  //     cbDumbIncrementStatic, 0,
  //     4096, // 20 was before
  // };
  lws_protocols pr1 = {"default", cbDumbIncrementStatic, 0};
  m_protocols[0] = pr1;

  lws_protocols pr3 = {NULL, NULL, 0};
  m_protocols[1] = pr3; /* end */
}

AlWsClient::~AlWsClient() {
  if (m_debug) {
    std::cout << "AlWsClient::~AlWsClient 1" << std::endl;
  }
  if (m_internalThread != NULL) {
    std::cout << "AlWsClient::~AlWsClient 2" << std::endl;
    m_internalThread->interrupt();
    std::cout << "AlWsClient::~AlWsClient 3" << std::endl;
    if (m_internalThread->joinable()) {
      std::cout << "AlWsClient::~AlWsClient 3.1" << std::endl;
      m_internalThread->join();
    }
    std::cout << "AlWsClient::~AlWsClient 4" << std::endl;
    delete m_internalThread;
    std::cout << "AlWsClient::~AlWsClient 5" << std::endl;
    m_internalThread = NULL;
  }
  if (m_debug) {
    std::cout << "AlWsClient::~AlWsClient 2" << std::endl;
  }
}

/*
* dumb_increment protocol
*
* since this also happens to be protocols[0], some callbacks that are not
* bound to a specific protocol also turn up here.
*/
int AlWsClient::cbDumbIncrement(struct lws *wsi,
                                enum lws_callback_reasons reason, void *user,
                                void *in, size_t len) {
  unsigned char buf[LWS_PRE + 4096];

  if (m_debug) {
    std::cout << "reason: " << reason << std::endl;
  }

  switch (reason) {

  case LWS_CALLBACK_CLIENT_ESTABLISHED:
    lwsl_info("dumb: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
    break;

  case LWS_CALLBACK_CLOSED:
    lwsl_notice("dumb: LWS_CALLBACK_CLOSED\n");
    wsi_dumb = NULL;
    break;

  case LWS_CALLBACK_CLIENT_RECEIVE: {
    ((char *)in)[len] = '\0';
    std::string msg((char *)in);
    _onMessageReceived(AlTextMessage(msg));
  } break;

  case LWS_CALLBACK_CLIENT_WRITEABLE: {
    if (m_debug) {
      std::cout << "LWS_CALLBACK_CLIENT_WRITEABLE" << std::endl;
    }

    if (!m_messageQueue.empty()) {
      AlTextMessage msg = m_messageQueue.front();
      m_messageQueue.pop();

      // std::string peerIdStr = msgPair.first;
      // std::string msgStr = msgPair.second;

      std::string msgStr = msg.toString();
      std::copy(std::begin(msgStr), std::end(msgStr), &(buf[LWS_PRE]));
      buf[LWS_PRE + msgStr.size()] = '\0';

      int n = lws_write(wsi, &buf[LWS_PRE], msgStr.size(), LWS_WRITE_TEXT);
      if (m_debug) {
        std::cout << "+++++++++++++++" << std::endl;
        std::cout << msgStr << std::endl;
        std::cout << n << std::endl;
        std::cout << "+++++++++++++++" << std::endl;
      }
    }
  } break;

  /* because we are protocols[0] ... */

  case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {
    if (wsi == wsi_dumb) {
      lwsl_err("dumb: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
      wsi_dumb = NULL;
    }
  } break;

  case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED: {
    if ((strcmp((char *)in, "deflate-stream") == 0) && m_denyDeflate) {
      lwsl_notice("denied deflate-stream extension\n");
      return 1;
    }
    if ((strcmp((char *)in, "x-webkit-deflate-frame") == 0))
      return 1;
    if ((strcmp((char *)in, "deflate-frame") == 0))
      return 1;
  } break;

  default:
    break;
  }

  return 0;
}

int AlWsClient::run() {
  m_internalThread = new boost::thread(&AlWsClient::threadMain, this);
  return 1;
}

int AlWsClient::threadMain() {

  memset(&info, 0, sizeof info);

  lwsl_notice("libwebsockets test client - license LGPL2.1+SLE\n");
  lwsl_notice("(C) Copyright 2010-2016 Andy Green <andy@warmcat.com>\n");

  signal(SIGINT, sighandler);

  memset(&m_i, 0, sizeof(m_i));

  // TODO: remove
  m_i.port = m_port;

  char *pathTmp = (char *)(m_path.c_str());
  if (lws_parse_uri(pathTmp, &prot, &m_i.address, &m_i.port, &p)) {
    if (m_debug) {
      std::cout << "PARCE!" << std::endl;
    }
    // usage();
  }

  /* add back the leading / on path */
  path[0] = '/';
  strncpy(path + 1, p, sizeof(path) - 2);
  path[sizeof(path) - 1] = '\0';
  m_i.path = path;

  if (!strcmp(prot, "http") || !strcmp(prot, "ws")) {
    m_useSsl = 0;
  }
  if (!strcmp(prot, "https") || !strcmp(prot, "wss")) {
    m_useSsl = 1;
  }

  /*
  * create the websockets context.  This tracks open connections and
  * knows how to route any traffic and which protocol version to use,
  * and if each connection is client or server side.
  *
  * For this client-only demo, we tell it to not listen on any port.
  */

  info.port = CONTEXT_PORT_NO_LISTEN;
  info.protocols = m_protocols;
  info.gid = -1;
  info.uid = -1;

  context = lws_create_context(&info);
  if (context == NULL) {
    fprintf(stderr, "Creating libwebsocket context failed\n");
    return 1;
  }

  m_i.context = context;
  m_i.ssl_connection = m_useSsl;
  m_i.host = m_i.address;
  m_i.origin = m_i.address;
  m_i.ietf_version_or_minus_one = m_ietfVersion;
  m_i.client_exts = exts;
  /*
  * sit there servicing the websocket context to handle incoming
  * packets, and drawing random circles on the mirror protocol websocket
  *
  * nothing happens until the client websocket connection is
  * asynchronously established... calling lws_client_connect() only
  * instantiates the connection logically, lws_service() progresses it
  * asynchronously.
  */

  while (!force_exit) {

    if (!wsi_dumb && ratelimit_connects(&rl_dumb, 2u)) {
      lwsl_notice("dumb: connecting\n");
      m_i.protocol = m_protocols[PROTOCOL_DUMB_INCREMENT].name;
      wsi_dumb = lws_client_connect_via_info(&m_i);
    }

    // TODO: dirty solution
    if (!m_messageQueue.empty()) {
      lws_callback_on_writable(wsi_dumb);
    }

    lws_service(context, 500);
  }

  lwsl_err("Exiting\n");
  lws_context_destroy(context);

  return 1;
}

void AlWsClient::sendMessage(AlTextMessage msg) {
  if (m_debug) {
    std::cout << "AlWsClient::sendMessage" << std::endl;
  }
  // std::pair<std::string, std::string> msgPair("123", "123");

  // **
  // * composing message to send
  // *
  // std::ostringstream stream;
  // boost::property_tree::ptree pt;
  // boost::property_tree::ptree dataNode;
  // pt.put("action", "send_to_peer");
  // dataNode.put("peer_id", peerIdStr);
  // dataNode.put("message", msgStr);
  // pt.add_child("data", dataNode);
  // boost::property_tree::write_json(stream, pt, false);
  // std::string strJson = stream.str();

  m_messageQueue.push(msg);
}

void AlWsClient::_onMessageReceived(AlTextMessage msg) {
  if (m_debug) {
    std::cout << "AlWsClient::_onMessageReceived" << std::endl;
  }
  if (m_debug) {
    std::cout << msg.toString() << std::endl;
  }

  if (!m_tmpFlag) {
    // TODO: just for testing, replace
    //===============================
    std::ostringstream stream1;
    boost::property_tree::ptree ptAuth1;
    // boost::property_tree::ptree dataNode;
    // authenticate [ token ] -> boolean
    ptAuth1.put("jsonrpc", "2.0");
    ptAuth1.put("method", "room/open");
    ptAuth1.put("params", "[\"test111\", \"true\"]");
    ptAuth1.put("id", "3");
    boost::property_tree::write_json(stream1, ptAuth1, false);
    std::string strJson1 = stream1.str();
    AlTextMessage msgToSend1(strJson1);
    m_messageQueue.push(msgToSend1);
    //===============================
    m_tmpFlag = true;
  }
  // newMessageSignal(msgVec);
}
