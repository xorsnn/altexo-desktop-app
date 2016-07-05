#ifndef HOLOGRAMRENDERER_H
#define HOLOGRAMRENDERER_H

#include "GLSLShader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class HologramRenderer {
public:
  HologramRenderer() {}
  ~HologramRenderer() {}

  int init();
  void render();

private:
  // shader reference
  GLSLShader shader;

  // vertex array and vertex buffer object IDs
  GLuint vaoID;
  GLuint vboVerticesID;
  GLuint vboIndicesID;

  // out vertex struct for interleaved attributes
  struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
  };

  // triangle vertices and indices
  Vertex vertices[3];
  GLushort indices[3];

  // projection and modelview matrices
  glm::mat4 P = glm::mat4(1);
  glm::mat4 MV = glm::mat4(1);
};

#endif // HOLOGRAMRENDERER_H
