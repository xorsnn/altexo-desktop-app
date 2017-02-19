#ifndef AL_SENSOR_CB_H
#define AL_SENSOR_CB_H

#include <string>
#include <vector>

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

  virtual void onVideoFrameParams(uint rgbWidth, uint rgbHeight,
                                  AlSensorCb::VideoType videoType,
                                  uint depthWidth, uint depthHeight) = 0;
};

#endif // AL_SENSOR_CB_H
