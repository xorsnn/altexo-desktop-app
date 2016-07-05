#include "hologramrenderer.hpp"

int HologramRenderer::init() {
  m_newFrame = false;
  tmpCounter = 1;
  // GL_CHECK_ERRORS
  // load the shader
  std::cout << "< 1" << std::endl;
  // shader.LoadFromFile(GL_VERTEX_SHADER, "../shaders/shader.vert");
  shader.LoadFromFile(GL_VERTEX_SHADER, "../shaders/shader.vert");
  std::cout << "< 2" << std::endl;
  shader.LoadFromFile(GL_FRAGMENT_SHADER, "../shaders/shader.frag");
  std::cout << "< 3" << std::endl;
  // compile and link shader
  shader.CreateAndLinkProgram();
  shader.Use();
  // add attributes and uniforms
  shader.AddAttribute("vVertex");
  shader.AddAttribute("vColor");
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
  vertices[0].color = glm::vec3(1, 0, 0);
  vertices[1].color = glm::vec3(0, 1, 0);
  vertices[2].color = glm::vec3(0, 0, 1);
  vertices[3].color = glm::vec3(0, 1, 1);

  vertices[0].position = glm::vec3(-0.5, -0.5, 0);
  vertices[1].position = glm::vec3(0.5, -0.5, 0);
  vertices[2].position = glm::vec3(0.5, 0.5, 0);
  vertices[3].position = glm::vec3(-0.5, 0.5, 0);

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
  glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, stride, 0);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for colour
  glEnableVertexAttribArray(shader["vColor"]);
  glVertexAttribPointer(shader["vColor"], 3, GL_FLOAT, GL_FALSE, stride,
                        (const GLvoid *)offsetof(Vertex, color));
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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glBindTexture(GL_TEXTURE_2D, 0);
  // depth tex
  glGenTextures(1, &sensorDepthTexID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glBindTexture(GL_TEXTURE_2D, 0);

  cout << "Initialization successfull" << endl;
  return 1;
}

void HologramRenderer::render() {
  // // clear the colour and depth buffer
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(vaoID);
  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sensorRGBTexID);

  if (m_newFrame && tmpCounter < 100) {
    // allocate texture
    int w = 640;
    int h = 480;

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, sensorRGBTexID);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
    //              m_rgbFrame.data());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, w, h, 0, GL_RED, GL_UNSIGNED_SHORT,
                 m_depthFrame.data());

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
  glBindTexture(GL_TEXTURE_2D, 0);
}

void HologramRenderer::newFrame(std::vector<uint8_t> rgbFrame,
                                std::vector<uint16_t> depthFrame) {
  if (!m_newFrame) {
    m_rgbFrame.swap(rgbFrame);
    m_depthFrame.swap(depthFrame);
    m_newFrame = true;
  }
  // tmpCounter++;
}
