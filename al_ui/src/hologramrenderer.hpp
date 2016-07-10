#ifndef HOLOGRAMRENDERER_H
#define HOLOGRAMRENDERER_H

#include "AL_CB/al_sensor_cb.hpp"
#include "GLSLShader.hpp"
#include "contact.hpp"
#include "sensordatafborenderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

class HologramRenderer {
public:
  HologramRenderer() {}
  ~HologramRenderer() {}

  int init();
  void render(int viewWidh, int viewHeight);

  void initFBO();
  // Sensor data capturer
  SensorDataFboRenderer m_sensorDataFboRenderer;

private:
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
    glm::vec3 position;
    glm::vec3 color;
  };

  // triangle vertices and indices
  Vertex vertices[4];
  GLushort indices[6];

  // projection and modelview matrices
  glm::mat4 P = glm::mat4(1);
  glm::mat4 MV = glm::mat4(1);

  // internal data
  std::vector<uint8_t> m_rgbFrame;
  std::vector<uint16_t> m_depthFrame;
  bool m_newFrame;

  // FBO and render buffer object ID
  GLuint fboID, rbID;
  // offscreen render texture ID
  GLuint renderTextureID;
  // screen resolution
  const int WIDTH = 1280;
  // const int HEIGHT = 960;
  const int HEIGHT = 480;
};

#endif // HOLOGRAMRENDERER_H
