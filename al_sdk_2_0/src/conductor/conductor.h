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
#include "altextmessage.hpp"
#include "alvideorenderer.h"
#include "conductor/aldatamanager.h"

namespace webrtc {
class VideoCaptureModule;
} // namespace webrtc

namespace cricket {
class VideoRenderer;
} // namespace cricket

// class MainWndCallback {
// public:
//    virtual void StartLogin(const std::string& server, int port) = 0;
//    virtual void DisconnectFromServer() = 0;
//    virtual void ConnectToPeer(QString peer_id) = 0;
//    virtual void DisconnectFromCurrentPeer() = 0;
//    virtual void UIThreadCallback(int msg_id, void* data) = 0;
//    virtual void Close() = 0;
// protected:
//    virtual ~MainWndCallback() {}
//};

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
  //    bool InitializePeerConnection();
  //    bool ReinitializePeerConnectionForLoopback();
  //    bool CreatePeerConnection(bool dtls);
  //    void DeletePeerConnection();
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
  //    virtual void OnStateChange(
  //            webrtc::PeerConnectionObserver::StateType state_changed) {}
  //    virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
  //    virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
  //    virtual void OnDataChannel(webrtc::DataChannelInterface* channel) {}
  //    virtual void OnRenegotiationNeeded() {}
  //    virtual void OnIceChange() {}
  //    virtual void OnIceCandidate(const webrtc::IceCandidateInterface*
  //    candidate);

  //    //
  //    // MainWndCallback implementation.
  //    //

  //    virtual void StartLogin(const std::string& server, int port);
  //    virtual void DisconnectFromServer();
  //    virtual void ConnectToPeer(QString peer_id);
  //    virtual void DisconnectFromCurrentPeer();
  virtual void queueUIThreadCallback(int msg_id, void *data);
  virtual void UIThreadCallback(int msg_id, void *data);

  // CreateSessionDescriptionObserver implementation.
  virtual void OnSuccess(webrtc::SessionDescriptionInterface *desc);
  virtual void OnFailure(const std::string &error);

  //    //
  //    // PeerConnectionClientObserver implementation.
  //    //
public:
  //    virtual void OnSignedIn();
  //    virtual void OnDisconnected();
  //    virtual void OnPeerConnected(std::string id, const std::string& name);
  //    virtual void OnPeerDisconnected(std::string id);

  // void OnMessageFromPeer(std::string peer_id, const std::string &message);
  void OnMessageFromPeer(std::string peer_id, std::vector<char> msgVec);

  //    virtual void OnMessageSent(int err);
  //    virtual void OnServerConnectionFailure();

  AlDataManager *m_dataManager;

protected:
  // Send a message to the remote peer.
  void SendMessage(const std::string &json_object);

  //    std::string peer_id_;
  // bool loopback_;

  rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peer_connection_;
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
