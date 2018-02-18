
#ifndef WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
#define WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>

#include "api/mediastreaminterface.h"
#include "api/peerconnectioninterface.h"

#include "defaults.h"

#include "altexosdk/src/alcallback.h"
#include "altextmessage.hpp"

#include "altexosdk/src/alvideorenderer.h"

#include "aldatamanager.h"

namespace webrtc {
class VideoCaptureModule;
} // namespace webrtc

namespace cricket {
class VideoRenderer;
} // namespace cricket

class Conductor : public webrtc::PeerConnectionObserver,
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

  Conductor(
      // PeerConnectionClient *client,
      AlCallback *alCallback);

  bool connection_active() const;

  virtual void Close();

  // TODO may be make protected or private
  bool InitializePeerConnection();
  bool ReinitializePeerConnectionForLoopback();
  bool CreatePeerConnection(bool dtls);
  void DeletePeerConnection();

  bool hasConnections() { return (m_peerConnection.get()); }

  void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);
  void setImageData(std::vector<unsigned char> imageBytes, int width,
                    int height);

  // TODO: maybe eliminate
  AlDataManager *m_dataManager;

protected:
  ~Conductor();
  void EnsureStreamingUI();
  void AddStreams();
  std::unique_ptr<cricket::VideoCapturer> OpenVideoCaptureDevice();

  //
  // PeerConnectionObserver implementation.
  //
  void OnSignalingChange(
      webrtc::PeerConnectionInterface::SignalingState new_state) override {}
  void
  OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  void OnRemoveStream(
      rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override {}
  void OnRenegotiationNeeded() override {}
  void OnIceConnectionChange(
      webrtc::PeerConnectionInterface::IceConnectionState new_state) override {}
  void OnIceGatheringChange(
      webrtc::PeerConnectionInterface::IceGatheringState new_state) override {}
  void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override;
  void OnIceConnectionReceivingChange(bool receiving) override {}

  // TODO: remove
  virtual void queueUIThreadCallback(int msg_id, void *data);
  virtual void UIThreadCallback(int msg_id, void *data);

  // CreateSessionDescriptionObserver implementation.
  virtual void OnSuccess(webrtc::SessionDescriptionInterface *desc);
  virtual void OnFailure(const std::string &error);

public:
  void OnMessageFromPeer(std::string peer_id, std::vector<char> msgVec);

protected:
  // Send a message to the remote peer.
  void SendMessage(const std::string &json_object);

  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
      peer_connection_factory_;
  // PeerConnectionClient *m_client;
  AlCallback *m_alCallback;
  std::deque<std::string *> m_pendingMessages;

  std::string server_;

  bool m_debug;
  std::queue<std::map<std::string, std::string>> m_messageQueue;
  bool m_processingMsg;
  bool m_isAcceptingConnection;

  // local renderer
  std::unique_ptr<AlVideoRenderer> m_localRenderer;
  // remote renderer
  std::unique_ptr<AlVideoRenderer> m_remoteRenderer;

public:
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peerConnection;
  std::map<std::string, rtc::scoped_refptr<webrtc::MediaStreamInterface>>
      m_activeStreams;
};

#endif // WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
