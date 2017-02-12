#include "alsdkplugin.hpp"
#include "alcallback.h"
#include "almanager.h"
#include <iostream>

using namespace std;

AlSdkPlugin::AlSdkPlugin()
    : m_internalThread(NULL), m_debug(true), m_videoDeviceName(""), WIDTH(0),
      HEIGHT(0) {

  // if (m_debug) {
  //   std::cout << "AlSdkPlugin::AlSdkPlugin()" << std::endl;
  // }
  // m_sdk = new AlSdk();
  // m_sdk->run();
  m_manager = new AlManager();
}

AlSdkPlugin::~AlSdkPlugin() {
  // sending a 'quit' message to custom socket server
  std::pair<int, std::vector<char>> msg(AlCallback::SdkMessageType::QUIT_SM,
                                        std::vector<char>());
  m_messageQueue.push(msg);

  std::cout << "AlSdkPlugin::~AlSdkPlugin 1" << std::endl;
  if (m_internalThread != NULL) {
    m_internalThread->interrupt();
    // TODO interrupt custom sockerserver
    // if (m_internalThread->joinable()) {
    // m_internalThread->join();
    // }
    delete m_internalThread;
    m_internalThread = NULL;
    // delete m_manager;
    // m_manager = NULL;
    std::cout << "AlSdkPlugin::~AlSdkPlugin 2" << std::endl;
  }
}

void AlSdkPlugin::init(AlSDKCb *alSdkCb) {
  m_sdkCb = alSdkCb;
  // // TODO debug, checking init thread
  m_internalThread = new boost::thread(&AlSdkPlugin::threadMain, this);
}

void AlSdkPlugin::init2() {
  alLogger() << "init!";
  // m_sdkCb = alSdkCb;
  // // // TODO debug, checking init thread
  // m_internalThread = new boost::thread(&AlSdkPlugin::threadMain, this);
  m_manager->init(this);
  m_manager->run();
}

void AlSdkPlugin::threadMain() {
  m_manager->init(this);
  m_manager->run();
}

void AlSdkPlugin::initializePeerConnection() {
  std::cout << "AlSdkPlugin::initializePeerConnection" << std::endl;
  boost::lock_guard<boost::mutex> guard(m_mtx);
  std::pair<int, std::vector<char>> msg(AlCallback::SdkMessageType::INIT_SM,
                                        std::vector<char>());
  m_messageQueue.push(msg);
}

void AlSdkPlugin::setDesiredDataSource(int dataSource) {}

void AlSdkPlugin::setRemoteSdp(const char *cSdp) {
  boost::lock_guard<boost::mutex> guard(m_mtx);
  std::vector<char> sdp = AlTextMessage::cStrToMsg(cSdp);
  std::pair<int, std::vector<char>> msg(AlCallback::SdkMessageType::SDP_SM,
                                        sdp);
  m_messageQueue.push(msg);
}

void AlSdkPlugin::setRemoteIceCandidate(std::vector<char> candidate) {
  boost::lock_guard<boost::mutex> guard(m_mtx);
  std::pair<int, std::vector<char>> msg(
      AlCallback::SdkMessageType::CANDIDATE_SM, candidate);
  m_messageQueue.push(msg);
}

void AlSdkPlugin::setImageData(std::vector<unsigned char> imageBytes, int width,
                               int height) {
  boost::lock_guard<boost::mutex> guard(m_mtx);
  m_imageBytes = imageBytes;
  std::pair<int, std::vector<char>> msg(
      AlCallback::SdkMessageType::NEW_FRAME_SM, std::vector<char>());
  m_messageQueue.push(msg);
}

void AlSdkPlugin::updateResolution(int width, int height) {
  boost::lock_guard<boost::mutex> guard(m_mtx);
  WIDTH = width;
  HEIGHT = height;
  std::pair<int, std::vector<char>> msg(
      AlCallback::SdkMessageType::UPDATE_RESOLUTION_SM, std::vector<char>());
  m_messageQueue.push(msg);
}

void AlSdkPlugin::setDesiredVideDeviceName(const char *deviceName) {
  std::cout << "AlSdkPlugin::setDesiredVideDeviceName" << std::endl;
  m_videoDeviceName = AlTextMessage::cStrToString(deviceName);
}

void AlSdkPlugin::updateFrameCb(const uint8_t *image, int width, int height) {
  m_sdkCb->updateFrameCb(image, width, height);
}

void AlSdkPlugin::updateLocalFrameCb(const uint8_t *image, int width,
                                     int height) {
  m_sdkCb->updateLocalFrameCb(image, width, height);
}
