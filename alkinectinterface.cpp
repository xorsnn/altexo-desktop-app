#include "alkinectinterface.h"
#include "recorder.h"
//void DrawGLScene() {
//    static std::vector<uint8_t> depth(640*480*4);
//    static std::vector<uint8_t> rgb(640*480*4);

//    // using getTiltDegs() in a closed loop is unstable
//    /*if(device->getState().m_code == TILT_STATUS_STOPPED){
//            freenect_angle = device->getState().getTiltDegs();
//        }*/
//    device->updateState();
//    printf("\r demanded tilt angle: %+4.2f device tilt angle: %+4.2f", freenect_angle, device->getState().getTiltDegs());
//    fflush(stdout);

//    device->getDepth(depth);
//    device->getRGB(rgb);

//    got_frames = 0;

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();

//    glEnable(GL_TEXTURE_2D);

//    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth[0]);

//    glBegin(GL_TRIANGLE_FAN);
//    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
//    glTexCoord2f(0, 0); glVertex3f(0,0,0);
//    glTexCoord2f(1, 0); glVertex3f(640,0,0);
//    glTexCoord2f(1, 1); glVertex3f(640,480,0);
//    glTexCoord2f(0, 1); glVertex3f(0,480,0);
//    glEnd();

//    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
//    if (device->getVideoFormat() == FREENECT_VIDEO_RGB || device->getVideoFormat() == FREENECT_VIDEO_YUV_RGB)
//        glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);
//    else
//        glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &rgb[0]);

//    glBegin(GL_TRIANGLE_FAN);
//    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
//    glTexCoord2f(0, 0); glVertex3f(640,0,0);
//    glTexCoord2f(1, 0); glVertex3f(1280,0,0);
//    glTexCoord2f(1, 1); glVertex3f(1280,480,0);
//    glTexCoord2f(0, 1); glVertex3f(640,480,0);
//    glEnd();

//    glutSwapBuffers();
//}
ALKinectInterface* thisObject;

ALKinectInterface::ALKinectInterface(QObject *parent) :
    QObject(parent)
{
    thisObject = this;
    this->requested_format = FREENECT_VIDEO_RGB;
    this->freenect_angle = 0;
    this->got_frames = 0;
    this->window = 0;


//    Recorder* recorder = new Recorder(&w);
//    QThread* thread = new QThread();
//    recorder->moveToThread(thread);
//    QObject::connect(&w, SIGNAL(stopRecorder()), recorder, SLOT(stop()));
//    QObject::connect(thread, SIGNAL(started()), recorder, SLOT(start()));
//    thread->start();

    this->device = &freenect.createDevice<ALFreenectDevice>(0);
    this->device->startVideo();
    this->device->startDepth();


//    this->gl_threadfunc(NULL);
//    this->device->stopVideo();
//    this->device->stopDepth();
}

void ALKinectInterface::DrawGLScene() {
    static std::vector<uint8_t> depth(640*480*4);
    static std::vector<uint8_t> rgb(640*480*4);

    // using getTiltDegs() in a closed loop is unstable
    /*if(device->getState().m_code == TILT_STATUS_STOPPED){
            freenect_angle = device->getState().getTiltDegs();
        }*/
    thisObject->device->updateState();
//    printf("\r demanded tilt angle: %+4.2f device tilt angle: %+4.2f", freenect_angle, device->getState().getTiltDegs());
    fflush(stdout);

//    thisObject->device->getDepth(depth);
//    thisObject->device->getRGB(rgb);
    thisObject->getDepthDt(depth);
    thisObject->getRGBDt(rgb);

    thisObject->got_frames = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, thisObject->gl_depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth[0]);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(0,0,0);
    glTexCoord2f(1, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 1); glVertex3f(640,480,0);
    glTexCoord2f(0, 1); glVertex3f(0,480,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, thisObject->gl_rgb_tex);
    if (thisObject->device->getVideoFormat() == FREENECT_VIDEO_RGB || thisObject->device->getVideoFormat() == FREENECT_VIDEO_YUV_RGB)
        glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &rgb[0]);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 0); glVertex3f(1280,0,0);
    glTexCoord2f(1, 1); glVertex3f(1280,480,0);
    glTexCoord2f(0, 1); glVertex3f(640,480,0);
    glEnd();

    glutSwapBuffers();
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

void ALKinectInterface::ReSizeGLScene(int Width, int Height)
{
    glViewport(0,0,Width,Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, 1280, 480, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void ALKinectInterface::InitGL(int Width, int Height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glGenTextures(1, &gl_depth_tex);
    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &gl_rgb_tex);
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ReSizeGLScene(Width, Height);
}

void *ALKinectInterface::gl_threadfunc(void *arg)
{
    printf("GL thread\n");

    char fakeParam[] = "fake";
    char *fakeargv[] = { fakeParam, NULL };
    int fakeargc = 1;

//    glutInit(&g_argc, g_argv);
    glutInit( &fakeargc, fakeargv );

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(1280, 480);
    glutInitWindowPosition(0, 0);

    window = glutCreateWindow("LibFreenect");

    //todo
    glutDisplayFunc(DrawGLScene);
    glutIdleFunc(DrawGLScene);
//    glutReshapeFunc(&ReSizeGLScene);
//    glutKeyboardFunc(&keyPressed);

    InitGL(1280, 480);

    glutMainLoop();

    return NULL;
}

//QByteArray ALKinectInterface::getDepthDt() {
//    static std::vector<uint8_t> depth(640*480*4);
//    this->device->getDepth(depth);
//    QByteArray res(reinterpret_cast<const char*>(&depth[0]), 640*480*4);
//    return res;
//}

//QByteArray ALKinectInterface::getRGBDt() {
//    static std::vector<uint8_t> rgb(640*480*4);
//    this->device->getRGB(rgb);
//    QByteArray res(reinterpret_cast<const char*>(&rgb[0]), 640*480*4);
//    return res;
//}

void ALKinectInterface::getDepthDt(std::vector<uint8_t> &depth) {
//    static std::vector<uint8_t> depth(640*480*4);
    this->device->getDepth(depth);
//    return depth;
//    QByteArray res(reinterpret_cast<const char*>(&depth[0]), 640*480*4);
//    return res;
}

void ALKinectInterface::getRGBDt(std::vector<uint8_t> &rgb) {
//    static std::vector<uint8_t> rgb(640*480*4);
    this->device->getRGB(rgb);
//    return rgb;
//    QByteArray res(reinterpret_cast<const char*>(&rgb[0]), 640*480*4);
//    return res;
}

void ALKinectInterface::updateDeviceState() {
    this->device->updateState();
}

void ALKinectInterface::newFrameArrivedSlot() {
    emit this->newFrameArrivedSignal();
}
