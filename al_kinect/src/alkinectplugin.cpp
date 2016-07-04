#include "alkinectplugin.hpp"
#include <iostream>

AlKinectPlugin::AlKinectPlugin() {
  m_kinectInterface = NULL;
  m_internalThread = boost::thread(&AlKinectPlugin::threadMain, this);
}
AlKinectPlugin::~AlKinectPlugin() {}

void AlKinectPlugin::init(AlCallback *alCallback) {
  std::cout << "AlKinectPlugin::init" << std::endl;
}

void AlKinectPlugin::threadMain() {
  m_kinectInterface = new ALKinectInterface();
  m_kinectInterface->init();
  // this->kinectInterface = new ALKinectInterface();
  // QThread* kinectThread = new QThread();
  // this->kinectInterface->moveToThread(kinectThread);
  // kinectThread->start();
  // QTimer::singleShot(0, this->kinectInterface, SLOT(init()));
}
