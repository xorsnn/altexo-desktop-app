#include "sensordatafborenderer.hpp"
#include "allogger.hpp"

SensorDataFboRenderer::SensorDataFboRenderer()
    : m_debug(true), m_outPixel(0), m_rgbWidth(0), m_rgbHeight(0),
      m_depthWidth(0), m_depthHeight(0), m_videoType(AlSensorCb::RGB), WIDTH(0),
      HEIGHT(0), m_depthScaleK(1), m_updateDepthScaleK(false) {}

SensorDataFboRenderer::~SensorDataFboRenderer() {}

void SensorDataFboRenderer::onUpdateResolution(int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  // TODO: handle all cases
  switch (m_videoType) {
  case AlSensorCb::RGB: {
    m_outPixel.resize(width * width * 2 * 3);
  } break;
  case AlSensorCb::RGBA: {
    m_outPixel.resize(width * width * 2 * 4);
  } break;
  }
}

void SensorDataFboRenderer::readGlFrame() {
  // TODO: handle all cases
  switch (m_videoType) {
  case AlSensorCb::RGB: {
    glReadPixels(0, 0, WIDTH * 2, HEIGHT, GL_BGR, GL_UNSIGNED_BYTE,
                 &(m_outPixel[0]));
  } break;
  case AlSensorCb::RGBA: {
    glReadPixels(0, 0, WIDTH * 2, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE,
                 &(m_outPixel[0]));
  } break;
  }
}

int SensorDataFboRenderer::init() {
  m_newFrame = false;
  // GL_CHECK_ERRORS
  // load the shader
  shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/sensorCompose.vert");
  shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/sensorCompose.frag");
  // compile and link shader
  shader.CreateAndLinkProgram();
  shader.Use();
  // add attributes and uniforms
  shader.AddAttribute("vVertex");
  shader.AddUniform("MVP");

  shader.AddUniform("depthScaleK");

  // m_depthScaleK = 0.000124987 * 1000.0;
  glUniform1f(shader("depthScaleK"), m_depthScaleK);

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

  if (m_updateDepthScaleK) {
    glUniform1f(shader("depthScaleK"), m_depthScaleK);
    m_updateDepthScaleK = false;
  }

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

void SensorDataFboRenderer::newFrame(const uint8_t *rgbFrame,
                                     const uint16_t *depthFrame) {
  if (!m_newFrame) {
    switch (m_videoType) {
    case AlSensorCb::RGB: {
      m_rgbFrame.assign(rgbFrame, rgbFrame + m_rgbWidth * m_rgbHeight * 3);
      m_depthFrame.assign(depthFrame,
                          depthFrame + m_depthWidth * m_depthHeight);
    } break;
    }
  }
}

void SensorDataFboRenderer::onVideoFrameParams(uint rgbWidth, uint rgbHeight,
                                               AlSensorCb::VideoType videoType,
                                               uint depthWidth,
                                               uint depthHeight) {
  m_rgbWidth = rgbWidth;
  m_rgbHeight = rgbHeight;
  m_depthWidth = depthHeight;
  m_depthHeight = depthHeight;
  m_videoType = videoType;
  onUpdateResolution(WIDTH, HEIGHT);
}

void SensorDataFboRenderer::onSensorParamsCb(float depthScaleK) {
  m_depthScaleK = depthScaleK;
  m_updateDepthScaleK = true;
}
