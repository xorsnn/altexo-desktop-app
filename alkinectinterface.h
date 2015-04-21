#ifndef ALKINECTINTERFACE_H
#define ALKINECTINTERFACE_H

#include <QObject>
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

signals:
    void newFrameArrivedSignal();

public slots:
    static void DrawGLScene();
//    void keyPressed(unsigned char key, int x, int y);
    void ReSizeGLScene(int Width, int Height);
    void InitGL(int Width, int Height);
    void *gl_threadfunc(void *arg);

//    QByteArray getDepthDt();
//    QByteArray getRGBDt();
    void getDepthDt(std::vector<uint8_t> &depth);
    void getRGBDt(std::vector<uint8_t> &rgb);

    void updateDeviceState();

    void newFrameArrivedSlot();

public:
    ALFreenectDevice* device;
    GLuint gl_depth_tex;
    GLuint gl_rgb_tex;
    double freenect_angle;
    int got_frames;

private:
    Freenect::Freenect freenect;
    freenect_video_format requested_format;

    int window;

    int g_argc;
    char **g_argv;

};

#endif // ALKINECTINTERFACE_H
