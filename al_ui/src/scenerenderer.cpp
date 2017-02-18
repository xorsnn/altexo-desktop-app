#include "scenerenderer.hpp"

SceneRenderer::SceneRenderer(int winWidth, int winHeight)
    : WIDTH(0), HEIGHT(0), m_outPixel(0), pendingRenderTexResize(false),
      m_debug(true), m_remoteFrameRenderer(-0.5, -0.5, -0.5, -0.5),
      m_localFrameRenderer(-0.5, -0.5, -0.5, -0.5), m_winWidth(winWidth),
      m_winHeight(winHeight), m_remoteStreamMode(1), m_localStreamMode(1) {
  _updateRenderersPos();
}

void SceneRenderer::setRemoteStreamMode(int mode) { m_remoteStreamMode = mode; }
void SceneRenderer::setLocalStreamMode(int mode) { m_localStreamMode = mode; }

void SceneRenderer::updateResolution(int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  m_outPixel.resize(WIDTH * HEIGHT * 2 * 3);
  pendingRenderTexResize = true;
}

int SceneRenderer::init() {
  m_newFrame = false;
  // GL_CHECK_ERRORS

  m_bottomPlane.init();
  m_hologram.init();

  initFBO();

  m_sensorDataFboRenderer.init();
  // TODO: doesn't make sense sitce it's default
  if (m_remoteStreamMode == AUDIO_VIDEO) {
    m_remoteFrameRenderer.init();
  }
  m_localFrameRenderer.init();
  m_worldCoordinate.init();

  // setup the camera position and target
  // cam.SetPosition(glm::vec3(1, 1, 1));
  cam.SetPosition(glm::vec3(1000, 1000, 1000));
  cam.SetTarget(glm::vec3(0, 0, 0));

  // also rotate the camera for proper orientation
  glm::vec3 look = glm::normalize(cam.GetTarget() - cam.GetPosition());

  float yaw = glm::degrees(float(atan2(look.z, look.x) + M_PI));
  float pitch = glm::degrees(asin(look.y));
  rX = yaw;
  rY = pitch;

  cam.Rotate(rX, rY, 0);

  cout << "Initialization successfull" << endl;
  return 1;
}

void SceneRenderer::render() {
  // TODO: move to 'onResize' event
  cam.SetupProjection(45, (GLfloat)m_winWidth / m_winHeight, 0.1f, 10000.0f);
  // set the camera transformation
  glm::mat4 MV = cam.GetViewMatrix();
  glm::mat4 P = cam.GetProjectionMatrix();
  glm::mat4 MVP = P * MV;

  if (pendingRenderTexResize) {
    pendingRenderTexResize = false;
    _resizeRenderTex();
  }

  // ============ FBO ==============
  // enable FBO
  glBindFramebuffer(GL_FRAMEBUFFER, fboID);
  // render to colour attachment 0
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  // clear the colour and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // clear the colour and depth buffer
  // ============ ~FBO ==============

  m_sensorDataFboRenderer.render(WIDTH * 2, HEIGHT);

  // alLogger() << "preSendingFrames";
  if (sendingFrames || true) {
    // alLogger() << "sendingFrames";
    glReadPixels(0, 0, WIDTH * 2, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE,
                 &(m_outPixel[0]));
    // TODO: NOT SURE IF IT IS OK TO MULTIPLY BY 2
    newFrameSignal(m_outPixel, WIDTH * 2, HEIGHT);

    // NOTE: TESTING
    // TODO: remove
    // updateLocalFrame(&m_outPixel[0], WIDTH * 2, HEIGHT);
    updateRemoteFrame(&m_outPixel[0], WIDTH * 2, HEIGHT);
  }

  // ============ FBO ==============
  // unbind the FBO
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // restore the default back buffer
  glDrawBuffer(GL_BACK_LEFT);
  // bind the FBO output at the current texture
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, renderTextureID);
  // ============ ~FBO ==============

  glViewport(0, 0, m_winWidth, m_winHeight);

  // Elements rendering
  m_bottomPlane.render(&MVP);
  // TODO: case when both sides has sensors is not handled
  if (m_remoteStreamMode == AUDIO_VIDEO) {
    m_remoteFrameRenderer.render();
  } else if (m_remoteStreamMode == HOLOGRAM) {
    m_remoteFrameRenderer.bindToTex();
    m_hologram.render(&MVP);
  }

  if (m_localStreamMode == HOLOGRAM) {
    // alLogger() << "HOLOGRAM";
    m_hologram.render(&MVP);
  } else if (m_localStreamMode == AUDIO_VIDEO) {
    // alLogger() << "VIDEO";
    m_localFrameRenderer.render();
  }

  m_worldCoordinate.render(&MVP);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // seems to be needed by something, otherwise some artifacts appears
  glActiveTexture(GL_TEXTURE0);
}

