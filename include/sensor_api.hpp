#ifndef SENSORAPI_H
#define SENSORAPI_H

#include <string>

class AlCallback;

class AlSensorAPI {
public:
  virtual ~AlSensorAPI() {}
  virtual void init(AlCallback *alCallback) = 0;
};
//]

#endif // SENSORAPI_H
