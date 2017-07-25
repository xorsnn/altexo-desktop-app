#include "alrealsenseplugin.hpp"
#include <iostream>

AlRealsensePlugin::AlRealsensePlugin()
    : m_internalThread(NULL), m_debug(true), m_running(false),
      m_hasNewFrame(false) {}

AlRealsensePlugin::~AlRealsensePlugin() {
  if (m_internalThread->joinable()) {
    // TODO: may be join a little bit earlyer
    m_internalThread->join();
  }
  delete m_internalThread;
  m_internalThread = NULL;
}

void AlRealsensePlugin::init(AlSensorCb *alSensorCb) {
  m_sensorCb = alSensorCb;
  // m_newFrameSignal.connect(
  //     boost::bind(&AlSensorCb::newFrame, m_sensorCb, _1, _2));
  // init in another thread no to delay main prograpm execution
  m_internalThread = new boost::thread(&AlRealsensePlugin::threadMain, this);
}

void AlRealsensePlugin::threadMain() {
  try {
    rs::context ctx;
    m_dev = ctx.get_device(0);
    m_dev->enable_stream(rs::stream::depth, rs::preset::best_quality);
    m_dev->enable_stream(rs::stream::color, rs::preset::best_quality);
    m_dev->start();
    m_running = true;
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
    // m_sensorCb->newFrame(m_colorImage, m_depthImage);
    // std::cout << "ooooo" << std::endl;
    //     m_dev->get_stream_intrinsics(rs::stream::depth);
    // rs::intrinsics depth_intrin =
    // rs::intrinsics color_intrin =
    //     m_dev->get_stream_intrinsics(rs::stream::color);
    // std::cout << depth_intrin.height << std::endl;
    // std::cout << color_intrin.height << std::endl;
    std::cout << m_dev->get_depth_scale() << std::endl;
    std::vector<uint8_t> rgb(m_colorImage, m_colorImage + 480 * 640 * 3);
    std::vector<uint16_t> depth(m_depthImage, m_depthImage + 480 * 640);
    m_sensorCb->newFrame(rgb, depth);
  }
}

void AlRealsensePlugin::start() {
  // this->m_device->startVideo();
  // this->m_device->startDepth();
}

void AlRealsensePlugin::stop() {
  m_running = false;
  // this->m_device->stopVideo();
  // this->m_device->stopDepth();
}
