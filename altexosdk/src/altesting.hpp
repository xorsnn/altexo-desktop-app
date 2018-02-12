#ifndef ALTESTING_H
#define ALTESTING_H

#include "AL_API/al_plugin_test_api.hpp"
#include <iostream>
#include <string>

class AlTesting : public AlPluginTestAPI {
public:
  AlTesting();
  ~AlTesting();

  void testMethod();
  int testMethod2(int a, int b);
};

#endif
