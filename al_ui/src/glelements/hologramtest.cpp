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

HologramTest::HologramTest() { _readPngFile("/home/xors/cto_intro1.png"); }

void HologramTest::render(glm::mat4 *MVP) {

  glBindVertexArray(vaoID);
  glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
  // bind the shader
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  shader.Use();
  // pass the shader uniform
  glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(*MVP));
  // draw points
  glEnable(GL_PROGRAM_POINT_SIZE);
  glDrawArrays(GL_POINTS, 0, 320 * 240);
  // unbind the shader
  shader.UnUse();
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
  std::cout << int(color_type) << std::endl;
  std::cout << width << std::endl;
  std::cout << height << std::endl;
  if (color_type == PNG_COLOR_TYPE_RGB) {
    std::cout << "FILE RGB!!!" << std::endl;
  }
  int index = 0;
  for (y = 0; y < height; y++) {
    png_byte *row = row_pointers[y];
    for (x = 0; x < width; x++) {
      png_byte *ptr = &(row[x * 4]);
      m_img[index * 4] = uint8_t(ptr[0]);
      m_img[index * 4 + 1] = uint8_t(ptr[1]);
      m_img[index * 4 + 2] = uint8_t(ptr[2]);
      m_img[index * 4 + 3] = uint8_t(ptr[3]);
      index++;
      //     printf(
      //         "Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d -
      //         %d\n",
      //         x, y, ptr[0], ptr[1], ptr[2], ptr[3]);
      //
      //     /* set red value to 0 and green value to the blue one */
      //     // ptr[0] = 0;
      //     // ptr[1] = ptr[2];
    }
  }
  // for (int i = 0; i < width * height * 3; i++) {
  //   if (m_img[i] > 0) {
  //     std::cout << int(m_img[i]) << std::endl;
  //   }
  // }
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
  // glBindTexture(GL_TEXTURE_2D, 0);
  // glActiveTexture(GL_TEXTURE3);
  // glBindTexture(GL_TEXTURE_2D, sensorDepthTexID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, &(m_img[0]));
}
