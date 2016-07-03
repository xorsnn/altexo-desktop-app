#include "alwsplugin.hpp"
#include <iostream>
using namespace std;

AlWsPlugin::AlWsPlugin() {
  std::cout << "AlWsPlugin constructor" << std::endl;
  // m_manager = new AlManager();
}

AlWsPlugin::~AlWsPlugin() {
  // delete m_manager;
  // m_manager = NULL;
}

void AlWsPlugin::init(AlCallback *alCallback) {
  // m_manager->init(alCallback);
}
