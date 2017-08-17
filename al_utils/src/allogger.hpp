
#ifndef ALLOGGER_H
#define ALLOGGER_H

#include <iostream>
#include <string>

#if defined _WIN32
#include "al_utils_export.h"
#endif

class
#if defined _WIN32
AL_UTILS_EXPORT
#endif
AlLogger {


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
