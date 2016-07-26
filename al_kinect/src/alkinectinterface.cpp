#include "alkinectinterface.h"
#include <boost/bind.hpp>

ALKinectInterface *thisObject;

ALKinectInterface::ALKinectInterface() {
  thisObject = this;
  this->requested_format = FREENECT_VIDEO_RGB;
  this->freenect_angle = 0;
  this->got_frames = 0;
  this->device = NULL;
}

void ALKinectInterface::init(AlSensorCb *alSensorCb) {
  this->device = &freenect.createDevice<ALFreenectDevice>(0);
  this->start();
  m_sensorCb = alSensorCb;
  m_newFrameSignal.connect(
      boost::bind(&AlSensorCb::newFrame, m_sensorCb, _1, _2));
}

void ALKinectInterface::start() {
  this->device->startVideo();
  this->device->startDepth();
}

void ALKinectInterface::stop() {
  this->device->stopVideo();
  this->device->stopDepth();
}

void ALKinectInterface::updateDeviceState() {
  //    this->device->updateState();
}

void ALKinectInterface::needDataSlot() {
  if (this->device->m_newDepthFrame && this->device->m_newDepthFrame) {
    m_newFrameSignal(this->device->getRGB(), this->device->getDepth());
  }
}
