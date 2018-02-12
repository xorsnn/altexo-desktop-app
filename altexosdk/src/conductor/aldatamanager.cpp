#include "aldatamanager.h"

AlDataManager::AlDataManager() : m_alVideoCapturer(NULL), m_debug(true) {
  m_desiredVideoSource = AlSdkAPI::DesiredVideoSource::CAMERA;
}

void AlDataManager::setDesiredDataSource(
    AlSdkAPI::DesiredVideoSource dataSource) {
  if (m_debug) {
  }
  m_desiredVideoSource = dataSource;
}

void AlDataManager::startVideoCapturer(AlVideoCapturer *alVideoCapturer) {
  m_alVideoCapturer = alVideoCapturer;
}

void AlDataManager::setImageData(uint8_t *pImageBytes, size_t len, int width,
                                 int height) {
  if (m_alVideoCapturer) {
    m_alVideoCapturer->setImageData(pImageBytes, len, width, height);
  }
}
