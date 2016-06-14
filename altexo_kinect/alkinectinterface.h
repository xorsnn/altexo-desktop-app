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
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif


class ALKinectInterface : public QObject
{
    Q_OBJECT
public:
    explicit ALKinectInterface(QObject *parent = 0);

Q_SIGNALS:

    void newWFrameSignal(QImage* image);
public Q_SLOTS:

    void init();
    void start();
    void stop();

    void needWDataSlot();

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

    QImage* m_image;
};

#endif // ALKINECTINTERFACE_H
