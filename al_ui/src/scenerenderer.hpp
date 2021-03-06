#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "AL_API/sdk_api.hpp"
#include "AL_CB/al_sensor_cb.hpp"
#include "GLSLShader.hpp"
#include "allogger.hpp"
#include "contact.hpp"
#include "glelements/bottomplane.hpp"
#include "glelements/hologram.hpp"
// #include "glelements/hologramtest.hpp"
#include "altypes.hpp"
#include "glelements/worldcoordinate.hpp"
#include "scenerenderercb.hpp"
#include "sensordatafborenderer.hpp"
#include "targetcamera.h"
#include "videostreamrenderer.hpp"
#include <boost/signals2/signal.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

const int MOUSE_HISTORY_BUFFER_SIZE = 10;
const float MOUSE_FILTER_WEIGHT = 0.75f;

class SceneRenderer : public SceneRendererCb {
public:
  SceneRenderer(int winWidth, int winHeight);
  ~SceneRenderer() {}

  void setRemoteStreamMode(al::VideoMode mode);

  int init();
  void render();
  void initFBO();
  void initFrameSending(AlSdkAPI *alSdkApi);

  void OnStartMouseMove(int initX, int initY);
  void onZoom(int deltaZoom);
  void onPan(int x, int y);
  void onRotate(int x, int y);

  // mouse move filtering function
  void filterMouseMoves(float dx, float dy);

  void onWinResize(int winWidth, int winHeight);

  /*
   * SceneRendererCb implemetatiopn
   */

  // Plain video stream renderer
  void updateRemoteFrameCb(const uint8_t *image, int width, int height);
  void updateLocalFrameCb(const uint8_t *image, int width, int height);
  void setLocalStreamModeCb(al::VideoMode mode);
  void updateResolutionCb(int width, int height);

  /*
   * Attribute
   */
  // Sensor data capturer
  SensorDataFboRenderer m_sensorDataFboRenderer;

  VideoStreamRenderer m_remoteFrameRenderer;
  VideoStreamRenderer m_localFrameRenderer;
  // HologramTest m_hologram;
  Hologram m_hologram;
  BottomPlane m_bottomPlane;
  WorldCoordinate m_worldCoordinate;

  bool sendingFrames = false;

private:
  bool m_debug;

  void _updateRenderersPos();
  void _resizeRenderTex();

  al::VideoMode m_remoteStreamMode;
  al::VideoMode m_localStreamMode;

  // mouse history buffer
  glm::vec2 mouseHistory[MOUSE_HISTORY_BUFFER_SIZE];

  // GLuint vboIndicesID;

  // TODO remove unnesessary, rename used
  // GLuint sensorRGBTexID;

  // internal data
  std::vector<uint8_t> m_rgbFrame;
  std::vector<uint16_t> m_depthFrame;
  bool m_newFrame;

  // FBO and render buffer object ID
  GLuint fboID, rbID;
  // offscreen render texture ID
  GLuint renderTextureID;

  // rendering data resolution
  int WIDTH;
  int HEIGHT;

  int m_winWidth;
  int m_winHeight;

  boost::signals2::signal<void(std::vector<GLubyte>, int, int)> newFrameSignal;

  // target camera instance
  CTargetCamera cam;
  // camera tranformation variables
  int state = 1;
  int oldRotateX = 0, oldRotateY = 0;
  int oldPanX = 0, oldPanY = 0;
  float rX = 0, rY = 0;

  float mouseX = 0, mouseY = 0; // filtered mouse values

  // flag to enable filtering
  // bool useFiltering = true;
  bool useFiltering = false;
  bool pendingRenderTexResize;
};

#endif // SCENERENDERER_H
