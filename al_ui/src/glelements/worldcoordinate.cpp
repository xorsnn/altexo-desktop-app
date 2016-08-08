#include "glelements/worldcoordinate.hpp"

WorldCoordinate::WorldCoordinate() {}

WorldCoordinate::~WorldCoordinate() {}

void WorldCoordinate::init() {
  m_vertices[0].position = glm::vec3(0, 0, 0);
  m_vertices[1].position = glm::vec3(0, 0, 1000);
  m_vertices[2].position = glm::vec3(0, 0, 0);
  m_vertices[3].position = glm::vec3(0, 1000, 0);
  m_vertices[4].position = glm::vec3(0, 0, 0);
  m_vertices[5].position = glm::vec3(1000, 0, 0);

  m_vertices[0].color = glm::vec3(1, 0, 0);
  m_vertices[1].color = glm::vec3(1, 0, 0);
  m_vertices[2].color = glm::vec3(0, 1, 0);
  m_vertices[3].color = glm::vec3(0, 1, 0);
  m_vertices[4].color = glm::vec3(0, 0, 1);
  m_vertices[5].color = glm::vec3(0, 0, 1);

  m_indices[0] = 0;
  m_indices[1] = 1;
  m_indices[2] = 2;
  m_indices[3] = 3;
  m_indices[4] = 4;
  m_indices[5] = 5;

  // GL_CHECK_ERRORS
  _initShaders();

  // setup triangle vao and vbo stuff
  glGenVertexArrays(1, &vaoID);
  glGenBuffers(1, &vboVerticesID);
  glGenBuffers(1, &vboIndicesID);

  GLsizei stride = sizeof(Vertex);

  glBindVertexArray(vaoID);

  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
  // pass triangle verteices to buffer object
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices[0],
               GL_STATIC_DRAW);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for position
  glEnableVertexAttribArray(shader["vVertex"]);
  glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, stride, 0);
  // enable vertex attribute array for colour
  glEnableVertexAttribArray(shader["vColor"]);
  glVertexAttribPointer(shader["vColor"], 3, GL_FLOAT, GL_FALSE, stride,
                        (const GLvoid *)offsetof(Vertex, color));
  // GL_CHECK_ERRORS
  // pass indices to element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), &m_indices[0],
               GL_STATIC_DRAW);
  // unbinding
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void WorldCoordinate::render(glm::mat4 *MVP) {
  // @gl.drawElements(@gl.LINE_STRIP, @count*3, @gl.UNSIGNED_SHORT, 0)
  glBindVertexArray(vaoID);
  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);

  // bind the shader
  shader.Use();

  // pass the shader uniform
  glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(*MVP));

  glDrawElements(GL_LINES, 6, GL_UNSIGNED_SHORT, 0);
  // unbind the shader
  shader.UnUse();

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // seems to be needed by something, otherwise some artifacts appears
  glActiveTexture(GL_TEXTURE0);
}

void WorldCoordinate::_initShaders() {
  // load the shader
  shader.LoadFromFile(GL_VERTEX_SHADER,
                      "../al_ui/shaders/worldCoordinates.vert");
  shader.LoadFromFile(GL_FRAGMENT_SHADER,
                      "../al_ui/shaders/worldCoordinates.frag");
  // compile and link shader
  shader.CreateAndLinkProgram();
  shader.Use();
  // add attributes and uniforms
  shader.AddAttribute("vVertex");
  shader.AddAttribute("vColor");
  shader.AddUniform("MVP");
  // pass values of constant uniforms at initialization
  shader.UnUse();
  // GL_CHECK_ERRORS
}
