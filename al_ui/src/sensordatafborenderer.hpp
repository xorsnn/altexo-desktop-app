
#ifndef SENSORDATAFBORENDERER_H
#define SENSORDATAFBORENDERER_H

#include "AL_CB/al_sensor_cb.hpp"
#include "GLSLShader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

class SensorDataFboRenderer : public AlSensorCb {
public:
  SensorDataFboRenderer();
  ~SensorDataFboRenderer();

  void onUpdateResolution(int width, int height);

  void readGlFrame();

  int init();

  void render(int viewWidth, int viewHeigt);

  /*
   * AlSensorCb implementation
   */
  // TODO: deprecated
  void newFrame(std::vector<uint8_t> rgbFrame,
                std::vector<uint16_t> depthFrame);

  void newFrame(const uint8_t *rgbFrame, const uint16_t *depthFrame);

  void onVideoFrameParams(unsigned int rgbWidth, unsigned int rgbHeight,
                          AlSensorCb::VideoType videoType, unsigned int depthWidth,
                          unsigned int depthHeight);

  void onSensorParamsCb(float depthScaleK);

  // renderered frame
  std::vector<GLubyte> m_outPixel;

private:
  bool m_debug;
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
  };

  // triangle vertices and indices
  Vertex vertices[4];
  GLushort indices[6];

  // projection and modelview matrices
  glm::mat4 MV = glm::mat4(1);
  glm::mat4 P = glm::mat4(1);

  // internal data
  std::vector<uint8_t> m_rgbFrame;
  std::vector<uint16_t> m_depthFrame;
  bool m_newFrame;

  /*
   * Sensor snapshot parameters
   */
  unsigned int m_rgbWidth;
  unsigned int m_rgbHeight;
  unsigned int m_depthWidth;
  unsigned int m_depthHeight;
  AlSensorCb::VideoType m_videoType;

  // shader params
  float m_depthScaleK;
  bool m_updateDepthScaleK;

  /*
   * TODO: remove this, window width and height
   */
  int WIDTH;
  int HEIGHT;

  // sensor data resolution
  // this depends from sensor
  const int WIDTH_SENSOR = 640;
  const int HEIGHT_SENSOR = 480;
};

#endif // SENSORDATAFBORENDERER_H
