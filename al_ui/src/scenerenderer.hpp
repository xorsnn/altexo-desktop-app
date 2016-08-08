#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "AL_API/sdk_api.hpp"
#include "AL_CB/al_sensor_cb.hpp"
#include "GLSLShader.hpp"
#include "contact.hpp"
#include "glelements/hologramtest.hpp"
#include "glelements/worldcoordinate.hpp"
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

class SceneRenderer {
public:
  SceneRenderer(int winWidth, int winHeight);
  ~SceneRenderer() {}

  void updateResolution(int width, int height);
  int init();
  void render();
  void initFBO();
  void initFrameSending(AlSdkAPI *alSdkApi);

  void OnStartMouseMove(int initX, int initY);
  // mouse move handler
  void OnMouseMove(int x, int y);

  // mouse move filtering function
  void filterMouseMoves(float dx, float dy);

  void onWinResize(int winWidth, int winHeight);

  // Sensor data capturer
  SensorDataFboRenderer m_sensorDataFboRenderer;

  // Plain video stream renderer
  void updateRemoteFrame(const uint8_t *image, int width, int height);
  void updateLocalFrame(const uint8_t *image, int width, int height);

  VideoStreamRenderer m_remoteFrameRenderer;
  VideoStreamRenderer m_localFrameRenderer;
  HologramTest m_hologram;
  // Hologram m_hologram;
  WorldCoordinate m_worldCoordinate;

  bool sendingFrames = false;

private:
  void _updateRenderersPos();
  void _resizeRenderTex();

  bool m_debug;

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

  std::vector<GLubyte> m_outPixel;
  boost::signals2::signal<void(std::vector<GLubyte>, int, int)> newFrameSignal;

  // target camera instance
  CTargetCamera cam;
  // camera tranformation variables
  int state = 1, oldX = 0, oldY = 0;
  float rX = 0, rY = 0;
  // float dist = 10;
  float dist = 3000;

  float mouseX = 0, mouseY = 0; // filtered mouse values

  // flag to enable filtering
  bool useFiltering = true;
  bool pendingRenderTexResize;
};

#endif // SCENERENDERER_H
