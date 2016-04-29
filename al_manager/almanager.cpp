#include "almanager.h"
#include <iostream>
#include "webrtc/base/ssladapter.h"

using namespace std;

extern "C" AlManager* create_object()
{
    return new AlManager;
}

extern "C" void destroy_object( AlManager* object )
{
    delete object;
}

AlManager::AlManager():
    m_conductor(NULL)
{
}

void AlManager::init(AlCallback* alCallback)
{
    this->thread = rtc::Thread::Current();
    m_socketServer = new CustomSocketServer(thread);
    thread->set_socketserver(m_socketServer);
    rtc::InitializeSSL();
    // Must be constructed after we set the socketserver.
    rtc::scoped_refptr<Conductor> conductor(
                new rtc::RefCountedObject<Conductor>(&m_client, alCallback, &m_dataManager));
    m_socketServer->set_conductor(conductor);
    m_socketServer->setAlCallback(alCallback);
    m_conductor = conductor;
}

void AlManager::run()
{
    this->thread->Run();
    this->thread->set_socketserver(NULL);
    rtc::CleanupSSL();
}

bool AlManager::hasConnections() {
    std::cout << ">>1" << std::endl;
     return m_conductor->hasConnections();
}

void AlManager::InitializePeerConnection() {
    if (m_conductor->InitializePeerConnection()) {
        std::cout << ">> 11" << std::endl;
//        qDebug() << "> 1";
//        qDebug() << "> 3";
//        m_peerId = peer_id.toStdString();
        m_conductor->m_peerConnection->CreateOffer(m_conductor, NULL);
    } else {
        std::cout << ">> 12" << std::endl;
//        qDebug() << "> 4";
        std::cout << "Error. Failed to initialize PeerConnection" << std::endl;
    }
}

void AlManager::DeletePeerConnection() {
    m_conductor->DeletePeerConnection();
}

void AlManager::OnMessageFromPeer(std::string peer_id, const std::string& message) {
    std::cout << "AlManager::OnMessageFromPeer" << std::endl;
    m_conductor->OnMessageFromPeer(peer_id, message);
}
