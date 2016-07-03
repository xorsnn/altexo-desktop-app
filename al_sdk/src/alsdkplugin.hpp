#ifndef ALSDKPLUGIN_H
#define ALSDKPLUGIN_H

#include "sdk_api.hpp"
#include <boost/config.hpp>
#include <string>

class AlCallback;
class AlManager;

class AlSdkPlugin : public AlSdkInterface {
public:
  AlSdkPlugin();
  ~AlSdkPlugin();

  void init(AlCallback *alCallback);
  void run();
  bool hasConnections();
  void InitializePeerConnection();
  void DeletePeerConnection();
  void OnMessageFromPeer(std::string peer_id, const std::string &message);
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
