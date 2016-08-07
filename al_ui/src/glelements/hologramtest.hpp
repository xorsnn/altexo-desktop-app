#ifndef HOLOGRAMTEST_H
#define HOLOGRAMTEST_H

#include "GLSLShader.hpp"
#include "glelements/hologram.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <png.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

class HologramTest : public Hologram {
public:
  HologramTest();
  void render(glm::mat4 *MPV);

protected:
  void _initTextures();

private:
  void _readPngFile(char *fileName);
  uint8_t m_img[1280 * 480 * 4];
  int x, y;

  int width, height;
  png_byte color_type;
  png_byte bit_depth;

  png_structp png_ptr;
  png_infop info_ptr;
  int number_of_passes;
  png_bytep *row_pointers;
};

#endif // HOLOGRAMTEST_H
