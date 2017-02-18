#include "alkinectplugin.hpp"
#include <iostream>

#define PNG_DEBUG 3
const float MAX_DEPTH = 1005.0;
const float MIN_DEPTH = 555.0;

void abort_(const char *s, ...) {
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

AlKinectPlugin::AlKinectPlugin() {
  _readPngFile("al_ui/cto_intro.png");

  uint8_t curPixel[3];
  int index = 0;
  for (int y = 0; y < 480; y++) {
    for (int x = 0; x < 1280; x++) {
      // curPixel[0] = m_img[y * x + x * 3];
      // curPixel[1] = m_img[y * x + x * 3 + 1];
      // curPixel[2] = m_img[y * x + x * 3 + 2];
      curPixel[0] = m_img[index * 3];
      curPixel[1] = m_img[index * 3 + 1];
      curPixel[2] = m_img[index * 3 + 2];
      if (x >= 640) {
        int curX = x - 640;
        m_rgb[curX * y + curX * 3] = curPixel[0];
        m_rgb[curX * y + curX * 3 + 1] = curPixel[1];
        m_rgb[curX * y + curX * 3 + 2] = curPixel[2];
      } else {
        AlKinectPlugin::PIXEL color;
        color.R = (1.0 * curPixel[0]) / 255;
        color.G = (1.0 * curPixel[1]) / 255;
        color.B = (1.0 * curPixel[2]) / 255;
        AlKinectPlugin::PIXEL hsl = _rgb2hsl(color);

        float val = hsl.R * (MAX_DEPTH - MIN_DEPTH) + MIN_DEPTH;
        uint16_t intVal = std::lround(val);
        // m_depth[(x - 640) * y + (x - 640)] = intVal;
        m_depth[x * y + x] = 1000;
      }
      index++;
    }
  }
}

AlKinectPlugin::~AlKinectPlugin() {}

AlKinectPlugin::PIXEL AlKinectPlugin::_rgb2hsl(AlKinectPlugin::PIXEL color) {
  float h = 0.0;
  float s = 0.0;
  float l = 0.0;
  float r = color.R;
  float g = color.G;
  float b = color.B;
  float cMin = fmin(r, fmin(g, b));
  float cMax = fmax(r, fmax(g, b));
  l = (cMax + cMin) / 2.0;
  if (cMax > cMin) {
    float cDelta = cMax - cMin;
    // saturation
    if (l < 0.5) {
      s = cDelta / (cMax + cMin);
    } else {
      s = cDelta / (2.0 - (cMax + cMin));
    }
    // hue
    if (r == cMax) {
      h = (g - b) / cDelta;
    } else if (g == cMax) {
      h = 2.0 + (b - r) / cDelta;
    } else {
      h = 4.0 + (r - g) / cDelta;
    }
    if (h < 0.0) {
      h += 6.0;
    }
    h = h / 6.0;
  }
  AlKinectPlugin::PIXEL res;
  res.R = h;
  res.G = s;
  res.B = l;
  return res;
}

void AlKinectPlugin::init(AlSensorCb *alSensorCb) {
  m_sensorCb = alSensorCb;
  m_newFrameSignal.connect(
      boost::bind(&AlSensorCb::newFrame, m_sensorCb, _1, _2));
}

void AlKinectPlugin::requestNewFrame() {
  // std::cout << "AlKinectPlugin::requestNewFrame" << std::endl;
  if (m_sensorCb) {
    // std::cout << "AlKinectPlugin::requestNewFrame" << std::endl;
    std::vector<uint8_t> rgbVec(m_rgb, m_rgb + 640 * 480 * 3);
    std::vector<uint16_t> depthVec(m_depth, m_depth + 640 * 480);
    m_newFrameSignal(rgbVec, depthVec);
    // m_newFrameSignal(this->m_device->getRGB(), this->m_device->getDepth());
  }
}

#ifndef _WIN32
void AlKinectPlugin::_readPngFile(char *file_name) {
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
#endif
