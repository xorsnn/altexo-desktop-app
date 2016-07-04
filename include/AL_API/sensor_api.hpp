#ifndef SENSORAPI_H
#define SENSORAPI_H

#include "AL_CB/al_sensor_cb.hpp"
#include <string>

class AlCallback;

class AlSensorAPI {
public:
  virtual ~AlSensorAPI() {}
  virtual void init(AlSensorCb *alSensorCb) = 0;
  virtual void requestNewFrame() = 0;
};
//]

#endif // SENSORAPI_H
