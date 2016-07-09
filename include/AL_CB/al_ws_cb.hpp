#ifndef AL_WS_CB_H
#define AL_WS_CB_H

#include <string>
#include <vector>

class AlWsCb {
public:
  virtual ~AlWsCb() {}
  virtual void onMessageCb(std::string peer_id, const std::string &message) = 0;
};
//]

#endif // AL_WS_CB_H
