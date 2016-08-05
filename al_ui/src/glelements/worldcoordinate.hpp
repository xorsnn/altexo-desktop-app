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
  void render(glm::mat4 MPV);

private:
  struct Vertex {
    glm::vec2 position;
    // glm::vec2 texCoord;
  };
};

#endif // WORLDCOORDINATE_H
