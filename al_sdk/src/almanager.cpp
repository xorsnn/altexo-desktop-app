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
  this->m_thread->Run();
  this->m_thread->set_socketserver(NULL);
  rtc::CleanupSSL();
}

bool AlManager::hasConnections() {
  return m_conductor->hasConnections();
}

void AlManager::InitializePeerConnection() {
  // m_socketServer->
  if (m_conductor->InitializePeerConnection()) {
    m_conductor->m_peerConnection->CreateOffer(m_conductor, NULL);
  } else {
    // TODO: handle error
  }
}

void AlManager::DeletePeerConnection() { m_conductor->DeletePeerConnection(); }

void AlManager::OnMessageFromPeer(std::string peer_id,
                                  const std::string &message) {
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
