#include "almanagerinterface.h"
#include "almanager.h"

using namespace std;

extern "C" AlManagerInterface* create_object()
{
    return new AlManagerInterface;
}

extern "C" void destroy_object( AlManagerInterface* object )
{
    delete object;
}

AlManagerInterface::AlManagerInterface() {
    m_manager = new AlManager();
}

AlManagerInterface::~AlManagerInterface() {
    delete m_manager;
    m_manager = NULL;
}

void AlManagerInterface::init(AlCallback* alCallback)
{
    m_manager->init(alCallback);
}

void AlManagerInterface::run()
{
    m_manager->run();
}

bool AlManagerInterface::hasConnections() {
    return m_manager->hasConnections();
}

void AlManagerInterface::InitializePeerConnection() {
    m_manager->InitializePeerConnection();
}

void AlManagerInterface::DeletePeerConnection() {
    m_manager->DeletePeerConnection();
}

void AlManagerInterface::OnMessageFromPeer(std::string peerId, const std::string& message) {
    m_manager->OnMessageFromPeer(peerId, message);
}

bool AlManagerInterface::isClientConnected() {
    return m_manager->m_client.is_connected();
}

void AlManagerInterface::clientConnect(const std::string& server, int port,
                    const std::string& client_name) {
    m_manager->m_client.Connect(server, port, client_name);
}

void AlManagerInterface::clientSignOut() {
    m_manager->m_client.SignOut();
}

void AlManagerInterface::setDesiredDataSource(int dataSource) {
    m_manager->setDesiredDataSource(dataSource);
}

void AlManagerInterface::setImageData(uint8_t *pImageBytes, size_t len, int width, int height) {
    m_manager->setImageData(pImageBytes, len, width, height);
}
