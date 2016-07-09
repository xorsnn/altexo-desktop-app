#ifndef ALWSAPI_H
#define ALWSAPI_H

#include "AL_CB/al_ws_cb.hpp"
#include <string>

class AlWsAPI {
public:
  virtual ~AlWsAPI() {}
  virtual void init(AlWsCb *alWsCb) = 0;
};
//]

#endif // ALWSAPI_H
