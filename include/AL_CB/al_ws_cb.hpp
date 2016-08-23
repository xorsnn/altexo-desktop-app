#ifndef AL_WS_CB_H
#define AL_WS_CB_H

#include "altextmessage.hpp"
#include <string>
#include <vector>

class AlWsCb {
public:
  virtual void onWsMessageCb(AlTextMessage msg) = 0;
  virtual void onIceCandidateCb(AlTextMessage msg) = 0;
  virtual void onSdpCb(AlTextMessage msg) = 0;
};
//]

#endif // AL_WS_CB_H
