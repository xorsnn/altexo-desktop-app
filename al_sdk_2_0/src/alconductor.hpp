
#ifndef AL_CONDUCTOR_H
#define AL_CONDUCTOR_H
#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>

#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"

namespace webrtc {
class VideoCaptureModule;
} // namespace webrtc

namespace cricket {
class VideoRenderer;
} // namespace cricket

class AlConductor : public webrtc::PeerConnectionObserver,
                    public webrtc::CreateSessionDescriptionObserver {
public:
  AlConductor();
  ~AlConductor();

  // General interface
  bool initializePeerConnection();

  rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peerConnection;

protected:
  // PeerConnectionObserver implementation.
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

  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
      m_peer_connection_factory_;
};

#endif //  AL_CONDUCTOR_H
