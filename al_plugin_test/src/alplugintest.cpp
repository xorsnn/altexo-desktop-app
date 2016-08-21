#include "alplugintest.hpp"
#include <iostream>

AlPluginTest::AlPluginTest() {
  std::cout << "AlPluginTest constructor" << std::endl;
}

AlPluginTest::~AlPluginTest() {
  std::cout << "AlPluginTest destructor" << std::endl;
}
void AlPluginTest::testMethod() {
  std::cout << "AlPluginTest::testMethod" << std::endl;
}
