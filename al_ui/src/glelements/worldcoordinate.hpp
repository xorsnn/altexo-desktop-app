#ifndef WORLDCOORDINATE_H
#define WORLDCOORDINATE_H

#include "GLSLShader.hpp"
#include "glelement.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class WorldCoordinate : public GlElement {
public:
  WorldCoordinate();
  ~WorldCoordinate();
  void init();
  void render(glm::mat4 *MPV);

protected:
  void _initShaders();

private:
  // shader reference
  GLSLShader shader;

  struct Vertex {
    glm::vec3 position;
  };

  Vertex m_vertices[2];
  GLushort m_indices[2];

  GLuint vaoID;
  GLuint vboVerticesID;
  GLuint vboIndicesID;
};

#endif // WORLDCOORDINATE_H
