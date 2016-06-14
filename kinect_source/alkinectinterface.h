#ifndef ALKINECTINTERFACE_H
#define ALKINECTINTERFACE_H

#include <QObject>
#include <QImage>
#include <QPainter>
#include "alfreenectdevice.h"
#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
//#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif


class ALKinectInterface : public QObject
{
    Q_OBJECT
public:
    explicit ALKinectInterface(QObject *parent = 0);

Q_SIGNALS:
    void newFrameSignal(QImage image);
    void newWFrameSignal(QImage image);
public Q_SLOTS:

    void init();
    void start();
    void stop();

    void needDataSlot();
    void needWDataSlot();

    void getDepthDt(std::vector<uint8_t> &depth);
    void getRGBDt(std::vector<uint8_t> &rgb);

    void updateDeviceState();

    void changeMaxDepth(int delta);

    void updateSettingsSlot();

public:
    ALFreenectDevice* device;
    GLuint gl_depth_tex;
    GLuint gl_rgb_tex;
    double freenect_angle;
    int got_frames;

private:
    Freenect::Freenect freenect;
    freenect_video_format requested_format;

//    int window;

//    int g_argc;
//    char **g_argv;

};

#endif // ALKINECTINTERFACE_H
