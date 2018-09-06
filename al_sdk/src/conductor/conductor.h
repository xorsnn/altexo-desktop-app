/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
#define WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>

#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"

#include "chromium/src/breakpad/src/common/scoped_ptr.h"
#include "peer_connection_client.h"

#include "defaults.h"

#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"

#include "alcallback.h"
#include "alvideorenderer.h"
#include "conductor/aldatamanager.h"

namespace webrtc {
class VideoCaptureModule;
} // namespace webrtc

namespace cricket {
class VideoRenderer;
} // namespace cricket


class Conductor : public webrtc::PeerConnectionObserver,
                  public webrtc::CreateSessionDescriptionObserver
//        public PeerConnectionClientObserver,
//        public MainWndCallback
{

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

  Conductor(PeerConnectionClient *client, AlCallback *alCallback,
            AlDataManager *alDataManager);

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

protected:
  ~Conductor();
  void EnsureStreamingUI();
  void AddStreams();
  cricket::VideoCapturer *OpenVideoCaptureDevice();

  //
  // PeerConnectionObserver implementation.
  //
  void OnSignalingChange(
      webrtc::PeerConnectionInterface::SignalingState new_state) override {}
  void OnAddStream(webrtc::MediaStreamInterface *stream) override;
  void OnRemoveStream(webrtc::MediaStreamInterface *stream) override;
  void OnDataChannel(webrtc::DataChannelInterface *channel) override {}
  void OnRenegotiationNeeded() override {}
  void OnIceConnectionChange(
      webrtc::PeerConnectionInterface::IceConnectionState new_state) override {}
  void OnIceGatheringChange(
      webrtc::PeerConnectionInterface::IceGatheringState new_state) override {}
  void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override;
  void OnIceConnectionReceivingChange(bool receiving) override {}
  virtual void queueUIThreadCallback(int msg_id, void *data);
  virtual void UIThreadCallback(int msg_id, void *data);

  // CreateSessionDescriptionObserver implementation.
  virtual void OnSuccess(webrtc::SessionDescriptionInterface *desc);
  virtual void OnFailure(const std::string &error);

public:
  void OnMessageFromPeer(std::string peer_id, std::vector<char> msgVec);

  AlDataManager *m_dataManager;

protected:
  // Send a message to the remote peer.
  void SendMessage(const std::string &json_object);

  //    std::string peer_id_;
  bool loopback_;

  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
      peer_connection_factory_;
  PeerConnectionClient *m_client;
  AlCallback *m_alCallback;
  std::deque<std::string *> m_pendingMessages;

  std::string server_;

  bool m_debug;
  std::queue<std::map<std::string, std::string>> m_messageQueue;
  bool m_processingMsg;
  bool m_isAcceptingConnection;

  // local renderer
  google_breakpad::scoped_ptr<AlVideoRenderer> m_localRenderer;
  // remote renderer
  google_breakpad::scoped_ptr<AlVideoRenderer> m_remoteRenderer;

public:
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peerConnection;
  std::map<std::string, rtc::scoped_refptr<webrtc::MediaStreamInterface>>
      m_activeStreams;
};

#endif // WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
