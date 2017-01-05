

#ifndef ALCONDUCTOR_H
#define ALCONDUCTOR_H
#pragma once

// #include <deque>
// #include <map>
// #include <set>
// #include <string>
// =============================
// #include "webrtc/base/nethelpers.h"
// #include "webrtc/base/physicalsocketserver.h"
// #include "webrtc/base/signalthread.h"
// #include "webrtc/base/sigslot.h"
//
// #include "webrtc/api/mediastreaminterface.h"
// #include "webrtc/base/win32.h"
// // #include "webrtc/examples/peerconnection/client/peer_connection_client.h"
// #include "webrtc/media/base/mediachannel.h"
// #include "webrtc/media/base/videocommon.h"
// #include "webrtc/video_frame.h"
//
// #include "libyuv/convert_argb.h"
// #include "webrtc/base/arraysize.h"
// #include "webrtc/base/common.h"
// #include "webrtc/base/logging.h"
// =============================
//
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"
//
// #include "chromium/src/breakpad/src/common/scoped_ptr.h"
// #include "peer_connection_client.h"
//
// #include "defaults.h"
//
// #include "webrtc/base/common.h"
// #include "webrtc/base/json.h"
// #include "webrtc/base/logging.h"
//
// #include "alcallback.h"
// #include "altextmessage.hpp"
// #include "alvideorenderer.h"
// #include "conductor/aldatamanager.h"

namespace webrtc {
class VideoCaptureModule;
} // namespace webrtc

namespace cricket {
class VideoRenderer;
} // namespace cricket

class AlConductor : public webrtc::PeerConnectionObserver,
                    public webrtc::CreateSessionDescriptionObserver {

public:
  enum CallbackID {
    MEDIA_CHANNELS_INITIALIZED = 1,
    PEER_CONNECTION_CLOSED,
    SEND_MESSAGE_TO_PEER,
    NEW_STREAM_ADDED,
    STREAM_REMOVED,
    SEND_LOCAL_CANDIDATE,
    SEND_LOCAL_SDP,
  };

  AlConductor();

  bool InitializePeerConnection();
  bool CreatePeerConnection(bool dtls);
  void DeletePeerConnection();

protected:
  ~AlConductor();

  //
  // PeerConnectionObserver implementation.
  //
  void OnSignalingChange(
      webrtc::PeerConnectionInterface::SignalingState new_state) override{};
  void
  OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  void OnRemoveStream(
      rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override {}
  void OnRenegotiationNeeded() override {}
  void OnIceConnectionChange(
      webrtc::PeerConnectionInterface::IceConnectionState new_state) override{};
  void OnIceGatheringChange(
      webrtc::PeerConnectionInterface::IceGatheringState new_state) override{};
  void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override;
  void OnIceConnectionReceivingChange(bool receiving) override {}

  // CreateSessionDescriptionObserver implementation.
  virtual void OnSuccess(webrtc::SessionDescriptionInterface *desc);
  virtual void OnFailure(const std::string &error);

  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
      peer_connection_factory_;
};

#endif // ALCONDUCTOR_H
