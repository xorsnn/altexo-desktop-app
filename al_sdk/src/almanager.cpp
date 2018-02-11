#include "almanager.h"
#include "webrtc/base/ssladapter.h"
#include <iostream>

using namespace std;

AlManager::AlManager()
    : m_conductor(NULL), m_socketServer(NULL), m_thread(NULL), m_debug(true) {}

AlManager::~AlManager() {
  // CustomSocketServer *m_socketServer;
  // rtc::Thread *m_thread;
  // rtc::scoped_refptr<Conductor> m_conductor;
}

void AlManager::init(AlCallback *alCallback) {
  // auto_thread = new rtc::AutoThread();
  rtc::AutoThread auto_thread;
  this->m_thread = rtc::Thread::Current();
  m_socketServer = new CustomSocketServer(m_thread);
  m_thread->set_socketserver(m_socketServer);

  rtc::InitializeSSL();
  // Must be constructed after we set the socketserver.
  rtc::scoped_refptr<Conductor> conductor(new rtc::RefCountedObject<Conductor>(
      &m_client, alCallback, &m_dataManager));
  m_socketServer->set_conductor(conductor);
  m_socketServer->setAlCallback(alCallback);
  m_conductor = conductor;
}

void AlManager::run() {
  if (m_debug) {
    std::cout << "AlManager::run" << std::endl;
  }

  std::cout << "AlManager::run1" << std::endl;
  this->m_thread->Run();
  std::cout << "AlManager::run2" << std::endl;
  this->m_thread->set_socketserver(NULL);
  std::cout << "AlManager::run3" << std::endl;
  rtc::CleanupSSL();
  std::cout << "AlManager::run4" << std::endl;
}

bool AlManager::hasConnections() {
  if (m_debug) {
    std::cout << "AlManager::hasConnections" << std::endl;
  }
  return m_conductor->hasConnections();
}

void AlManager::InitializePeerConnection() {
  // m_socketServer->
  if (m_debug) {
    std::cout << "AlManager::InitializePeerConnection" << std::endl;
  }
  if (m_conductor->InitializePeerConnection()) {
    if (m_debug) {
      std::cout << "AlManager AlManager CreateOffer" << std::endl;
    }
    m_conductor->m_peerConnection->CreateOffer(m_conductor, NULL);
  } else {
    std::cout << "Error. Failed to initialize PeerConnection" << std::endl;
  }
}

void AlManager::DeletePeerConnection() { m_conductor->DeletePeerConnection(); }

void AlManager::OnMessageFromPeer(std::string peer_id,
                                  const std::string &message) {
  std::cout << "AlManager::OnMessageFromPeer" << std::endl;
  std::vector<char> msgVec(message.begin(), message.end());
  m_conductor->OnMessageFromPeer(peer_id, msgVec);
}

void AlManager::setDesiredDataSource(int dataSource) {
  m_dataManager.setDesiredDataSource(dataSource);
}

void AlManager::setImageData(uint8_t *pImageBytes, size_t len, int width,
                             int height) {
  m_dataManager.setImageData(pImageBytes, len, width, height);
}
