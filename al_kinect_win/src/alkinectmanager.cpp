#include "alkinectmanager.hpp"

AlKinectManager::AlKinectManager() : m_debug(true) {
  std::cout << "constructor" << std::endl;
    DWORD width = 0;
    DWORD height = 0;

    NuiImageResolutionToSize(cDepthResolution, width, height);
    m_depthWidth  = static_cast<LONG>(width);
    m_depthHeight = static_cast<LONG>(height);

    NuiImageResolutionToSize(cColorResolution, width, height);
    m_colorWidth  = static_cast<LONG>(width);
    m_colorHeight = static_cast<LONG>(height);

    m_colorToDepthDivisor = m_colorWidth/m_depthWidth;

    // m_hInst = NULL;
    // m_hWnd = NULL;
    // m_featureLevel = D3D_FEATURE_LEVEL_11_0;
    // m_pd3dDevice = NULL;
    // m_pImmediateContext = NULL;
    // m_pSwapChain = NULL;
    // m_pRenderTargetView = NULL;
    // m_pDepthStencil = NULL;
    // m_pDepthStencilView = NULL;
    // m_pVertexLayout = NULL;
    // m_pVertexBuffer = NULL;
    // m_pCBChangesEveryFrame = NULL;
    // m_projection;

    // m_pVertexShader = NULL;
    // m_pPixelShader = NULL;
    // m_pGeometryShader = NULL;

    // m_xyScale = 0.0f;

    // // Initial window resolution
    // m_windowResX = 1280;
    // m_windowResY = 960;
    //
    // m_pDepthTexture2D = NULL;
    // m_pDepthTextureRV = NULL;
    // m_pColorTexture2D = NULL;
    // m_pColorTextureRV = NULL;
    // m_pColorSampler = NULL;

    m_bDepthReceived = false;
    m_bColorReceived = false;

    m_hNextDepthFrameEvent = INVALID_HANDLE_VALUE;
    m_pDepthStreamHandle = INVALID_HANDLE_VALUE;
    m_hNextColorFrameEvent = INVALID_HANDLE_VALUE;
    m_pColorStreamHandle = INVALID_HANDLE_VALUE;

    m_depthD16 = new USHORT[m_depthWidth*m_depthHeight];
    m_colorCoordinates = new LONG[m_depthWidth*m_depthHeight*2];
    m_colorRGBX = new BYTE[m_colorWidth*m_colorHeight*cBytesPerPixel];

    // m_bNearMode = false;

    m_bPaused = false;
}

AlKinectManager::~AlKinectManager() {
  if (NULL != m_pNuiSensor) {
    m_pNuiSensor->NuiShutdown();
    m_pNuiSensor->Release();
  }

  // if (m_pImmediateContext) {
  //   m_pImmediateContext->ClearState();
  // }
  //
  // SAFE_RELEASE(m_pCBChangesEveryFrame);
  // SAFE_RELEASE(m_pGeometryShader);
  // SAFE_RELEASE(m_pPixelShader);
  // SAFE_RELEASE(m_pVertexBuffer);
  // SAFE_RELEASE(m_pVertexLayout);
  // SAFE_RELEASE(m_pVertexShader);
  // SAFE_RELEASE(m_pDepthStencil);
  // SAFE_RELEASE(m_pDepthStencilView);
  // SAFE_RELEASE(m_pDepthTexture2D);
  // SAFE_RELEASE(m_pDepthTextureRV);
  // SAFE_RELEASE(m_pColorTexture2D);
  // SAFE_RELEASE(m_pColorTextureRV);
  // SAFE_RELEASE(m_pColorSampler);
  // SAFE_RELEASE(m_pRenderTargetView);
  // SAFE_RELEASE(m_pSwapChain);
  // SAFE_RELEASE(m_pImmediateContext);
  // SAFE_RELEASE(m_pd3dDevice);
  //
  // CloseHandle(m_hNextDepthFrameEvent);
  // CloseHandle(m_hNextColorFrameEvent);
  //
  // // done with pixel data
  // delete[] m_colorRGBX;
  // delete[] m_colorCoordinates;
  // delete[] m_depthD16;
}

void AlKinectManager::init() {
  m_internalThread = new boost::thread(&AlKinectManager::_internalThread, this);
}

