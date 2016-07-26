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

  void init(AlSensorCb *alSensorCb);
  void start();
  void stop();

  void needDataSlot();
  void updateDeviceState();

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
};

#endif // ALKINECTINTERFACE_H
