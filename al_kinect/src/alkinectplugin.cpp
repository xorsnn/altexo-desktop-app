#include "alkinectplugin.hpp"
#include <iostream>

AlKinectPlugin::AlKinectPlugin()
    : m_device(NULL), m_freenect(NULL), m_internalThread(NULL), m_debug(true),
      m_initFinished(false) {}

AlKinectPlugin::~AlKinectPlugin() {
  if (m_internalThread->joinable()) {
    // TODO: may be join a little bit earlyer
    m_internalThread->join();
  }
  delete m_internalThread;
  m_internalThread = NULL;
}

void AlKinectPlugin::init(AlSensorCb *alSensorCb) {
  m_sensorCb = alSensorCb;
  m_newFrameSignal.connect(
      boost::bind(&AlSensorCb::newFrame, m_sensorCb, _1, _2));
  // init in another thread no to delay main prograpm execution
  m_internalThread = new boost::thread(&AlKinectPlugin::threadMain, this);
}

void AlKinectPlugin::threadMain() {
  try {
    m_freenect = new Freenect::Freenect();
    this->m_device = &(m_freenect->createDevice<ALFreenectDevice>(0));
    this->start();
    m_initFinished = true;
  } catch (std::exception &e) {
    std::cout << "\n";
    std::cout << "Error: " << e.what() << "\n";
  }
}

void AlKinectPlugin::requestNewFrame() {
  if (m_sensorCb && m_initFinished) {
    if (this->m_device->m_newDepthFrame && this->m_device->m_newDepthFrame) {
      m_newFrameSignal(this->m_device->getRGB(), this->m_device->getDepth());
    }
  }
}

void AlKinectPlugin::start() {
  this->m_device->startVideo();
  this->m_device->startDepth();
}

void AlKinectPlugin::stop() {
  this->m_device->stopVideo();
  this->m_device->stopDepth();
}