void AlKinectManager::_internalThread() {
  // if ( FAILED( g_Application.CreateFirstConnected() ) )
  // {
  //     MessageBox(NULL, L"No ready Kinect found!", L"Error", MB_ICONHAND |
  //     MB_OK);
  //     return 0;
  // }
  CreateFirstConnected();
  while (1) {
    std::cout << "oper" << std::endl;
    // if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
    // {
    //     TranslateMessage(&msg);
    //     DispatchMessageW(&msg);
    // }
    // else
    // {
    _render();
    // }
    boost::this_thread::sleep_for(boost::chrono::milliseconds(33));
  }
}

HRESULT AlKinectManager::_render() {

  if (m_bPaused) {
    return S_OK;
  }

  bool needToMapColorToDepth = false;

  if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNextDepthFrameEvent, 0)) {
    // if we have received any valid new depth data we may need to draw
    if (SUCCEEDED(_processDepth())) {
      needToMapColorToDepth = true;
    }

    if (m_debug) {
      std::cout << "_processDepth" << std::endl;
    }
  }

  if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNextColorFrameEvent, 0)) {
    // if we have received any valid new color data we may need to draw
    if (SUCCEEDED(_processColor())) {
      needToMapColorToDepth = true;
    }
    if (m_debug) {
      std::cout << "_processColor" << std::endl;
    }
  }

  // If we have not yet received any data for either color or depth since we
  // started up, we shouldn't draw
  if (!m_bDepthReceived || !m_bColorReceived) {
    needToMapColorToDepth = false;
  }

  TODO: sems unnesessary - take a look later
  // if (needToMapColorToDepth) {
  //   _mapColorToDepth();
  // }

  return S_OK;
}

HRESULT AlKinectManager::_processDepth() {
  NUI_IMAGE_FRAME imageFrame;

  HRESULT hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pDepthStreamHandle, 0,
                                                        &imageFrame);
  if (FAILED(hr)) {
    return hr;
  }

  NUI_LOCKED_RECT LockedRect;
  hr = imageFrame.pFrameTexture->LockRect(0, &LockedRect, NULL, 0);
  if (FAILED(hr)) {
    return hr;
  }

  memcpy(m_depthD16, LockedRect.pBits, LockedRect.size);
  m_bDepthReceived = true;

  hr = imageFrame.pFrameTexture->UnlockRect(0);
  if (FAILED(hr)) {
    return hr;
  };

  hr = m_pNuiSensor->NuiImageStreamReleaseFrame(m_pDepthStreamHandle,
                                                &imageFrame);

  // // copy to our d3d 11 depth texture
  // D3D11_MAPPED_SUBRESOURCE msT;
  // hr = m_pImmediateContext->Map(m_pDepthTexture2D, NULL,
  //                               D3D11_MAP_WRITE_DISCARD, NULL, &msT);
  // if (FAILED(hr)) {
  //   return hr;
  // }
  //
  // memcpy(msT.pData, m_depthD16, LockedRect.size);
  // m_pImmediateContext->Unmap(m_pDepthTexture2D, NULL);

  return hr;
}

HRESULT AlKinectManager::_processColor() {
  NUI_IMAGE_FRAME imageFrame;

  HRESULT hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pColorStreamHandle, 0,
                                                        &imageFrame);
  if (FAILED(hr)) {
    return hr;
  }

  NUI_LOCKED_RECT LockedRect;
  hr = imageFrame.pFrameTexture->LockRect(0, &LockedRect, NULL, 0);
  if (FAILED(hr)) {
    return hr;
  }

  memcpy(m_colorRGBX, LockedRect.pBits, LockedRect.size);
  m_bColorReceived = true;

  hr = imageFrame.pFrameTexture->UnlockRect(0);
  if (FAILED(hr)) {
    return hr;
  };

  hr = m_pNuiSensor->NuiImageStreamReleaseFrame(m_pColorStreamHandle,
                                                &imageFrame);

  return hr;
}

