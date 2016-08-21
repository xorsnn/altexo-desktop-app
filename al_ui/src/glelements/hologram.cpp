#include "glelements/hologram.hpp"

Hologram::Hologram() { m_vertices = new Vertex[wAmount * hAmount]; }

Hologram::~Hologram() { delete m_vertices; }

void Hologram::init() {
  _initShaders();
  int normConst = wAmount;
  int index = 0;
  for (int yCoord = 0; yCoord < hAmount; yCoord++) {
    for (int xCoord = 0; xCoord < wAmount; xCoord++) {
      m_vertices[index].position =
          glm::vec2((float(xCoord) - (0.5 * float(wAmount))) / normConst,
                    (float(yCoord) - (0.5 * float(hAmount))) / normConst);
      m_vertices[index].texCoord =
          glm::vec2(float(xCoord) / wAmount, float(yCoord) / hAmount);
      index++;
    }
  }
  // setup triangle vao and vbo stuff
  glGenVertexArrays(1, &vaoID);
  glGenBuffers(1, &vboVerticesID);
  // glGenBuffers(1, &vboIndicesID);
  GLsizei stride = sizeof(Vertex);

  glBindVertexArray(vaoID);

  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
  // pass triangle verteices to buffer object
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices[0],
               GL_STATIC_DRAW);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for position
  glEnableVertexAttribArray(m_shader["vVertex"]);
  glVertexAttribPointer(m_shader["vVertex"], 2, GL_FLOAT, GL_FALSE, stride, 0);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for colour
  glEnableVertexAttribArray(m_shader["vTexCoord"]);
  glVertexAttribPointer(m_shader["vTexCoord"], 2, GL_FLOAT, GL_FALSE, stride,
                        (const GLvoid *)offsetof(Vertex, texCoord));
  // GL_CHECK_ERRORS

  // unbinding
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  _initTextures();
}

void Hologram::render(glm::mat4 *MVP) {
  // RENDER HOLOGRAM
  glActiveTexture(GL_TEXTURE3);
  // TODO: take a look this should be an FBO texture
  // glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  glBindVertexArray(vaoID);
  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
  // bind the shader
  m_shader.Use();
  // pass the shader uniform
  glUniformMatrix4fv(m_shader("MVP"), 1, GL_FALSE, glm::value_ptr(*MVP));
  // drwa triangle
  glEnable(GL_PROGRAM_POINT_SIZE);
  glDrawArrays(GL_POINTS, 0, 320 * 240);
  // unbind the shader
  m_shader.UnUse();
}

void Hologram::_initShaders() {
  // load the shader
  m_shader.LoadFromFile(GL_VERTEX_SHADER,
                        "shaders/hologramRenderer.vert");
  m_shader.LoadFromFile(GL_FRAGMENT_SHADER,
                        "shaders/hologramRenderer.frag");
  // compile and link shader
  m_shader.CreateAndLinkProgram();
  m_shader.Use();
  // add attributes and uniforms
  m_shader.AddAttribute("vVertex");
  m_shader.AddAttribute("vTexCoord");
  m_shader.AddUniform("MVP");
  m_shader.AddUniform("textureMap");
  // pass values of constant uniforms at initialization
  glUniform1i(m_shader("textureMap"), 3);
  // pass values of constant uniforms at initialization
  m_shader.UnUse();
}

void Hologram::_initTextures() {
  // some tex ==============
  glGenTextures(1, &sensorDepthTexID);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
  // =====================
}
