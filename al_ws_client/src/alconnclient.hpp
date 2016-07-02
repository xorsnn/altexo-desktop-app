#ifndef ALCONNCLIENT_H
#define ALCONNCLIENT_H

#include <iostream>
#include <cpr/cpr.h>

#include "alwsclient.hpp"

class AlConnClient
{
public:
  AlConnClient();
  ~AlConnClient();

  enum HTTP_RESPONSE_TYPE {
    HTTP_LOGIN = 1,
    HTTP_ME,
    HTTP_INIT_WS_CONN,
  };

  void login(std::string login, std::string password);

private:
  void handleHttpResponse(cpr::Response r, int responseType);

  std::string m_token;
  std::string m_wssLink;

	AlWsClient m_WsCl;
};

#endif // ALCONNCLIENT_H
