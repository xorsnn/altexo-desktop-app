
#ifndef ALSDKCALLBACK_H
#define ALSDKCALLBACK_H

#include "altextmessage.hpp"
#include <map>
#include <vector>

typedef std::map<int, std::string> Peers;

class AlSdkCallback {
public:
  enum SdkMessageType {
    SDP_SM = 1,
    CANDIDATE_SM,
    INIT_SM,
    NEW_FRAME_SM,
    UPDATE_RESOLUTION_SM,
    QUIT_SM,
  };

  virtual void onSdpCb(const std::string &message) = 0;
  virtual void onCandidateCb(const std::string &message) = 0;

  virtual void stopLocalRendererCb() = 0;
  virtual void stopRemoteRendererCb() = 0;

  virtual void onDevicesListChangedCb(std::vector<alMsg> device_names) = 0;

  virtual void updateFrameCb(const uint8_t *image, int width, int height) = 0;
  virtual void updateLocalFrameCb(const uint8_t *image, int width,
                                  int height) = 0;
  virtual std::vector<uint8_t> *getFrameRef() = 0;

  // TODO: move to AlManager
  virtual std::string getVideoDeviceName() = 0;

  virtual bool ifNewMessage() = 0;
  virtual std::pair<int, std::vector<char>> degueueMessage() = 0;
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
};

#endif // ALSDKCALLBACK_H
