#include "alrealsenseplugin.hpp"
#include <iostream>

AlRealsensePlugin::AlRealsensePlugin()
    : m_internalThread(NULL), m_running(false), m_hasNewFrame(false) {
  m_resRgb.resize(640 * 480 * 3);
  for (int i = 0; i < 640 * 480 * 3; i++) {
    m_resRgb[i] = 0;
  }
}

AlRealsensePlugin::~AlRealsensePlugin() {
  // #ifdef DEBUG
  std::cout << "AlRealsensePlugin::~AlRealsensePlugin()" << std::endl;
  // #endif

  if (m_internalThread->joinable()) {
    // TODO: may be join a little bit earlyer
    m_internalThread->join();
  }
  delete m_internalThread;
  m_internalThread = NULL;
}

void AlRealsensePlugin::init(AlSensorCb *alSensorCb) {
  m_sensorCb = alSensorCb;
  // init in another thread no to delay main prograpm execution
  m_internalThread = new boost::thread(&AlRealsensePlugin::threadMain, this);
}

void AlRealsensePlugin::threadMain() {
  try {
    rs::context ctx;
    m_dev = ctx.get_device(0);

    // TODO: maybe move to "start()"
    m_dev->enable_stream(rs::stream::depth, rs::preset::best_quality);
    m_dev->enable_stream(rs::stream::color, rs::preset::best_quality);
    m_dev->start();
    // ~

    m_running = true;
    m_sensorCb->onSensorParamsCb(m_dev->get_depth_scale() * 1000.0);
    while (m_running) {
      m_dev->wait_for_frames();
      Mutex::ScopedLock lock(m_mutex);
      m_depthImage = (uint16_t *)m_dev->get_frame_data(rs::stream::depth);
      m_colorImage = (uint8_t *)m_dev->get_frame_data(rs::stream::color);
      m_hasNewFrame = true;
    }
  } catch (std::exception &e) {
    std::cout << "\n";
    std::cout << "Error: " << e.what() << "\n";
  }
}

void AlRealsensePlugin::requestNewFrame() {
  if (m_sensorCb && m_running && m_hasNewFrame) {
    Mutex::ScopedLock lock(m_mutex);
    m_hasNewFrame = false;
    rs::intrinsics depth_intrin =
        m_dev->get_stream_intrinsics(rs::stream::depth);
    rs::extrinsics depth_to_color =
        m_dev->get_extrinsics(rs::stream::depth, rs::stream::color);
    rs::intrinsics color_intrin =
        m_dev->get_stream_intrinsics(rs::stream::color);
    float scale = m_dev->get_depth_scale();

    for (int dy = 0; dy < depth_intrin.height; ++dy) {
      for (int dx = 0; dx < depth_intrin.width; ++dx) {
        // Retrieve the 16-bit depth value and map it into a depth in meters
        uint16_t depth_value = m_depthImage[dy * depth_intrin.width + dx];
        float depth_in_meters = depth_value * scale;

        // Skip over pixels with a depth value of zero, which is used to
        // indicate no data
        if (depth_value == 0)
          continue;

        // Map from pixel coordinates in the depth image to pixel coordinates in
        // the color image
        rs::float2 depth_pixel = {(float)dx, (float)dy};
        rs::float3 depth_point =
            depth_intrin.deproject(depth_pixel, depth_in_meters);
        rs::float3 color_point = depth_to_color.transform(depth_point);
        rs::float2 color_pixel = color_intrin.project(color_point);

        // Use the color from the nearest color pixel, or pure white if this
        // point falls outside the color image
        const int cx = (int)std::round(color_pixel.x),
                  cy = (int)std::round(color_pixel.y);
        if (cx < 0 || cy < 0 || cx >= color_intrin.width ||
            cy >= color_intrin.height) {
          m_resRgb[(dy * depth_intrin.width + dx) * 3] = 0;
          m_resRgb[(dy * depth_intrin.width + dx) * 3 + 1] = 0;
          m_resRgb[(dy * depth_intrin.width + dx) * 3 + 2] = 0;
        } else {
          m_resRgb[(dy * depth_intrin.width + dx) * 3] =
              m_colorImage[(cy * color_intrin.width + cx) * 3];
          m_resRgb[(dy * depth_intrin.width + dx) * 3 + 1] =
              m_colorImage[(cy * color_intrin.width + cx) * 3 + 1];
          m_resRgb[(dy * depth_intrin.width + dx) * 3 + 2] =
              m_colorImage[(cy * color_intrin.width + cx) * 3 + 2];
        }
      }
    }
    std::vector<uint16_t> depth(m_depthImage, m_depthImage + 480 * 640);
    m_sensorCb->newFrame(m_resRgb, depth);
  }
}

void AlRealsensePlugin::start() {}

void AlRealsensePlugin::stop() { m_running = false; }
