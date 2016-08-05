#include "worldcoordinate.hpp"

WorldCoordinate::WorldCoordinate() {}

WorldCoordinate::~WorldCoordinate() {}

void WorldCoordinate::init() {
  m_vertices[0].position = glm::vec3(0, 0, 0);
  m_vertices[1].position = glm::vec3(0, 0, 1000);

  m_indices[0] = 0;
  m_indices[1] = 1;

  // GL_CHECK_ERRORS
  _initShaders();

  // setup triangle vao and vbo stuff
  glGenVertexArrays(1, &vaoID);
  glGenBuffers(1, &vboVerticesID);
  glGenBuffers(1, &vboIndicesID);
}

void WorldCoordinate::render(glm::mat4 *MPV) {}

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
  shader.AddAttribute("vTexCoord");
  shader.AddUniform("MVP");
  shader.AddUniform("textureMap");
  // pass values of constant uniforms at initialization
  glUniform1i(shader("textureMap"), 4);
  // pass values of constant uniforms at initialization
  shader.UnUse();
  // GL_CHECK_ERRORS
}
