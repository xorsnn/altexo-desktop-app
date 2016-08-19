#ifndef HOLOGRAMTEST_H
#define HOLOGRAMTEST_H

#include "GLSLShader.hpp"
#include "glelements/hologram.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#ifndef _WIN32
#include <png.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// TODO: figure out if needed
// #include <unistd.h>

class HologramTest : public Hologram {
public:
  HologramTest();
  void render(glm::mat4 *MPV);
  void init();

protected:
  void _initShaders();
  void _initTextures();

private:
  typedef Hologram super;

#ifndef _WIN32
  void _readPngFile(char *fileName);
#endif

  uint8_t m_img[1280 * 480 * 3];
  int x, y;

  int width, height;

#ifndef _WIN32
  png_byte color_type;
  png_byte bit_depth;

  png_structp png_ptr;
  png_infop info_ptr;
  int number_of_passes;
  png_bytep *row_pointers;
#endif
};

#endif // HOLOGRAMTEST_H
