#include "alsdkplugin.h"
// #include "almanager.h"
#include <iostream>
using namespace std;

AlSdkPlugin::AlSdkPlugin() {
  std::cout << "AlSdkPlugin constructor" << std::endl;
  // m_manager = new AlManager();
}

AlSdkPlugin::~AlSdkPlugin() {
    // delete m_manager;
    // m_manager = NULL;
}

void AlSdkPlugin::init(AlCallback* alCallback)
{
    // m_manager->init(alCallback);
}

void AlSdkPlugin::run()
{
    // m_manager->run();
}

bool AlSdkPlugin::hasConnections() {
    // return m_manager->hasConnections();
}

void AlSdkPlugin::InitializePeerConnection() {
    // m_manager->InitializePeerConnection();
}

void AlSdkPlugin::DeletePeerConnection() {
    // m_manager->DeletePeerConnection();
}

void AlSdkPlugin::OnMessageFromPeer(std::string peerId, const std::string& message) {
    // m_manager->OnMessageFromPeer(peerId, message);
}

bool AlSdkPlugin::isClientConnected() {
    // return m_manager->m_client.is_connected();
}

void AlSdkPlugin::clientConnect(const std::string& server, int port,
                    const std::string& client_name) {
    // m_manager->m_client.Connect(server, port, client_name);
}

void AlSdkPlugin::clientSignOut() {
    // m_manager->m_client.SignOut();
}

void AlSdkPlugin::setDesiredDataSource(int dataSource) {
    // m_manager->setDesiredDataSource(dataSource);
}

void AlSdkPlugin::setImageData(uint8_t *pImageBytes, size_t len, int width, int height) {
    // m_manager->setImageData(pImageBytes, len, width, height);
}
