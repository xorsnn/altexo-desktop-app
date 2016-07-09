#include "alsdkplugin.hpp"
#include "almanager.h"
#include <iostream>
using namespace std;

AlSdkPlugin::AlSdkPlugin() {
  std::cout << "AlSdkPlugin constructor" << std::endl;
  m_manager = new AlManager();
}

AlSdkPlugin::~AlSdkPlugin() {
  delete m_manager;
  m_manager = NULL;
}

void AlSdkPlugin::init(AlSDKCb *AlSdkCb) {
  // m_manager->init(alCallback);
}

AlWsCb *AlSdkPlugin::getWsCb() { return this; }

void AlSdkPlugin::run() {
  // m_manager->run();
}

bool AlSdkPlugin::hasConnections() { return m_manager->hasConnections(); }

void AlSdkPlugin::InitializePeerConnection() {
  m_manager->InitializePeerConnection();
}

void AlSdkPlugin::DeletePeerConnection() { m_manager->DeletePeerConnection(); }

void AlSdkPlugin::OnMessageFromPeer(std::string peerId,
                                    const std::string &message) {
  m_manager->OnMessageFromPeer(peerId, message);
}
void AlSdkPlugin::onMessageCb(std::vector<char> peerId,
                              std::vector<char> message) {
  std::string strPeerId(peerId.begin(), peerId.end());
  std::string strMsg(message.begin(), message.end());
  std::cout << "===========================" << std::endl;
  std::cout << strPeerId << std::endl;
  std::cout << "===========================" << std::endl;
}

bool AlSdkPlugin::isClientConnected() {
  return m_manager->m_client.is_connected();
}

void AlSdkPlugin::clientConnect(const std::string &server, int port,
                                const std::string &client_name) {
  m_manager->m_client.Connect(server, port, client_name);
}

void AlSdkPlugin::clientSignOut() { m_manager->m_client.SignOut(); }

void AlSdkPlugin::setDesiredDataSource(int dataSource) {
  m_manager->setDesiredDataSource(dataSource);
}

// TODO: reconfigure this
void AlSdkPlugin::setImageData(uint8_t *pImageBytes, size_t len, int width,
                               int height) {
  m_manager->setImageData(pImageBytes, len, width, height);
}