// initialize FBO
void SceneRenderer::initFBO() {
  // generate and bind fbo ID
  glGenFramebuffers(1, &fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, fboID);

  // generate and bind render buffer ID
  glGenRenderbuffers(1, &rbID);
  glBindRenderbuffer(GL_RENDERBUFFER, rbID);

  // set the render buffer storage
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, WIDTH * 2,
                        HEIGHT);

  // generate the offscreen texture
  glGenTextures(1, &renderTextureID);
  glBindTexture(GL_TEXTURE_2D, renderTextureID);

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH * 2, HEIGHT, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, NULL);

  // bind the renderTextureID as colour attachment of FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         renderTextureID, 0);
  // set the render buffer as the depth attachment of FBO
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, rbID);

  // check for frame buffer completeness status
  GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status == GL_FRAMEBUFFER_COMPLETE) {
    printf("FBO setup succeededa.\n");
  } else {
    printf("Error in FBO setup.\n");
  }
  // unbind the texture and FBO
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderer::_resizeRenderTex() {
  glBindRenderbuffer(GL_RENDERBUFFER, rbID);
  // set the render buffer storage
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, WIDTH * 2,
                        HEIGHT);
  glBindTexture(GL_TEXTURE_2D, renderTextureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH * 2, HEIGHT, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, NULL);
}

void SceneRenderer::initFrameSending(AlSdkAPI *alSdkApi) {
  if (m_debug) {
    std::cout << "SceneRenderer::initFrameSending" << std::endl;
  }
  if (!sendingFrames) {
    newFrameSignal.connect(
        boost::bind(&AlSdkAPI::setImageData, alSdkApi, _1, _2, _3));
  }
  sendingFrames = true;
}

void SceneRenderer::OnStartMouseMove(int initX, int initY) {
  oldRotateX = initX;
  oldRotateY = initY;
  oldPanX = initX;
  oldPanY = initY;
}

void SceneRenderer::onZoom(int deltaZoom) { cam.Zoom(deltaZoom * 100.0f); }

void SceneRenderer::onPan(int x, int y) {
  float dy = float(y - oldPanY) / 5.0f;
  float dx = float(oldPanX - x) / 5.0f;
  if (useFiltering) {
    filterMouseMoves(dx, dy);
  } else {
    mouseX = dx;
    mouseY = dy;
  }
  cam.Pan(mouseX, mouseY);
  oldPanX = x;
  oldPanY = y;
}

void SceneRenderer::onRotate(int x, int y) {
  // seems like when state == 1 it means left button
  rY += (y - oldRotateY) / 5.0f;
  rX += (oldRotateX - x) / 5.0f;
  if (useFiltering) {
    filterMouseMoves(rX, rY);
  } else {
    mouseX = rX;
    mouseY = rY;
  }
  cam.Rotate(mouseX, mouseY, 0);

  oldRotateX = x;
  oldRotateY = y;
}

void SceneRenderer::filterMouseMoves(float dx, float dy) {

  for (int i = MOUSE_HISTORY_BUFFER_SIZE - 1; i > 0; --i) {
    mouseHistory[i] = mouseHistory[i - 1];
  }

  // Store current mouse entry at front of array.
  mouseHistory[0] = glm::vec2(dx, dy);

  float averageX = 0.0f;
  float averageY = 0.0f;
  float averageTotal = 0.0f;
  float currentWeight = 1.0f;

  // Filter the mouse.
  for (int i = 0; i < MOUSE_HISTORY_BUFFER_SIZE; ++i) {
    glm::vec2 tmp = mouseHistory[i];
    averageX += tmp.x * currentWeight;
    averageY += tmp.y * currentWeight;
    averageTotal += 1.0f * currentWeight;
    currentWeight *= MOUSE_FILTER_WEIGHT;
  }

  mouseX = averageX / averageTotal;
  mouseY = averageY / averageTotal;
}

void SceneRenderer::updateRemoteFrame(const uint8_t *image, int width,
                                      int height) {
  m_remoteFrameRenderer.updateFrame(image, width, height);
}

void SceneRenderer::updateLocalFrame(const uint8_t *image, int width,
                                     int height) {
  m_localFrameRenderer.updateFrame(image, width, height);
}

void SceneRenderer::onWinResize(int winWidth, int winHeight) {
  m_winWidth = winWidth;
  m_winHeight = winHeight;
  _updateRenderersPos();
}

void SceneRenderer::_updateRenderersPos() {
  m_remoteFrameRenderer.setPosition(-0.8, -0.8, 0.8, 0.8, m_winWidth,
                                    m_winHeight);
  // 50px from left and 50px from bottom
  // 200 px for reqtangle
  int size = 200;
  VideoStreamRenderer::Borders absoluteBorders;
  absoluteBorders.x2 = float(m_winWidth - 50);
  absoluteBorders.x1 = float(m_winWidth - 50 - size);
  absoluteBorders.y1 = m_winHeight - float(m_winHeight - 50);
  absoluteBorders.y2 = m_winHeight - float(m_winHeight - 50 - size);
  VideoStreamRenderer::Borders relativeBorders =
      VideoStreamRenderer::absoluteToRelative(absoluteBorders, m_winWidth,
                                              m_winHeight);

  m_localFrameRenderer.setPosition(relativeBorders.x1, relativeBorders.y1,
                                   relativeBorders.x2, relativeBorders.y2,
                                   m_winWidth, m_winHeight);
}
