#include "glelements/bottomplane.hpp"

BottomPlane::BottomPlane() {}

BottomPlane::~BottomPlane() {}

void BottomPlane::init() {
  _initShaders();

  // setup triangle vertices
  m_planeVertices[0].position = glm::vec3(-1000, -300, -1000);
  m_planeVertices[1].position = glm::vec3(1000, -300, -1000);
  m_planeVertices[2].position = glm::vec3(1000, -300, 1000);
  m_planeVertices[3].position = glm::vec3(-1000, -300, 1000);
  m_planeVertices[0].texCoord = glm::vec2(0, 0);
  m_planeVertices[1].texCoord = glm::vec2(1, 0);
  m_planeVertices[2].texCoord = glm::vec2(1, 1);
  m_planeVertices[3].texCoord = glm::vec2(0, 1);

  // setup triangle indices
  m_planeIndices[0] = 0;
  m_planeIndices[1] = 1;
  m_planeIndices[2] = 2;
  m_planeIndices[3] = 0;
  m_planeIndices[4] = 2;
  m_planeIndices[5] = 3;

  // setup triangle vao and vbo stuff
  glGenVertexArrays(1, &m_PlaneVaoID);
  glGenBuffers(1, &m_PlaneVboVerticesID);
  glGenBuffers(1, &m_PlaneVboIndicesID);

  GLsizei planeStride = sizeof(PlaneVertex);

  glBindVertexArray(m_PlaneVaoID);

  glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVboVerticesID);
  // pass triangle verteices to buffer object
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_planeVertices), &m_planeVertices[0],
               GL_STATIC_DRAW);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for position
  glEnableVertexAttribArray(m_shader["vVertex"]);
  glVertexAttribPointer(m_shader["vVertex"], 3, GL_FLOAT, GL_FALSE, planeStride,
                        0);
  // GL_CHECK_ERRORS
  // pass indices to element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PlaneVboIndicesID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_planeIndices),
               &m_planeIndices[0], GL_STATIC_DRAW);

  // unbinding
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  _initTextures();
}

void BottomPlane::render(glm::mat4 *MVP) {
  // RENDER BOTTOM PLANE
  glBindVertexArray(m_PlaneVaoID);
  glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVboVerticesID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PlaneVboIndicesID);
  // bind the shader
  m_shader.Use();
  // pass the shader uniform
  glUniformMatrix4fv(m_shader("MVP"), 1, GL_FALSE, glm::value_ptr(*MVP));
  // draw triangle
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
  // unbind the shader
  m_shader.UnUse();
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BottomPlane::_initShaders() {
  // LOADING SHADER FOR PLANE
  m_shader.LoadFromFile(GL_VERTEX_SHADER, "../al_ui/shaders/holoPlane.vert");
  m_shader.LoadFromFile(GL_FRAGMENT_SHADER, "../al_ui/shaders/holoPlane.frag");
  // compile and link shader
  m_shader.CreateAndLinkProgram();
  m_shader.Use();
  // add attributes and uniforms
  // m_shader.AddAttribute("vVertex");
  m_shader.AddAttribute("vTexCoord");
  m_shader.AddUniform("MVP");
  m_shader.UnUse();
}

void BottomPlane::_initTextures() {}
