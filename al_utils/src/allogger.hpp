
#ifndef ALLOGGER_H
#define ALLOGGER_H

#include <iostream>
#include <string>
#include "al_utils_export.h"

class AL_UTILS_EXPORT AlLogger {

public:
  AlLogger();
  ~AlLogger();

  void operator<<(std::string msg);
};

static AlLogger alLogger() {
  AlLogger lg;
  return lg;
};

#endif // ALLOGGER_H
