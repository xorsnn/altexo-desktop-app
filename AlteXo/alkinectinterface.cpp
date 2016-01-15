#include "alkinectinterface.h"

ALKinectInterface* thisObject;

ALKinectInterface::ALKinectInterface(QObject *parent) :
    QObject(parent)
{
    thisObject = this;
    this->requested_format = FREENECT_VIDEO_RGB;
    this->freenect_angle = 0;
    this->got_frames = 0;
    this->device = NULL;
}

void ALKinectInterface::init() {
    this->device = &freenect.createDevice<ALFreenectDevice>(0);
    this->start();
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
    qDebug() << (this->device->getMaxDepth()+delta);
    this->device->setMaxDepth(this->device->getMaxDepth()+delta);
}

//void ALKinectInterface::keyPressed(unsigned char key, int x, int y) {
//    if (key == 27) {
//        glutDestroyWindow(window);
//    }
//    if (key == '1') {
//        device->setLed(LED_GREEN);
//    }
//    if (key == '2') {
//        device->setLed(LED_RED);
//    }
//    if (key == '3') {
//        device->setLed(LED_YELLOW);
//    }
//    if (key == '4') {
//        device->setLed(LED_BLINK_GREEN);
//    }
//    if (key == '5') {
//        // 5 is the same as 4
//        device->setLed(LED_BLINK_GREEN);
//    }
//    if (key == '6') {
//        device->setLed(LED_BLINK_RED_YELLOW);
//    }
//    if (key == '0') {
//        device->setLed(LED_OFF);
//    }
//    if (key == 'f') {
//        if (requested_format == FREENECT_VIDEO_IR_8BIT)
//            requested_format = FREENECT_VIDEO_RGB;
//        else if (requested_format == FREENECT_VIDEO_RGB)
//            requested_format = FREENECT_VIDEO_YUV_RGB;
//        else
//            requested_format = FREENECT_VIDEO_IR_8BIT;
//        device->setVideoFormat(requested_format);
//    }

//    if (key == 'w') {
//        freenect_angle++;
//        if (freenect_angle > 30) {
//            freenect_angle = 30;
//        }
//    }
//    if (key == 's' || key == 'd') {
//        freenect_angle = 0;
//    }
//    if (key == 'x') {
//        freenect_angle--;
//        if (freenect_angle < -30) {
//            freenect_angle = -30;
//        }
//    }
//    if (key == 'e') {
//        freenect_angle = 10;
//    }
//    if (key == 'c') {
//        freenect_angle = -10;
//    }
//    device->setTiltDegrees(freenect_angle);
//}

//deprecated
void ALKinectInterface::getDepthDt(std::vector<uint8_t> &depth) {
    this->device->getDepth(depth);
}
//deprecated
void ALKinectInterface::getRGBDt(std::vector<uint8_t> &rgb) {
    this->device->getRGB(rgb);
}

void ALKinectInterface::updateDeviceState() {
//    this->device->updateState();
}

void ALKinectInterface::needDataSlot() {

    static std::vector<uint8_t> rgb_(640*480*3);
    this->device->getRGB(rgb_);
    static std::vector<uint8_t> depth_(640*480*3);
    this->device->getDepth(depth_);

    QImage image_rgb((&(rgb_[0])), 640, 480, QImage::Format_RGB888);

    QImage image_depth((&(depth_[0])), 640, 480, QImage::Format_RGB888);

    QImage image(1280, 480, QImage::Format_RGB888);
    QPainter painter;
    painter.begin(&image);
    painter.drawImage(0, 0, image_depth);
    painter.drawImage(640, 0, image_rgb);
    painter.end();

    Q_EMIT this->newFrameSignal(image);
}

void ALKinectInterface::needWDataSlot() {
    static std::vector<uint8_t> rgb_(640*480*3);
    this->device->getRGB(rgb_);
    static std::vector<uint8_t> depth_(640*480*3);
    this->device->getDepth(depth_);

    QImage image_rgb((&(rgb_[0])), 640, 480, QImage::Format_RGB888);

    QImage image_depth((&(depth_[0])), 640, 480, QImage::Format_RGB888);

    QImage image(1280, 480, QImage::Format_RGB888);
    QPainter painter;
    painter.begin(&image);
    painter.drawImage(0, 0, image_depth);
    painter.drawImage(640, 0, image_rgb);
    painter.end();

    Q_EMIT this->newWFrameSignal(image);
}

void ALKinectInterface::updateSettingsSlot()
{
    this->device->updateSettings();
}
