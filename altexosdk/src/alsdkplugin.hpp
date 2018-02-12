#ifndef ALSDKPLUGIN_H
#define ALSDKPLUGIN_H

#include "AL_API/sdk_api.hpp"
#include "AL_CB/al_sdk_cb.hpp"
#include "alcallback.h"
#include "altextmessage.hpp"

#ifdef _WIN32
#include "boost/dll/alias.hpp"
#endif

#include <cstring>
#include <iostream>
#include <queue>
#include <string>

class AlManager;

class AlSdkPlugin : public AlSdkAPI, public AlCallback {
public:
  AlSdkPlugin();
  ~AlSdkPlugin();

  void init(AlSDKCb *alSdkCb);
  void initializePeerConnection();
  void setRemoteSdp(const char *cSdp);

  void setRemoteIceCandidate2(const char *candidate);

  // TODO: deprecated
  void setImageData(std::vector<unsigned char> imageBytes, int width,
                    int height);

  void setImageData2(const unsigned char *imageBytes, int width, int height){};

  void updateResolution(int widht, int height);

  void setDesiredDataSource(AlSdkAPI::DesiredVideoSource dataSource);

  void onSdpCb(const std::string &message) {
    AlTextMessage msg(message);
    m_sdkCb->onLocalSdpCb(msg);
  }

  void onCandidateCb(const std::string &message) {
    AlTextMessage msg(message);
    m_sdkCb->onLocalIceCandidateCb(msg);
  }

  void stopLocalRendererCb() {}

  void stopRemoteRendererCb() {}

  void onDevicesListChangedCb(std::vector<alMsg> deviceNames) {
    for (unsigned long i = 0; i < deviceNames.size(); i++) {
      AlTextMessage msg(deviceNames[i]);
      std::string msgStr = msg.toString();
      char *cstr = new char[msgStr.length() + 1];
      cstr[msgStr.length()] = '\n';
      std::strcpy(cstr, msgStr.c_str());
      m_sdkCb->onNewCaptureDeciceCb(cstr);
      delete[] cstr;
    }
  }

  void updateFrameCb(const uint8_t *image, int width, int height);
  void updateLocalFrameCb(const uint8_t *image, int width, int height);

  void setDesiredVideDeviceName(const char *deviceName);

  //  TODO move to AlManager
  std::string getVideoDeviceName() { return m_videoDeviceName; }

  // getters
  bool ifNewMessage() { return !m_messageQueue.empty(); }

  std::pair<int, std::vector<char>> degueueMessage() {
    std::pair<int, std::vector<char>> result = m_messageQueue.front();
    m_messageQueue.pop();
    return result;
  }

  std::vector<unsigned char> *getFrameRef() { return &m_imageBytes; }

  int getWidth() { return WIDTH; }
  int getHeight() { return HEIGHT; }

private:
  AlManager *m_manager;
  AlSDKCb *m_sdkCb;

  // signals

  // bool m_debug;

  std::vector<unsigned char> m_imageBytes;

  std::queue<std::pair<int, std::vector<char>>> m_messageQueue;

  std::string m_videoDeviceName;

  int WIDTH;
  int HEIGHT;
};

#endif // ALSDKPLUGIN_H
