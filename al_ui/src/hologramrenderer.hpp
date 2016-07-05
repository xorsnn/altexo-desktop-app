#ifndef HOLOGRAMRENDERER_H
#define HOLOGRAMRENDERER_H

#include "AL_CB/al_sensor_cb.hpp"
#include "GLSLShader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

class HologramRenderer : public AlSensorCb {
public:
  HologramRenderer() {}
  ~HologramRenderer() {}

  int init();
  void render();

  // sensor cb
  void newFrame(std::vector<uint8_t> rgbFrame);

private:
  // shader reference
  GLSLShader shader;

  // vertex array and vertex buffer object IDs
  GLuint vaoID;
  GLuint vboVerticesID;
  GLuint vboIndicesID;

  GLuint textureID;

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
  bool m_newFrame;
  int tmpCounter;
};

#endif // HOLOGRAMRENDERER_H
