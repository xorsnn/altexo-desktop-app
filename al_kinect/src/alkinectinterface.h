#ifndef ALKINECTINTERFACE_H
#define ALKINECTINTERFACE_H

#include "AL_CB/al_sensor_cb.hpp"
#include "alfreenectdevice.h"
#include <boost/signals2/signal.hpp>
#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

class ALKinectInterface {
public:
  ALKinectInterface();

  // Q_SIGNALS:
  // void newWFrameSignal(QImage *image);
  // public Q_SLOTS:

  void init(AlSensorCb *alSensorCb);
  void start();
  void stop();

  void needWDataSlot();
  void updateDeviceState();
  void changeMaxDepth(int delta);
  void updateSettingsSlot();

public:
  ALFreenectDevice *device;
  GLuint gl_depth_tex;
  GLuint gl_rgb_tex;
  double freenect_angle;
  int got_frames;

private:
  Freenect::Freenect freenect;
  freenect_video_format requested_format;
  AlSensorCb *m_sensorCb;
  boost::signals2::signal<void(std::vector<uint8_t> rgbFrame,
                               std::vector<uint16_t> depthFrame)>
      m_newFrameSignal;

  // QImage *m_image;
};

#endif // ALKINECTINTERFACE_H
