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

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "mainwindow.h"
#include "peer_connection_client.h"
#include "webrtc/base/scoped_ptr.h"


namespace webrtc {
class VideoCaptureModule;
}  // namespace webrtc

namespace cricket {
class VideoRenderer;
}  // namespace cricket

class Conductor
        : public QObject,
        public webrtc::PeerConnectionObserver,
        public webrtc::CreateSessionDescriptionObserver,
        public PeerConnectionClientObserver,
        public MainWndCallback {
    Q_OBJECT
public:

    enum CallbackID {
        MEDIA_CHANNELS_INITIALIZED = 1,
        PEER_CONNECTION_CLOSED,
        SEND_MESSAGE_TO_PEER,
        NEW_STREAM_ADDED,
        STREAM_REMOVED,
    };

    explicit Conductor(PeerConnectionClient* client, MainWnd* main_wnd, QObject *parent = 0);

    bool connection_active() const;

    virtual void Close();

protected:
    ~Conductor();
    bool InitializePeerConnection();
    bool ReinitializePeerConnectionForLoopback();
    bool CreatePeerConnection(bool dtls);
    void DeletePeerConnection();
    void EnsureStreamingUI();
    void AddStreams();
    cricket::VideoCapturer* OpenVideoCaptureDevice();

    //
    // PeerConnectionObserver implementation.
    //
    virtual void OnStateChange(
            webrtc::PeerConnectionObserver::StateType state_changed) {}
    virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
    virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
    virtual void OnDataChannel(webrtc::DataChannelInterface* channel) {}
    virtual void OnRenegotiationNeeded() {}
    virtual void OnIceChange() {}
    virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);


    //
    // MainWndCallback implementation.
    //

    virtual void StartLogin(const std::string& server, int port);
    virtual void DisconnectFromServer();
    virtual void ConnectToPeer(QString peer_id);
    virtual void DisconnectFromCurrentPeer();
    virtual void UIThreadCallback(int msg_id, void* data);

    // CreateSessionDescriptionObserver implementation.
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    virtual void OnFailure(const std::string& error);

    //
    // PeerConnectionClientObserver implementation.
    //
    virtual void OnSignedIn();
    virtual void OnDisconnected();
    virtual void OnPeerConnected(QString id, const std::string& name);
    virtual void OnPeerDisconnected(QString id);
    virtual void OnMessageFromPeer(QString peer_id, const std::string& message);
    virtual void OnMessageSent(int err);
    virtual void OnServerConnectionFailure();

    //TODO may be not nesessary
public Q_SLOTS:
    void OnSignedInSlot();
    void OnDisconnectedSlot();
    void OnPeerConnectedSlot(QString id, const std::string& name);
    void OnPeerDisconnectedSlot(QString id);
    void OnMessageFromPeerSlot(QString peer_id, const std::string& message);
    void OnMessageSentSlot(int err);
    void OnServerConnectionFailureSlot();

Q_SIGNALS:
    void SendToPeerSignal(QString peer_id, const QString &message);
    void SendHangUpSignal(QString peer_id_);

protected:
    // Send a message to the remote peer.
    void SendMessage(const std::string& json_object);

    QString peer_id_;
    bool loopback_;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
    peer_connection_factory_;
    PeerConnectionClient* client_;
    MainWnd* main_wnd_;
    std::deque<std::string*> pending_messages_;
    std::map<std::string, rtc::scoped_refptr<webrtc::MediaStreamInterface> >
    active_streams_;
    std::string server_;

    bool debug_;
};

#endif  // WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
