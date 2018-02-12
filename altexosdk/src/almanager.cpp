#include "almanager.h"
#include "webrtc/rtc_base/ssladapter.h"

AlManager::AlManager()
    : m_socketServer(NULL), m_thread(NULL), m_conductor(NULL) {}

AlManager::~AlManager() {
  if (m_thread) {
    m_thread->Quit();
  }
}

void AlManager::run(AlCallback *alCallback) {
  CustomSocketServer socket_server;
  rtc::AutoSocketServerThread thread(&socket_server);
  rtc::InitializeSSL();
  // Must be constructed after we set the socketserver.
  // PeerConnectionClient client;
  rtc::scoped_refptr<Conductor> conductor(
      new rtc::RefCountedObject<Conductor>(alCallback));
  m_conductor = conductor;
  socket_server.set_conductor(conductor);
  socket_server.setAlCallback(alCallback);
  m_thread = &thread;
  thread.Run();
  m_thread = NULL;
  rtc::CleanupSSL();
  printf(">> run 2\n");
}

bool AlManager::hasConnections() { return false; }

void AlManager::InitializePeerConnection() {
  if (m_conductor->InitializePeerConnection()) {
    m_conductor->m_peerConnection->CreateOffer(m_conductor, NULL);
  } else {
  }
}

void AlManager::DeletePeerConnection() { m_conductor->DeletePeerConnection(); }

void AlManager::OnMessageFromPeer(std::string peer_id,
                                  const std::string &message) {
  std::vector<char> msgVec(message.begin(), message.end());
  m_conductor->OnMessageFromPeer(peer_id, msgVec);
}

void AlManager::setDesiredDataSource(AlSdkAPI::DesiredVideoSource dataSource) {
  // TODO: checking just for debug
  if (m_conductor) {
    m_conductor->m_dataManager->setDesiredDataSource(dataSource);
  }
}

void AlManager::setImageData(uint8_t *pImageBytes, size_t len, int width,
                             int height) {}
