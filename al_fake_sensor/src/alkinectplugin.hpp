
#ifndef ALKINECTPLUGIN_H
#define ALKINECTPLUGIN_H

#include "AL_API/sensor_api.hpp"
#include "allogger.hpp"
#include <boost/config.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <string>

#ifndef _WIN32
#include <png.h>
#endif

class AlManager;

class AlKinectPlugin : public AlSensorAPI {
public:
  AlKinectPlugin();
  ~AlKinectPlugin();

  struct PIXEL {
    float R;
    float G;
    float B;
  };

  void init(AlSensorCb *alSensorCb);
  void requestNewFrame();

#ifndef _WIN32
  void _readPngFile(char *fileName);
#endif

private:
  AlKinectPlugin::PIXEL _rgb2hsl(AlKinectPlugin::PIXEL color);

  AlSensorCb *m_sensorCb;
  boost::signals2::signal<void(std::vector<uint8_t> rgbFrame,
                               std::vector<uint16_t> depthFrame)>
      m_newFrameSignal;

  uint8_t m_img[1280 * 480 * 3];
  uint8_t m_rgb[640 * 480 * 3];
  uint16_t m_depth[640 * 480];

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

extern "C" BOOST_SYMBOL_EXPORT AlKinectPlugin plugin;
AlKinectPlugin plugin;

#endif // ALKINECTPLUGIN_H
