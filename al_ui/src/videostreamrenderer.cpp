#include "videostreamrenderer.hpp"

VideoStreamRenderer::VideoStreamRenderer(float x1, float y1, float x2, float y2)
    : m_outPixel(1280 * 480 * 3), m_newFrame(false), m_width(0), m_height(0),
      m_updateSize(false), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2),
      m_winResized(false) {}

int VideoStreamRenderer::init() {
  // GL_CHECK_ERRORS
  // load the shader
  shader.LoadFromFile(GL_VERTEX_SHADER,
                      "../al_ui/shaders/videoStreamRenderer.vert");
  shader.LoadFromFile(GL_FRAGMENT_SHADER,
                      "../al_ui/shaders/videoStreamRenderer.frag");
  // compile and link shader
  shader.CreateAndLinkProgram();
  shader.Use();
  // add attributes and uniforms
  shader.AddAttribute("vVertex");
  shader.AddAttribute("vTexCoord");
  shader.AddUniform("MVP");
  shader.AddUniform("textureMap");
  // pass values of constant uniforms at initialization
  glUniform1i(shader("textureMap"), 4);
  // pass values of constant uniforms at initialization
  shader.UnUse();

  // GL_CHECK_ERRORS

  // setup triangle geometry
  // setup triangle vertices
  vertices[0].texCoord = glm::vec2(0, 0);
  vertices[1].texCoord = glm::vec2(1, 0);
  vertices[2].texCoord = glm::vec2(1, 1);
  vertices[3].texCoord = glm::vec2(0, 1);

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

  _updateVertices();

  // GL_CHECK_ERRORS

  // some tex ==============
  glGenTextures(1, &sensorDepthTexID);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glBindTexture(GL_TEXTURE_2D, 0);
  // =====================

  cout << "Initialization successfull" << endl;
  return 1;
}

void VideoStreamRenderer::render() {
  if (m_winResized) {
    m_winResized = false;
    _updateVertices();
  }
  if (m_newFrame) {
    m_remoteFrameMtx.lock();
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, &(m_remoteFrame[0]));
    m_remoteFrameMtx.unlock();
  }

  glBindVertexArray(vaoID);
  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);

  // bind the shader
  shader.Use();

  // pass the shader uniform
  glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P * MV));

  // draw triangle
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
  // unbind the shader
  shader.UnUse();

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // seems to be needed by something, otherwise some artifacts appears
  glActiveTexture(GL_TEXTURE0);
}

void VideoStreamRenderer::updateFrame(const uint8_t *image, int width,
                                      int height) {
  boost::lock_guard<boost::mutex> guard(m_remoteFrameMtx);
  if (m_width != width || m_height != height) {
    m_width = width;
    m_height = height;
    m_remoteFrame.resize(width * height * 4);
    m_updateSize = true;
  }
  std::copy(image, image + width * height * 4, m_remoteFrame.begin());
  m_newFrame = true;
}

void VideoStreamRenderer::setPosition(float x1, float y1, float x2, float y2) {
  m_x1 = x1;
  m_y1 = y1;
  m_x2 = x2;
  m_y2 = y2;
  m_winResized = true;
}

void VideoStreamRenderer::_updateVertices() {
  vertices[0].position = glm::vec2(m_x1, m_y1);
  vertices[1].position = glm::vec2(m_x2, m_y1);
  vertices[2].position = glm::vec2(m_x2, m_y2);
  vertices[3].position = glm::vec2(m_x1, m_y2);

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
  // enable vertex attribute array for tex coord
  glEnableVertexAttribArray(shader["vTexCoord"]);
  glVertexAttribPointer(shader["vTexCoord"], 2, GL_FLOAT, GL_FALSE, stride,
                        (const GLvoid *)offsetof(Vertex, texCoord));
  // GL_CHECK_ERRORS
  // pass indices to element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
               GL_STATIC_DRAW);
  // unbinding
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
