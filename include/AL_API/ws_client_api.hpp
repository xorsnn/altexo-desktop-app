#ifndef ALWSAPI_H
#define ALWSAPI_H

#include "AL_CB/al_ws_cb.hpp"
#include "altextmessage.hpp"
#include <string>

class AlWsAPI {
public:
  virtual ~AlWsAPI() {}
  virtual void init(AlWsCb *alWsCb) = 0;
  virtual void sendMessageToPeer(AlTextMessage peerId, AlTextMessage msg) = 0;
};
//]

#endif // ALWSAPI_H
