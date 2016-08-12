#ifndef VIDEOSTREAMRENDERER_H
#define VIDEOSTREAMRENDERER_H

#include "AL_API/sdk_api.hpp"
#include "AL_CB/al_sensor_cb.hpp"
#include "GLSLShader.hpp"
#include "contact.hpp"
#include "sensordatafborenderer.hpp"
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

class VideoStreamRenderer {
public:
  VideoStreamRenderer(float x1, float y1, float x2, float y2);
  ~VideoStreamRenderer() {}

  struct Borders {
    float x1;
    float x2;
    float y1;
    float y2;
  };

  static Borders absoluteToRelative(Borders absoluteBorders, int winWidth,
                                    int winHeight);
  static Borders relativeToAbsolute(Borders relativeBorders, int winWidth,
                                    int winHeight);

  int init();
  void render();
  void setPosition(float x1, float y1, float x2, float y2, int winWidth,
                   int winHeight);
  void updateFrame(const uint8_t *image, int width, int height);
  // void initFBO();
  void bindToTex();

private:
  void _updateVertices();

  float m_x1, m_y1, m_x2, m_y2;
  float m_winWidth, m_winHeight;

  // shader reference
  GLSLShader shader;

  // vertex array and vertex buffer object IDs
  GLuint vaoID;
  GLuint vboVerticesID;
  GLuint vboIndicesID;

  GLuint sensorDepthTexID;

  // out vertex struct for interleaved attributes
  struct Vertex {
    glm::vec2 position;
    glm::vec2 texCoord;
  };

  // triangle vertices and indices
  Vertex vertices[4];
  GLushort indices[6];

  // projection and modelview matrices
  glm::mat4 P = glm::mat4(1);
  glm::mat4 MV = glm::mat4(1);

  // screen resolution
  std::vector<GLubyte> m_outPixel;
  boost::signals2::signal<void(std::vector<GLubyte>, int, int)> newFrameSignal;
  int m_width;
  int m_height;

  boost::mutex m_remoteFrameMtx;
  bool m_newFrame;
  bool m_updateSize;
  std::vector<uint8_t> m_remoteFrame;
};

#endif // VIDEOSTREAMRENDERER_H
