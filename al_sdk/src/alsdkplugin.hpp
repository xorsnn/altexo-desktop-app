#ifndef ALSDKPLUGIN_H
#define ALSDKPLUGIN_H

#include "AL_API/sdk_api.hpp"
#include "AL_CB/al_sdk_cb.hpp"
#include "AL_CB/al_ws_cb.hpp"
#include <boost/config.hpp>
#include <string>

class AlManager;

class AlSdkPlugin : public AlSdkAPI, public AlWsCb {
public:
  AlSdkPlugin();
  ~AlSdkPlugin();

  void init(AlSDKCb *alSdkCb);
  AlWsCb *getWsCb();
  void run();
  bool hasConnections();
  void InitializePeerConnection();
  void DeletePeerConnection();

  // TODO: union the two methods
  void OnMessageFromPeer(std::string peer_id, const std::string &message);
  void onMessageCb(std::string peer_id, const std::string &message);

  bool isClientConnected();
  void clientConnect(const std::string &server, int port,
                     const std::string &client_name);
  void clientSignOut();
  void setDesiredDataSource(int dataSource);
  void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);

private:
  AlManager *m_manager;
};

extern "C" BOOST_SYMBOL_EXPORT AlSdkPlugin plugin;
AlSdkPlugin plugin;

#endif // ALSDKPLUGIN_H
