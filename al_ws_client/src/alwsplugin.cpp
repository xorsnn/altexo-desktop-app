#include "alwsplugin.hpp"
#include <iostream>
using namespace std;

AlWsPlugin::AlWsPlugin() {
  std::cout << "AlWsPlugin constructor" << std::endl;
  m_connClient = NULL;
}

AlWsPlugin::~AlWsPlugin() {
  delete m_connClient;
  m_connClient = NULL;
}

void AlWsPlugin::init(AlCallback *alCallback) {
  m_connClient = new AlConnClient();
  // m_manager->init(alCallback);
}
