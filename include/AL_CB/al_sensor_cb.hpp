#ifndef AL_SENSOR_CB_H
#define AL_SENSOR_CB_H

#include <string>
#include <vector>

// #ifdef _WIN32
// #define uint unsigned int
// #endif

class AlSensorCb {
public:
  enum VideoType {
    RGB = 1,
    RGBA,
    BGRA,
  };
  virtual ~AlSensorCb() {}

  // TODO: deprecated
  virtual void newFrame(std::vector<uint8_t> rgbFrame,
                        std::vector<uint16_t> depthFrame) = 0;

  virtual void newFrame(const uint8_t *rgbFrame,
                        const uint16_t *depthFrame) = 0;

  virtual void onVideoFrameParams(unsigned int rgbWidth, unsigned int rgbHeight,
                                  AlSensorCb::VideoType videoType,
                                  unsigned int depthWidth, unsigned int depthHeight) = 0;

  virtual void onSensorParamsCb(float depthScaleK) = 0;
};

#endif // AL_SENSOR_CB_H
