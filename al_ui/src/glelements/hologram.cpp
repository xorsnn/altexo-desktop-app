#include "glelements/hologram.hpp"

Hologram::Hologram() {}

Hologram::~Hologram() {}

void Hologram::init() {
  _initShaders();
  int wAmount = 320;
  int hAmount = 240;
  int normConst = wAmount;
  int index = 0;
  for (int yCoord = 0; yCoord < hAmount; yCoord++) {
    for (int xCoord = 0; xCoord < wAmount; xCoord++) {
      vertices[index].position = glm::vec2(
          float(xCoord) / normConst - (0.5 * float(wAmount) / normConst),
          float(yCoord) / normConst - (0.5 * float(wAmount) / normConst));
      vertices[index].texCoord =
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for position
  glEnableVertexAttribArray(shader["vVertex"]);
  glVertexAttribPointer(shader["vVertex"], 2, GL_FLOAT, GL_FALSE, stride, 0);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for colour
  glEnableVertexAttribArray(shader["vTexCoord"]);
  glVertexAttribPointer(shader["vTexCoord"], 2, GL_FLOAT, GL_FALSE, stride,
                        (const GLvoid *)offsetof(Vertex, texCoord));
  // GL_CHECK_ERRORS

  // unbinding
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  _initTextures();
}

void Hologram::render(glm::mat4 *MVP) {
  glBindVertexArray(vaoID);
  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);

  // bind the shader
  shader.Use();

  // pass the shader uniform
  glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(*MVP));

  // drwa triangle
  glEnable(GL_PROGRAM_POINT_SIZE);
  glDrawArrays(GL_POINTS, 0, 320 * 240);
  // unbind the shader
  shader.UnUse();
}

void Hologram::_initShaders() {
  // load the shader
  shader.LoadFromFile(GL_VERTEX_SHADER,
                      "../al_ui/shaders/hologramRenderer.vert");
  shader.LoadFromFile(GL_FRAGMENT_SHADER,
                      "../al_ui/shaders/hologramRenderer.frag");
  // compile and link shader
  shader.CreateAndLinkProgram();
  shader.Use();
  // add attributes and uniforms
  shader.AddAttribute("vVertex");
  shader.AddAttribute("vTexCoord");
  shader.AddUniform("MVP");
  shader.AddUniform("textureMap");
  // pass values of constant uniforms at initialization
  glUniform1i(shader("textureMap"), 3);
  // pass values of constant uniforms at initialization
  shader.UnUse();
}

void Hologram::_initTextures() {
  // some tex ==============
  glGenTextures(1, &sensorDepthTexID);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glBindTexture(GL_TEXTURE_2D, 0);
  // =====================
}
