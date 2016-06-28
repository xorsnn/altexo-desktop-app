#ifndef ALWSCLIENT_H
#define ALWSCLIENT_H

#include <iostream>
#include <cpr/cpr.h>

class AlWsClient
{
public:
  AlWsClient();
  ~AlWsClient();

  enum HTTP_RESPONSE_TYPE {
    HTTP_LOGIN = 1,
    HTTP_ME,
    HTTP_INIT_WS_CONN,
  };

  void login(std::string login, std::string password);

private:
  void handleHttpResponse(cpr::Response r, int responseType);

  std::string m_token;
};

#endif // ALWSCLIENT_H
