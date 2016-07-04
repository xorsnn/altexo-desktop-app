#ifndef AL_SENSOR_CB_H
#define AL_SENSOR_CB_H

#include <string>
#include <vector>

class AlSensorCb {
public:
  virtual ~AlSensorCb() {}
  virtual void newFrame(std::vector<uint8_t> rgbFrame) = 0;
};
//]

#endif // AL_SENSOR_CB_H
