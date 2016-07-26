#include "alkinectplugin.hpp"
#include <iostream>

AlKinectPlugin::AlKinectPlugin() {
  m_kinectInterface = NULL;
  // TODO: seems like thread is not a cycle
  m_internalThread = boost::thread(&AlKinectPlugin::threadMain, this);
}

AlKinectPlugin::~AlKinectPlugin() {}

void AlKinectPlugin::init(AlSensorCb *alSensorCb) { m_sensorCb = alSensorCb; }

// TODO: seems like it makes no to run in a thread not a loop
void AlKinectPlugin::threadMain() {
  m_kinectInterface = new ALKinectInterface();
  m_kinectInterface->init(m_sensorCb);
  m_needNewFrameSignal.connect(
      boost::bind(&ALKinectInterface::needDataSlot, m_kinectInterface));
}

void AlKinectPlugin::requestNewFrame() {
  if (m_sensorCb) {
    m_needNewFrameSignal();
  }
}
