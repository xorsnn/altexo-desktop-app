#ifndef AL_WS_CB_H
#define AL_WS_CB_H

#include <string>
#include <vector>

class AlWsCb {
public:
  virtual ~AlWsCb() {}

  // TODO: for some reason std::string doesn't work
  virtual void onMessageCb(std::vector<char> peerId, std::vector<char> msg) = 0;
  virtual void onWsMessageCb(std::vector<char> msg) = 0;
};
//]

#endif // AL_WS_CB_H
