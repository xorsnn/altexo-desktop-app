#include "alkinectinterface.h"
#include "recorder.h"

ALKinectInterface* thisObject;

ALKinectInterface::ALKinectInterface(QObject *parent) :
    QObject(parent)
{
    thisObject = this;
    this->requested_format = FREENECT_VIDEO_RGB;
    this->freenect_angle = 0;
    this->got_frames = 0;
    this->window = 0;

    this->device = &freenect.createDevice<ALFreenectDevice>(0);
    this->device->startVideo();
    this->device->startDepth();

//    this->device->stopVideo();
//    this->device->stopDepth();
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

void ALKinectInterface::getDepthDt(std::vector<uint8_t> &depth) {
    this->device->getDepth(depth);
}

void ALKinectInterface::getRGBDt(std::vector<uint8_t> &rgb) {
    this->device->getRGB(rgb);
}

void ALKinectInterface::updateDeviceState() {
//    this->device->updateState();
}
