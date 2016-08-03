#ifndef ALDATAMANAGER_H
#define ALDATAMANAGER_H

#include "alvideocapturer.h"
#include "alvideorenderer.h"
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"

class AlDataManager {
public:
  AlDataManager();
  void setDesiredDataSource(int dataSource);
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
