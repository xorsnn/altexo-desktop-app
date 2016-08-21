#ifndef ALKINECTMANAGER_H
#define ALKINECTMANAGER_H

#include <iostream>

// windows
#pragma once

#include <windows.h>

// #include <xnamath.h>

#include "NuiApi.h"

#include <boost/thread/thread.hpp>

class AlKinectManager {
  static const int cBytesPerPixel = 4;

  static const NUI_IMAGE_RESOLUTION cDepthResolution =
      NUI_IMAGE_RESOLUTION_640x480;
  static const NUI_IMAGE_RESOLUTION cColorResolution =
      NUI_IMAGE_RESOLUTION_640x480;

public:
  AlKinectManager();
  ~AlKinectManager();

  HRESULT CreateFirstConnected();
  void init();

private:
  void _internalThread();
  HRESULT _render();
  HRESULT _processDepth();
  HRESULT _processColor();
  HRESULT _mapColorToDepth();

  boost::thread *m_internalThread;
  // Kinect
  INuiSensor *m_pNuiSensor;
  HANDLE m_hNextDepthFrameEvent;
  HANDLE m_pDepthStreamHandle;
  HANDLE m_hNextColorFrameEvent;
  HANDLE m_pColorStreamHandle;

  // for mapping depth to color
  USHORT *m_depthD16;
  BYTE *m_colorRGBX;
  LONG *m_colorCoordinates;

  // to prevent drawing until we have data for both streams
  bool m_bDepthReceived;
  bool m_bColorReceived;

    LONG                                m_depthWidth;
    LONG                                m_depthHeight;

    LONG                                m_colorWidth;
    LONG                                m_colorHeight;

    LONG                                m_colorToDepthDivisor;
    
  bool m_debug;
  bool m_bPaused;
};

#endif // ALKINECTMANAGER_H
