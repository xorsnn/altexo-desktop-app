#include "alsdkplugin.hpp"
#include "alcallback.h"
#include "almanager.h"
#include <iostream>

using namespace std;

AlSdkPlugin::AlSdkPlugin()
    : m_debug(true), WIDTH(0), HEIGHT(0), m_videoDeviceName("") {
  m_manager = new AlManager();
}

AlSdkPlugin::~AlSdkPlugin() {
  if (m_internalThread->joinable()) {
    m_internalThread->join();
  }
  delete m_internalThread;
  m_internalThread = NULL;
  delete m_manager;
  m_manager = NULL;
}

void AlSdkPlugin::init(AlSDKCb *alSdkCb) {
  m_sdkCb = alSdkCb;
  // TODO debug, checking init thread
  m_internalThread = new boost::thread(&AlSdkPlugin::threadMain, this);
}

void AlSdkPlugin::threadMain() {
  m_manager->init(this);
  m_manager->run();
}

void AlSdkPlugin::initializePeerConnection() {
  boost::lock_guard<boost::mutex> guard(m_mtx);
  std::pair<int, std::vector<char>> msg(AlCallback::SdkMessageType::INIT_SM,
                                        std::vector<char>());
  m_messageQueue.push(msg);
}

// bool AlSdkPlugin::hasConnections() { return m_manager->hasConnections(); }
// void AlSdkPlugin::DeletePeerConnection() { m_manager->DeletePeerConnection();
// }
// bool AlSdkPlugin::isClientConnected() {
//   return m_manager->m_client.is_connected();
// }
// void AlSdkPlugin::clientConnect(const std::string &server, int port,
//                                 const std::string &client_name) {
//   m_manager->m_client.Connect(server, port, client_name);
// }
// void AlSdkPlugin::clientSignOut() { m_manager->m_client.SignOut(); }

void AlSdkPlugin::setDesiredDataSource(int dataSource) {
  m_manager->setDesiredDataSource(dataSource);
}

void AlSdkPlugin::setRemoteSdp(std::vector<char> sdp) {
  boost::lock_guard<boost::mutex> guard(m_mtx);
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

void AlSdkPlugin::setDesiredVideDeviceName(AlTextMessage deviceName) {
  m_videoDeviceName = deviceName.getText();
}

void AlSdkPlugin::updateFrameCb(const uint8_t *image, int width, int height) {
  m_sdkCb->updateFrameCb(image, width, height);
}
