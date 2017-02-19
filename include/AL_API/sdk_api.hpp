#ifndef ALSDKINTERFACE_H
#define ALSDKINTERFACE_H

#include "AL_CB/al_sdk_cb.hpp"
#include "altextmessage.hpp"
#include <string>

class AlCallback;
class AlManager;

class AlSdkAPI {
public:
  enum DesiredVideoSource {
    IMG_SNAPSHOTS = 1,
    CAMERA,
  };

  // AlSdkInterface();
  virtual ~AlSdkAPI() {}

  virtual void init(AlSDKCb *alSdkCb) = 0;
  virtual void init2() = 0;
  virtual void initializePeerConnection() = 0;
  virtual void setRemoteSdp(const char *sdp) = 0;
  virtual void setRemoteIceCandidate(std::vector<char> candidate) = 0;
  virtual void
  setDesiredDataSource(AlSdkAPI::DesiredVideoSource dataSource) = 0;
  virtual void setDesiredVideDeviceName(const char *deviceName) = 0;
  virtual void setImageData(std::vector<unsigned char> imageBytes, int width,
                            int height) = 0;
  virtual void updateResolution(int widht, int height) = 0;

  // virtual AlWsCb *getWsCb() = 0;
  // virtual void run() = 0;
  // virtual bool hasConnections() = 0;
  // virtual void InitializePeerConnection() = 0;
  // virtual void DeletePeerConnection() = 0;
  // // TODO: remove this
  // virtual void OnMessageFromPeer(std::string peerId,
  //                                const std::string &message) = 0;
  // this lib should not parce json
  // virtual void onMessageFromPeer(std::vector<char> peerId,
  //                                std::vector<char> message) = 0;
  // virtual bool isClientConnected() = 0;
  // virtual void clientConnect(const std::string &server, int port,
  //                            const std::string &clientName) = 0;
  // virtual void clientSignOut() = 0;
  // virtual void setImageData(uint8_t *pImageBytes, size_t len, int width,
  //                           int height) = 0;
};
//]

#endif // ALSDKINTERFACE_H
