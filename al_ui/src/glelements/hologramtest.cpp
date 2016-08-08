#include "glelements/hologramtest.hpp"
#define PNG_DEBUG 3

void abort_(const char *s, ...) {
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

HologramTest::HologramTest() { _readPngFile("/home/xors/cto_intro.png"); }

void HologramTest::init() {
  super::init();
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

  GLsizei stride = sizeof(PlaneVertex);

  glBindVertexArray(m_PlaneVaoID);

  glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVboVerticesID);
  // pass triangle verteices to buffer object
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_planeVertices), &m_planeVertices[0],
               GL_STATIC_DRAW);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for position
  glEnableVertexAttribArray(m_plainShader["vVertex"]);
  glVertexAttribPointer(m_plainShader["vVertex"], 3, GL_FLOAT, GL_FALSE, stride,
                        0);
  // GL_CHECK_ERRORS
  // enable vertex attribute array for tex coord
  glEnableVertexAttribArray(m_plainShader["vTexCoord"]);
  glVertexAttribPointer(m_plainShader["vTexCoord"], 2, GL_FLOAT, GL_FALSE,
                        stride, (const GLvoid *)offsetof(Vertex, texCoord));
  // GL_CHECK_ERRORS
  // pass indices to element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PlaneVboIndicesID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_planeIndices),
               &m_planeIndices[0], GL_STATIC_DRAW);
  // unbinding
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void HologramTest::render(glm::mat4 *MVP) {
  glBindVertexArray(m_PlaneVaoID);
  glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVboVerticesID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PlaneVboIndicesID);

  // bind the shader
  m_plainShader.Use();

  // pass the shader uniform
  glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(*MVP));

  // draw triangle
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
  // unbind the shader
  m_plainShader.UnUse();

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  super::render(MVP);
  // seems to be needed by something, otherwise some artifacts appears
  glActiveTexture(GL_TEXTURE0);
}

void HologramTest::_readPngFile(char *file_name) {
  unsigned char header[8]; // 8 is the maximum size that can be checked

  /* open file and test for it being a png */
  FILE *fp = fopen(file_name, "rb");
  if (!fp)
    abort_("[read_png_file] File %s could not be opened for reading",
           file_name);
  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8))
    abort_("[read_png_file] File %s is not recognized as a PNG file",
           file_name);

  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort_("[read_png_file] png_create_read_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during init_io");

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during read_image");

  row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
  for (y = 0; y < height; y++)
    row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));

  png_read_image(png_ptr, row_pointers);

  fclose(fp);
  std::cout << "FILE READ!!!" << std::endl;
  std::cout << width << std::endl;
  std::cout << height << std::endl;

  int index = 0;
  for (y = 0; y < height; y++) {
    png_byte *row = row_pointers[y];
    for (x = 0; x < width; x++) {
      png_byte *ptr = &(row[x * 3]);
      m_img[index * 3] = uint8_t(ptr[0]);
      m_img[index * 3 + 1] = uint8_t(ptr[1]);
      m_img[index * 3 + 2] = uint8_t(ptr[2]);
      index++;
    }
  }
}

void HologramTest::_initShaders() {
  super::_initShaders();

  // LOADING SHADER FOR PLANE
  m_plainShader.LoadFromFile(GL_VERTEX_SHADER,
                             "../al_ui/shaders/holoPlane.vert");
  m_plainShader.LoadFromFile(GL_FRAGMENT_SHADER,
                             "../al_ui/shaders/holoPlane.frag");
  // compile and link shader
  m_plainShader.CreateAndLinkProgram();
  m_plainShader.Use();
  // add attributes and uniforms
  m_plainShader.AddAttribute("vVertex");
  m_plainShader.AddAttribute("vTexCoord");
  m_plainShader.AddUniform("MVP");
  m_plainShader.AddUniform("textureMap");
  // pass values of constant uniforms at initialization
  glUniform1i(m_plainShader("textureMap"), 3);
  // pass values of constant uniforms at initialization
  m_plainShader.UnUse();
}

void HologramTest::_initTextures() {
  glGenTextures(1, &sensorDepthTexID);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  // applying test texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, &(m_img[0]));
}
