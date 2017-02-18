#ifndef AL_SDK_CB_H
#define AL_SDK_CB_H

#include "altextmessage.hpp"
#include <string>
#include <vector>

class AlSDKCb {
public:
  virtual ~AlSDKCb() {}
  virtual void onLocalSdpCb(AlTextMessage sdp) = 0;
  virtual void onLocalIceCandidateCb(AlTextMessage candidate) = 0;

  virtual void onNewCaptureDeciceCb(const char *deviceName) = 0;

  virtual void updateFrameCb(const uint8_t *image, int width, int height) = 0;
  virtual void updateLocalFrameCb(const uint8_t *image, int width,
                                  int height) = 0;
};
//]

#endif // AL_SDK_CB_H
