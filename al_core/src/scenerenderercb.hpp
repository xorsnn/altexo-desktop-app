#ifndef SCENERENDERERCB_H
#define SCENERENDERERCB_H

#include "altypes.hpp"
#include "stdint.h"

class SceneRendererCb {
public:
  virtual void updateRemoteFrameCb(const uint8_t *image, int width,
                                   int height) = 0;
  virtual void updateLocalFrameCb(const uint8_t *image, int width,
                                  int height) = 0;
  virtual void setLocalStreamModeCb(al::VideoMode mode) = 0;
  virtual void updateResolutionCb(int width, int height) = 0;
};
#endif // SCENERENDERERCB_H
