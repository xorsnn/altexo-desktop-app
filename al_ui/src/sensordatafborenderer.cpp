#include "sensordatafborenderer.hpp"

SensorDataFboRenderer::SensorDataFboRenderer() : m_debug(true) {}

SensorDataFboRenderer::~SensorDataFboRenderer() {}

int SensorDataFboRenderer::init() {
  m_newFrame = false;
  // GL_CHECK_ERRORS
  // load the shader
  shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/sensorCompose.vert");
  shader.LoadFromFile(GL_FRAGMENT_SHADER,
                      "shaders/sensorCompose.frag");
  // compile and link shader
  shader.CreateAndLinkProgram();
  shader.Use();
  // add attributes and uniforms
  shader.AddAttribute("vVertex");
  shader.AddUniform("MVP");

  shader.AddUniform("textureMap");
  // pass values of constant uniforms at initialization
  glUniform1i(shader("textureMap"), 0);

  shader.AddUniform("depthTexMap");
  // pass values of constant uniforms at initialization
  glUniform1i(shader("depthTexMap"), 1);

  shader.UnUse();

  // GL_CHECK_ERRORS

  // setup triangle geometry
  // setup triangle vertices

  vertices[0].position = glm::vec2(-1.0, -1.0);
  vertices[1].position = glm::vec2(1.0, -1.0);
  vertices[2].position = glm::vec2(1.0, 1.0);
  vertices[3].position = glm::vec2(-1.0, 1.0);

  // setup triangle indices
  indices[0] = 0;
  indices[1] = 1;
  indices[2] = 2;
  indices[3] = 0;
  indices[4] = 2;
  indices[5] = 3;

  // GL_CHECK_ERRORS

  // setup triangle vao and vbo stuff
  glGenVertexArrays(1, &vaoID);
  glGenBuffers(1, &vboVerticesID);
  glGenBuffers(1, &vboIndicesID);
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
  // pass indices to element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
               GL_STATIC_DRAW);
  // GL_CHECK_ERRORS

  // unbinding
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // setup OpenGL texture and bind to texture unit 0
  glGenTextures(1, &sensorRGBTexID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sensorRGBTexID);
  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
  // depth tex
  glGenTextures(1, &sensorDepthTexID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  // set texture parameters
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (m_debug) {
    cout << "Initialization successfull" << endl;
  }
  return 1;
}

void SensorDataFboRenderer::render(int viewWidth, int viewHeight) {
  glViewport(0, 0, viewWidth, viewHeight);
  glBindVertexArray(vaoID);
  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);

  if (m_newFrame) {
    // allocate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sensorRGBTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH_SENSOR, HEIGHT_SENSOR, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, m_rgbFrame.data());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, WIDTH_SENSOR, HEIGHT_SENSOR, 0,
                 GL_RED_INTEGER, GL_UNSIGNED_SHORT, m_depthFrame.data());

    m_newFrame = false;
  }

  // bind the shader
  shader.Use();

  // pass the shader uniform
  glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P * MV));

  // drwa triangle
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

  // unbind the shader
  shader.UnUse();

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SensorDataFboRenderer::newFrame(std::vector<uint8_t> rgbFrame,
                                     std::vector<uint16_t> depthFrame) {
  if (!m_newFrame) {
    m_rgbFrame.swap(rgbFrame);
    m_depthFrame.swap(depthFrame);
    m_newFrame = true;
  }
}