HRESULT AlKinectManager::_mapColorToDepth() {
  HRESULT hr;

  // Get of x, y coordinates for color in depth space
  // This will allow us to later compensate for the differences in location,
  // angle, etc between the depth and color cameras
  m_pNuiSensor
      ->NuiImageGetColorPixelCoordinateFrameFromDepthPixelFrameAtResolution(
          cColorResolution, cDepthResolution, m_depthWidth * m_depthHeight,
          m_depthD16, m_depthWidth * m_depthHeight * 2, m_colorCoordinates);

  // copy to our d3d 11 color texture
  // D3D11_MAPPED_SUBRESOURCE msT;
  // hr = m_pImmediateContext->Map(m_pColorTexture2D, NULL,
  //                               D3D11_MAP_WRITE_DISCARD, NULL, &msT);
  if (FAILED(hr)) {
    return hr;
  }

  // loop over each row and column of the color
  for (LONG y = 0; y < m_colorHeight; ++y) {
    // LONG *pDest = (LONG *)((BYTE *)msT.pData + msT.RowPitch * y);
    for (LONG x = 0; x < m_colorWidth; ++x) {
      // calculate index into depth array
      int depthIndex =
          x / m_colorToDepthDivisor + y / m_colorToDepthDivisor * m_depthWidth;

      // retrieve the depth to color mapping for the current depth pixel
      LONG colorInDepthX = m_colorCoordinates[depthIndex * 2];
      LONG colorInDepthY = m_colorCoordinates[depthIndex * 2 + 1];

      // make sure the depth pixel maps to a valid point in color space
      if (colorInDepthX >= 0 && colorInDepthX < m_colorWidth &&
          colorInDepthY >= 0 && colorInDepthY < m_colorHeight) {
        // calculate index into color array
        LONG colorIndex = colorInDepthX + colorInDepthY * m_colorWidth;

        // set source for copy to the color pixel
        LONG *pSrc = (LONG *)m_colorRGBX + colorIndex;
        // *pDest = *pSrc;
      } else {
        // *pDest = 0;
      }

      // pDest++;
    }
  }

  if (m_debug) {
    std::cout << m_colorToDepthDivisor << std::endl;
  }
  // m_pImmediateContext->Unmap(m_pColorTexture2D, NULL);

  return hr;
}

HRESULT AlKinectManager::CreateFirstConnected() {
  if (m_debug) {
    std::cout << "AlKinectManager::CreateFirstConnected" << std::endl;
  }

  INuiSensor *pNuiSensor = NULL;
  HRESULT hr;

  int iSensorCount = 0;
  hr = NuiGetSensorCount(&iSensorCount);
  if (FAILED(hr)) {
    return hr;
  }

  // Look at each Kinect sensor
  for (int i = 0; i < iSensorCount; ++i) {
    // Create the sensor so we can check status, if we can't create it, move on
    // to the next
    hr = NuiCreateSensorByIndex(i, &pNuiSensor);
    if (FAILED(hr)) {
      continue;
    }

    // Get the status of the sensor, and if connected, then we can initialize it
    hr = pNuiSensor->NuiStatus();
    if (S_OK == hr) {
      m_pNuiSensor = pNuiSensor;
      if (m_debug) {
        std::cout << "Yahooo!" << std::endl;
      }
      break;
    }

    // This sensor wasn't OK, so release it since we're not using it
    pNuiSensor->Release();
  }

  if (NULL == m_pNuiSensor) {
    return E_FAIL;
  }

  // Initialize the Kinect and specify that we'll be using depth
  hr = m_pNuiSensor->NuiInitialize(
      NUI_INITIALIZE_FLAG_USES_COLOR |
      NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX);
  if (FAILED(hr)) {
    return hr;
  }

  // Create an event that will be signaled when depth data is available
  m_hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  // Open a depth image stream to receive depth frames
  hr = m_pNuiSensor->NuiImageStreamOpen(
      NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, cDepthResolution, 0, 2,
      m_hNextDepthFrameEvent, &m_pDepthStreamHandle);
  if (FAILED(hr)) {
    return hr;
  }

  // Create an event that will be signaled when color data is available
  m_hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  // Open a color image stream to receive color frames
  hr = m_pNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, cColorResolution,
                                        0, 2, m_hNextColorFrameEvent,
                                        &m_pColorStreamHandle);
  if (FAILED(hr)) {
    return hr;
  }

  // Start with near mode on
  // TODO:
  // ToggleNearMode();

  return hr;
}
