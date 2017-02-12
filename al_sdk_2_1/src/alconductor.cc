#include "alconductor.hpp"
#include "allogger.hpp"

// #include "webrtc/api/test/fakeconstraints.h"
// #include "webrtc/base/common.h"
// #include "webrtc/base/json.h"
// #include "webrtc/base/logging.h"
// #include "webrtc/examples/peerconnection/client/defaults.h"

#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/modules/video_capture/video_capture_factory.h"

// #include "defaults.h"
// #include "webrtc/api/test/fakeconstraints.h"

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";

#define DTLS_ON true
#define DTLS_OFF false

AlConductor::AlConductor() {}

AlConductor::~AlConductor() {}

bool AlConductor::InitializePeerConnection() {
  ASSERT(peer_connection_factory_.get() == NULL);
  ASSERT(peer_connection_.get() == NULL);

  alLogger() << "AlConductor::InitializePeerConnection";

  peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();

  if (!peer_connection_factory_.get()) {
    // main_wnd_->MessageBox("Error", "Failed to initialize
    // PeerConnectionFactory",
    //                       true);
    // DeletePeerConnection();
    alLogger() << "Failed to initialize PeerConnectionFactory";
    return false;
  }

  if (!CreatePeerConnection(DTLS_ON)) {
    // main_wnd_->MessageBox("Error", "CreatePeerConnection failed", true);
    alLogger() << "CreatePeerConnection failed";
    DeletePeerConnection();
  }
  // AddStreams();
  return peer_connection_.get() != NULL;
}

bool AlConductor::CreatePeerConnection(bool dtls) {
  ASSERT(peer_connection_factory_.get() != NULL);
  ASSERT(peer_connection_.get() == NULL);

  webrtc::PeerConnectionInterface::RTCConfiguration config;
  webrtc::PeerConnectionInterface::IceServer server;
  // server.uri = GetPeerConnectionString();
  // config.servers.push_back(server);

  // webrtc::FakeConstraints constraints;
  // if (dtls) {
  //   constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
  //                           "true");
  // } else {
  //   constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
  //                           "false");
  // }
  //
  // peer_connection_ = peer_connection_factory_->CreatePeerConnection(
  //     config, &constraints, NULL, NULL, this);
  // return peer_connection_.get() != NULL;
  return true;
}

void AlConductor::DeletePeerConnection() {}

//
// PeerConnectionObserver implementation.
//

void AlConductor::OnAddStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {}

void AlConductor::OnRemoveStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {}

void AlConductor::OnIceCandidate(
    const webrtc::IceCandidateInterface *candidate) {}

// CreateSessionDescriptionObserver implementation.
void AlConductor::OnSuccess(webrtc::SessionDescriptionInterface *desc) {}

void AlConductor::OnFailure(const std::string &error) {}
