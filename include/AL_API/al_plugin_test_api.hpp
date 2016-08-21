#ifndef ALPLUGINTESTAPI_H
#define ALPLUGINTESTAPI_H

#include "altextmessage.hpp"
#include <string>


class AlPluginTestAPI {
public:
  virtual ~AlPluginTestAPI() {}
  virtual void testMethod() = 0;
};
//]

#endif // ALPLUGINTESTAPI_H
