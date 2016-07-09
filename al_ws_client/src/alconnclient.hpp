#ifndef ALCONNCLIENT_H
#define ALCONNCLIENT_H

#include <cpr/cpr.h>
#include <iostream>
#include <vector>

#include "alwsclient.hpp"
#include "boost/thread.hpp"

class AlConnClient : public WsCb {
public:
  AlConnClient();
  ~AlConnClient();

  enum HTTP_RESPONSE_TYPE {
    HTTP_LOGIN = 1,
    HTTP_ME,
    HTTP_INIT_WS_CONN,
  };

  void login(std::string login, std::string password);
  void onWsMessageCb(std::vector<char> msg);

private:
  void handleHttpResponse(cpr::Response r, int responseType);
  // void threadMain();

  std::string m_token;
  std::string m_wssLink;
  std::string m_peerId;

  AlWsClient m_WsCl;
  boost::thread m_internalThread;
};

#endif // ALCONNCLIENT_H
