#ifndef BOTTOMPLANE_H
#define BOTTOMPLANE_H

#include "GLSLShader.hpp"
#include "glelement.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BottomPlane : public GlElement {
public:
  BottomPlane();
  ~BottomPlane();
  void init();
  void render(glm::mat4 *MPV);

protected:
  void _initShaders();
  void _initTextures();
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
  GLSLShader m_shader;
};

#endif // BOTTOMPLANE_H
