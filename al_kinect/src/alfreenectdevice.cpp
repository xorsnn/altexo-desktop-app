#include "alfreenectdevice.h"

ALFreenectDevice::ALFreenectDevice(freenect_context *_ctx, int _index)
    : Freenect::FreenectDevice(_ctx, _index),
      m_bufferDepth(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM,
                                             FREENECT_DEPTH_REGISTERED)
                        .bytes /
                    2),
      m_bufferVideo(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM,
                                             FREENECT_VIDEO_RGB)
                        .bytes),
      m_newRgbFrame(false), m_newDepthFrame(false) {
  setDepthFormat(FREENECT_DEPTH_REGISTERED);
}

void ALFreenectDevice::VideoCallback(void *_rgb, uint32_t timestamp) {
  Mutex::ScopedLock lock(m_rgbMutex);
  uint8_t *rgb = static_cast<uint8_t *>(_rgb);
  std::copy(rgb, rgb + getVideoBufferSize(), m_bufferVideo.begin());
  m_newRgbFrame = true;
}

void ALFreenectDevice::DepthCallback(void *_depth, uint32_t timestamp) {
  Mutex::ScopedLock lock(m_depthMutex);
  uint16_t *depth = static_cast<uint16_t *>(_depth);
  std::copy(depth, depth + getDepthBufferSize() / 2, m_bufferDepth.begin());
  m_newDepthFrame = true;
}

std::vector<uint8_t> ALFreenectDevice::getRGB() {
  Mutex::ScopedLock lock(m_rgbMutex);
  m_newRgbFrame = false;
  return m_bufferVideo;
}

std::vector<uint16_t> ALFreenectDevice::getDepth() {
  Mutex::ScopedLock lock(m_depthMutex);
  m_newDepthFrame = false;
  return m_bufferDepth;
}
