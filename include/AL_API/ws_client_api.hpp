#ifndef ALWSAPI_H
#define ALWSAPI_H

#include "AL_CB/al_ws_cb.hpp"
#include "altextmessage.hpp"
#include <string>

class AlWsAPI {
public:
  virtual ~AlWsAPI() {}
  virtual void init(AlWsCb *alWsCb) = 0;

  // TODO: deprecated
  virtual void sendMessageToPeer(AlTextMessage peerId, AlTextMessage msg) = 0;
  virtual void sendMessage(AlTextMessage msg) = 0;

  virtual void sendSdpAnswer(AlTextMessage msg) = 0;
  virtual void sendSdpOffer(const char *msg) = 0;
  virtual void sendIceCandidate(AlTextMessage msg) = 0;

  // json-rpc
  virtual void roomOpen(AlTextMessage msg) = 0;
};
//]

#endif // ALWSAPI_H
