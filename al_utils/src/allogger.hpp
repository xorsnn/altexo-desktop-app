
#ifndef ALLOGGER_H
#define ALLOGGER_H

#include <iostream>

class AlLogger {

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
