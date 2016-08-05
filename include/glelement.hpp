
#ifndef GLELEMENT_H
#define GLELEMENT_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GlElement {
public:
  virtual ~GlElement() {}
  virtual void init() = 0;
  virtual void render(glm::mat4 MPV) = 0;
};

#endif // GLELEMENT_H
