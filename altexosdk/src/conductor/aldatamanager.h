#ifndef ALDATAMANAGER_H
#define ALDATAMANAGER_H

#include "AL_API/sdk_api.hpp"
#include "altexosdk/src/alvideocapturer.h"
#include "altexosdk/src/alvideorenderer.h"
#include "api/mediastreaminterface.h"
#include "api/peerconnectioninterface.h"

class AlDataManager {
public:
  AlDataManager();
  void setDesiredDataSource(AlSdkAPI::DesiredVideoSource dataSource);
  void startVideoCapturer(AlVideoCapturer *alVideoCapturer);
  void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);
  AlVideoCapturer *m_alVideoCapturer;

private:
  std::deque<std::string *> m_pendingMessages;
  bool m_debug;

public:
  int m_desiredVideoSource;
};

#endif // ALDATAMANAGER_H
