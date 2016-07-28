#ifndef HOLOGRAMRENDERER_H
#define HOLOGRAMRENDERER_H

#include "AL_API/sdk_api.hpp"
#include "AL_CB/al_sensor_cb.hpp"
#include "GLSLShader.hpp"
#include "contact.hpp"
#include "sensordatafborenderer.hpp"
#include "targetcamera.h"
#include <boost/signals2/signal.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

const int MOUSE_HISTORY_BUFFER_SIZE = 10;
const float MOUSE_FILTER_WEIGHT = 0.75f;

class HologramRenderer {
public:
  HologramRenderer();
  ~HologramRenderer() {}

  void updateResolution(int width, int height);
  int init();
  void render(int viewWidh, int viewHeight);
  void initFBO();
  void resizeRenderTex();
  void initFrameSending(AlSdkAPI *alSdkApi) {
    if (m_debug) {
      std::cout << "HologramRenderer::initFrameSending" << std::endl;
    }
    if (!sendingFrames) {
      newFrameSignal.connect(
          boost::bind(&AlSdkAPI::setImageData, alSdkApi, _1, _2, _3));
    }
    sendingFrames = true;
  }

  void OnStartMouseMove(int initX, int initY) {
    oldX = initX;
    oldY = initY;
  }
  // mouse move handler
  void OnMouseMove(int x, int y) {

    if (state == 0) {
      dist = (y - oldY) / 5.0f;
      cam.Zoom(dist);
    } else if (state == 2) {
      float dy = float(y - oldY) / 100.0f;
      float dx = float(oldX - x) / 100.0f;
      if (useFiltering) {
        // filterMouseMoves(dx, dy);
      } else {
        mouseX = dx;
        mouseY = dy;
      }

      cam.Pan(mouseX, mouseY);
    } else {
      rY += (y - oldY) / 5.0f;
      rX += (oldX - x) / 5.0f;
      if (useFiltering) {
        filterMouseMoves(rX, rY);
      } else {
        mouseX = rX;
        mouseY = rY;
      }
      cam.Rotate(mouseX, mouseY, 0);
    }
    oldX = x;
    oldY = y;

    // glutPostRedisplay();
  }
  // mouse move filtering function
  void filterMouseMoves(float dx, float dy) {
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

  // Sensor data capturer
  SensorDataFboRenderer m_sensorDataFboRenderer;

  bool sendingFrames = false;

private:
  bool m_debug;

  // mouse history buffer
  glm::vec2 mouseHistory[MOUSE_HISTORY_BUFFER_SIZE];

  // shader reference
  GLSLShader shader;

  // vertex array and vertex buffer object IDs
  GLuint vaoID;
  GLuint vboVerticesID;
  GLuint vboIndicesID;

  GLuint sensorRGBTexID;
  GLuint sensorDepthTexID;

  // out vertex struct for interleaved attributes
  struct Vertex {
    glm::vec2 position;
    glm::vec2 coord;
  };

  // triangle vertices and indices
  Vertex vertices[320 * 240];

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

  std::vector<GLubyte> m_outPixel;
  boost::signals2::signal<void(std::vector<GLubyte>, int, int)> newFrameSignal;

  // target camera instance
  CTargetCamera cam;
  // camera tranformation variables
  int state = 1, oldX = 0, oldY = 0;
  float rX = 0, rY = 0, dist = 10;

  float mouseX = 0, mouseY = 0; // filtered mouse values

  // flag to enable filtering
  bool useFiltering = true;
  bool pendingRenderTexResize;
};

#endif // HOLOGRAMRENDERER_H
