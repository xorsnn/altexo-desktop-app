#include "alkinectplugin.hpp"
#include <iostream>

AlKinectPlugin::AlKinectPlugin() {
  m_kinectInterface = NULL;
  m_internalThread = boost::thread(&AlKinectPlugin::threadMain, this);
}
AlKinectPlugin::~AlKinectPlugin() {}

void AlKinectPlugin::init(AlSensorCb *alSensorCb) {
  // std::cout << "AlKinectPlugin::init" << std::endl;
  m_sensorCb = alSensorCb;
}

void AlKinectPlugin::threadMain() {
  m_kinectInterface = new ALKinectInterface();
  m_kinectInterface->init(m_sensorCb);
  m_needNewFrameSignal.connect(
      boost::bind(&ALKinectInterface::needWDataSlot, m_kinectInterface));
  // this->kinectInterface = new ALKinectInterface();
  // QThread* kinectThread = new QThread();
  // this->kinectInterface->moveToThread(kinectThread);
  // kinectThread->start();
  // QTimer::singleShot(0, this->kinectInterface, SLOT(init()));
}

void AlKinectPlugin::requestNewFrame() {
  if (m_sensorCb) {
    // std::cout << "AlKinectPlugin::requestNewFrame" << std::endl;
    m_needNewFrameSignal();
    // if (m_kinectInterface->device) {
    //   std::vector<uint8_t> tmp_rgb = m_kinectInterface->device->getRGB();
    // }
    // m_sensorCb->newFrame(m_kinectInterface->device->getRGB());
  }
}
