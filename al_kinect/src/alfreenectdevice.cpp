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
      // m_bufferDepth_new(480 * 640 * 3),
      m_newRgbFrame(false), m_newDepthFrame(false) {
  setDepthFormat(FREENECT_DEPTH_REGISTERED);
  this->updateSettings();
}

int ALFreenectDevice::getMaxDepth() { return this->maxDepth; }

void ALFreenectDevice::setMaxDepth(int newDepth) { this->maxDepth = newDepth; }

void ALFreenectDevice::VideoCallback(void *_rgb, uint32_t timestamp) {
  Mutex::ScopedLock lock(m_rgbMutex);
  uint8_t *rgb = static_cast<uint8_t *>(_rgb);
  if (!m_newRgbFrame) {
    std::copy(rgb, rgb + getVideoBufferSize(), m_bufferVideo.begin());
    m_newRgbFrame = true;
  }
}

float ALFreenectDevice::ofMap(float value, float inputMin, float inputMax,
                              float outputMin, float outputMax, bool clamp) {

  if (fabs(inputMin - inputMax) < FLT_EPSILON) {
    std::cout << "ofMap(): avoiding possible divide by zero, check inputMin "
                 "and inputMax: "
              << std::endl;
    std::cout << inputMin << std::endl;
    std::cout << inputMax << std::endl;
    return outputMin;
  } else {
    float outVal =
        ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) +
         outputMin);

    if (clamp) {
      if (outputMax < outputMin) {
        if (outVal < outputMax)
          outVal = outputMax;
        else if (outVal > outputMin)
          outVal = outputMin;
      } else {
        if (outVal > outputMax)
          outVal = outputMax;
        else if (outVal < outputMin)
          outVal = outputMin;
      }
    }
    return outVal;
  }
}

ALColor ALFreenectDevice::huePixelForDepth(uint16_t pix) {
  ALColor color;
  color.set(0, 255);
  if (pix == 0 || pix < this->minDepth || pix > this->maxDepth) {
    return color;
  }
  float alignedPix = ALFreenectDevice::ofMap(pix, this->minDepth,
                                             this->maxDepth, 0, 255, true);

  color.setHsb(alignedPix, 255, 255, 255);
  return color;
}

void ALFreenectDevice::DepthCallback(void *_depth, uint32_t timestamp) {
  // std::cout << "ALFreenectDevice::DepthCallback" << std::endl;
  Mutex::ScopedLock lock(m_depthMutex);
  uint16_t *depth = static_cast<uint16_t *>(_depth);
  // TODO: we can make it on GPU
  // but for now we will do it in a separate thread
  // but it seems also not to be a bottleneck
  // for (unsigned int i = 0; i < 640 * 480; i++) {
  //   ALColor color = this->huePixelForDepth(m_depth[i]);
  //   m_bufferDepth[3 * i + 0] = (int)floorf(color.r);
  //   m_bufferDepth[3 * i + 1] = (int)floorf(color.g);
  //   m_bufferDepth[3 * i + 2] = (int)floorf(color.b);
  // }
  // std::copy(depth, depth + getDepthBufferSize(), m_bufferDepth.begin());
  // std::cout << m_bufferDepth.size() << std::endl;
  // std::cout << getDepthBufferSize() << std::endl;
  m_newDepthFrame = true;
}

bool ALFreenectDevice::getRGB(std::vector<uint8_t> &buffer) {
  Mutex::ScopedLock lock(m_rgbMutex);
  if (!m_newRgbFrame) {
    return false;
  }
  buffer.swap(m_bufferVideo);
  m_newRgbFrame = false;
  // return m_bufferVideo;
  return true;
}

std::vector<uint16_t> ALFreenectDevice::getDepth() {
  Mutex::ScopedLock lock(m_depthMutex);
  // if (!m_newDepthFrame)
  // return false;
  // buffer.swap(m_bufferDepth);
  m_newDepthFrame = false;
  return m_bufferDepth;
}

void ALFreenectDevice::updateSettings() {
  // TODO: use some other setting storage
  this->minDepth = 555;
  this->maxDepth = 1005;
  // QSettings settings;
  // this->minDepth = settings.value("altexo/minDepth", 555).toInt();
  // this->maxDepth = settings.value("altexo/maxDepth", 1005).toInt();
}
