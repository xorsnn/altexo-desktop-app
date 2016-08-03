#ifndef AL_SDK_CB_H
#define AL_SDK_CB_H

#include "altextmessage.hpp"
#include <string>
#include <vector>

class AlSDKCb {
public:
  virtual ~AlSDKCb() {}
  virtual void onSdp(std::vector<char> sdp) = 0;
  virtual void onCandidate(std::vector<char> candidate) = 0;
  virtual void
  onDevicesListChangedCb(std::vector<AlTextMessage> deviceNames) = 0;
  virtual void updateFrameCb(const uint8_t *image, int width, int height) = 0;
  virtual void updateLocalFrameCb(const uint8_t *image, int width,
                                  int height) = 0;
};
//]

#endif // AL_SDK_CB_H
