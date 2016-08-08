#ifndef HOLOGRAM_H
#define HOLOGRAM_H

#include "GLSLShader.hpp"
#include "glelement.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Hologram : public GlElement {
public:
  Hologram();
  ~Hologram();
  void init();
  void render(glm::mat4 *MPV);

protected:
  void _initShaders();
  void _initTextures();

  // shader reference
  GLSLShader shader;

  // vertex array and vertex buffer object IDs
  GLuint vaoID;
  GLuint vboVerticesID;

  GLuint sensorDepthTexID;

  // out vertex struct for interleaved attributes
  struct Vertex {
    glm::vec2 position;
    glm::vec2 texCoord;
  };

  // triangle vertices and indices
  Vertex vertices[320 * 240];

  // plane
  struct PlaneVertex {
    glm::vec3 position;
    glm::vec2 texCoord;
  };
  PlaneVertex m_planeVertices[4];
  GLushort m_planeIndices[6];
  GLuint m_PlaneVaoID;
  GLuint m_PlaneVboVerticesID;
  GLuint m_PlaneVboIndicesID;
  // shader reference
  GLSLShader m_plainShader;
};

#endif // HOLOGRAM_H
