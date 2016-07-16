#ifndef ALSDKINTERFACE_H
#define ALSDKINTERFACE_H

#include "AL_CB/al_sdk_cb.hpp"
// #include "AL_CB/al_ws_cb.hpp"
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
  // virtual AlWsCb *getWsCb() = 0;
  // virtual void run() = 0;
  // virtual bool hasConnections() = 0;
  // virtual void InitializePeerConnection() = 0;
  // virtual void DeletePeerConnection() = 0;
  // // TODO: remove this
  // virtual void OnMessageFromPeer(std::string peerId,
  //                                const std::string &message) = 0;
  virtual void onMessageFromPeer(std::vector<char> peerId,
                                 std::vector<char> message) = 0;
  // virtual bool isClientConnected() = 0;
  // virtual void clientConnect(const std::string &server, int port,
  //                            const std::string &clientName) = 0;
  // virtual void clientSignOut() = 0;
  // virtual void setDesiredDataSource(int dataSource) = 0;
  // virtual void setImageData(uint8_t *pImageBytes, size_t len, int width,
  //                           int height) = 0;
};
//]

#endif // ALSDKINTERFACE_H
