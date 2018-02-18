#include "altexosdk/src/alsdkplugin.hpp"
#include "alcallback.h"
#include "almanager.h"

using namespace std;

AlSdkPlugin::AlSdkPlugin()
    : m_manager(NULL), m_videoDeviceName(""), WIDTH(0), HEIGHT(0) {

  printf("AlSdkPlugin::AlSdkPlugin()\n");
}

AlSdkPlugin::~AlSdkPlugin() {
  // sending a 'quit' message to custom socket server
  std::pair<int, std::vector<char>> msg(AlCallback::SdkMessageType::QUIT_SM,
                                        std::vector<char>());
  m_messageQueue.push(msg);

  if (m_manager) {
    delete m_manager;
    m_manager = NULL;
  }
}
void AlSdkPlugin::init(AlSDKCb *alSdkCb) {
  m_sdkCb = alSdkCb;
  m_manager = new AlManager();
  m_manager->run(this);
}

void AlSdkPlugin::initializePeerConnection() {
  std::pair<int, std::vector<char>> msg(AlCallback::SdkMessageType::INIT_SM,
                                        std::vector<char>());
  m_messageQueue.push(msg);
}

void AlSdkPlugin::setDesiredDataSource(
    AlSdkAPI::DesiredVideoSource dataSource) {
  m_manager->setDesiredDataSource(dataSource);
}

void AlSdkPlugin::setRemoteSdp(const char *cSdp) {
  std::vector<char> sdp = AlTextMessage::cStrToMsg(cSdp);
  std::pair<int, std::vector<char>> msg(AlCallback::SdkMessageType::SDP_SM,
                                        sdp);
  m_messageQueue.push(msg);
}

void AlSdkPlugin::setRemoteIceCandidate2(const char *candidate) {
  std::pair<int, std::vector<char>> msg(
      AlCallback::SdkMessageType::CANDIDATE_SM,
      AlTextMessage::cStrToMsg(candidate));
  m_messageQueue.push(msg);
}

void AlSdkPlugin::setImageData(std::vector<unsigned char> imageBytes, int width,
                               int height) {
  m_imageBytes = imageBytes;
  std::pair<int, std::vector<char>> msg(
      AlCallback::SdkMessageType::NEW_FRAME_SM, std::vector<char>());
  m_messageQueue.push(msg);
}

void AlSdkPlugin::updateResolution(int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  std::pair<int, std::vector<char>> msg(
      AlCallback::SdkMessageType::UPDATE_RESOLUTION_SM, std::vector<char>());
  m_messageQueue.push(msg);
}

void AlSdkPlugin::setDesiredVideDeviceName(const char *deviceName) {
  m_videoDeviceName = AlTextMessage::cStrToString(deviceName);
}

void AlSdkPlugin::updateFrameCb(const uint8_t *image, int width, int height) {
  m_sdkCb->updateFrameCb(image, width, height);
}

void AlSdkPlugin::updateLocalFrameCb(const uint8_t *image, int width,
                                     int height) {
  m_sdkCb->updateLocalFrameCb(image, width, height);
}
