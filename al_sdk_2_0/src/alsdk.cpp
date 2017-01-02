#include "alsdk.hpp"
#include <iostream>

AlSdk::AlSdk() {
  std::cout << "AlSdk::AlSdk" << std::endl;
  std::cout << "*******************************" << std::endl;
  std::cout << "*******************************" << std::endl;
  std::cout << "*******************************" << std::endl;
  std::cout << "*******************************" << std::endl;
  // m_conductor = new rtc::RefCountedObject<AlConductor>();
}

AlSdk::~AlSdk() {
  std::cout << "<><><><><><><><><><><>" << std::endl;
  std::cout << "<><><><><><><><><><><>" << std::endl;
  std::cout << "<><><><><><><><><><><>" << std::endl;
}

void AlSdk::run() {
  std::cout << "AlSdk::threadMain" << std::endl;
  m_internalThread = new boost::thread(&AlSdk::threadMain, this);
  std::cout << "AlSdk::threadMain" << std::endl;
}

void AlSdk::threadMain() { std::cout << "AlSdk::threadMain" << std::endl; }
