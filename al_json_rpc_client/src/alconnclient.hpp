#ifndef ALCONNCLIENT_H
#define ALCONNCLIENT_H

#include <cpr/cpr.h>
#include <iostream>
#include <vector>

#include "AL_CB/al_ws_cb.hpp"
#include "alrpc.hpp"
#include "boost/thread.hpp"

class AlConnClient {
public:
  AlConnClient(AlWsCb *alWsCb);
  ~AlConnClient();

  enum HTTP_RESPONSE_TYPE {
    HTTP_LOGIN = 1,
    HTTP_ME,
  };

  void initWsConnection();

  AlRpc *getWsClientRef() { return &m_wsCl; };

private:
  bool m_debug;

  std::string m_token;
  std::string m_wssLink;

  AlRpc m_wsCl;
  AlWsCb *m_alWsCb;
};

#endif // ALCONNCLIENT_H
