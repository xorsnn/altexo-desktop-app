#include "alkinectinterface.h"
#include <boost/bind.hpp>

ALKinectInterface *thisObject;

ALKinectInterface::ALKinectInterface() {
  thisObject = this;
  this->requested_format = FREENECT_VIDEO_RGB;
  this->freenect_angle = 0;
  this->got_frames = 0;
  this->device = NULL;

  // TODO: add to destructor
  // this->m_image = new QImage(1280, 480, QImage::Format_RGB888);
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

void ALKinectInterface::changeMaxDepth(int delta) {
  std::cout << (this->device->getMaxDepth() + delta) << std::endl;
  this->device->setMaxDepth(this->device->getMaxDepth() + delta);
}

void ALKinectInterface::updateDeviceState() {
  //    this->device->updateState();
}

void ALKinectInterface::needWDataSlot() {
  // std::cout << "ALKinectInterface::needWDataSlot" << std::endl;
  if (this->device->m_newDepthFrame && this->device->m_newDepthFrame) {
    // static std::vector<uint8_t> rgb_(640 * 480 * 3);
    // this->device->getRGB(rgb_);
    m_newFrameSignal(this->device->getRGB(), this->device->getDepth());
    // static std::vector<uint8_t> depth_(640 * 480 * 3);
    // this->device->getDepth(depth_);

    // QImage image_rgb((&(rgb_[0])), 640, 480, QImage::Format_RGB888);
    // QImage image_depth((&(depth_[0])), 640, 480, QImage::Format_RGB888);
    //
    // QPainter painter;
    // painter.begin(m_image);
    // painter.drawImage(0, 0, image_depth);
    // painter.drawImage(640, 0, image_rgb);
    // painter.end();
  }

  // Q_EMIT this->newWFrameSignal(m_image);
}

void ALKinectInterface::updateSettingsSlot() { this->device->updateSettings(); }
