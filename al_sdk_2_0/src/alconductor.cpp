#include "alconductor.hpp"

#include <memory>
#include <utility>
#include <vector>

#include <iostream>

#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";

#define DTLS_ON true
#define DTLS_OFF false

AlConductor::AlConductor() {
  // LOG(INFO) << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  // std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa" << std::endl;
}

AlConductor::~AlConductor() {}

// General interface
bool AlConductor::initializePeerConnection() {

  if (true) {
    std::cout << "Conductor::InitializePeerConnection" << std::endl;
  }
  //
  // ASSERT(m_peer_connection_factory_.get() == NULL);
  // ASSERT(m_peerConnection.get() == NULL);
  //
  // m_peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();

  // if (!peer_connection_factory_.get()) {
  //   std::cout << "Error, Failed to initialize PeerConnectionFactory"
  //             << std::endl;
  //   DeletePeerConnection();
  //   return false;
  // }
  //
  // if (!CreatePeerConnection(DTLS_ON)) {
  //   std::cout << "Error, CreatePeerConnection failed" << std::endl;
  //   DeletePeerConnection();
  // }
  // AddStreams();
  // if (m_debug) {
  //   std::cout << "Conductor m_peerConnection:" << std::endl;
  //   std::cout << m_peerConnection.get() << std::endl;
  //   // std::cout << (m_peerConnection.get() != NULL) << std::endl;
  // }
  // return m_peerConnection.get() != NULL;
}

//
// PeerConnectionObserver implementation.
//

// Called when a remote stream is added
void AlConductor::OnAddStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {
  LOG(INFO) << __FUNCTION__ << " " << stream->label();
  // TODO: restore
  // main_wnd_->QueueUIThreadCallback(NEW_STREAM_ADDED, stream.release());
}

void AlConductor::OnRemoveStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {
  LOG(INFO) << __FUNCTION__ << " " << stream->label();
  // TODO: restore
  // main_wnd_->QueueUIThreadCallback(STREAM_REMOVED, stream.release());
}

void AlConductor::OnIceCandidate(
    const webrtc::IceCandidateInterface *candidate) {
  LOG(INFO) << __FUNCTION__ << " " << candidate->sdp_mline_index();

  Json::StyledWriter writer;
  Json::Value jmessage;

  jmessage[kCandidateSdpMidName] = candidate->sdp_mid();
  jmessage[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
  std::string sdp;
  if (!candidate->ToString(&sdp)) {
    LOG(LS_ERROR) << "Failed to serialize candidate";
    return;
  }
  jmessage[kCandidateSdpName] = sdp;
  // TODO: restore
  // SendMessage(writer.write(jmessage));
}

// CreateSessionDescriptionObserver implementation.
void AlConductor::OnSuccess(webrtc::SessionDescriptionInterface *desc) {
  std::cout << "AlConductor::OnSuccess" << std::endl;
  // TODO: restore
  // peer_connection_->SetLocalDescription(
  //     DummySetSessionDescriptionObserver::Create(), desc);
  //
  // std::string sdp;
  // desc->ToString(&sdp);
  //
  // Json::StyledWriter writer;
  // Json::Value jmessage;
  // jmessage[kSessionDescriptionTypeName] = desc->type();
  // jmessage[kSessionDescriptionSdpName] = sdp;
  // SendMessage(writer.write(jmessage));
}

void AlConductor::OnFailure(const std::string &error) { LOG(LERROR) << error; }
