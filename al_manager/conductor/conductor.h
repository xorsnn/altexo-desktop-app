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
//#include "mainwindow.h"
#include "peer_connection_client.h"
#include "webrtc/base/scoped_ptr.h"

#include "talk/app/webrtc/videosourceinterface.h"
#include "defaults.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/app/webrtc/test/fakeconstraints.h"
#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"

//#include <QQueue>
//#include <QMap>
#include "alcallback.h"



namespace webrtc {
class VideoCaptureModule;
}  // namespace webrtc

namespace cricket {
class VideoRenderer;
}  // namespace cricket



//class MainWndCallback {
//public:
//    virtual void StartLogin(const std::string& server, int port) = 0;
//    virtual void DisconnectFromServer() = 0;
//    virtual void ConnectToPeer(QString peer_id) = 0;
//    virtual void DisconnectFromCurrentPeer() = 0;
//    virtual void UIThreadCallback(int msg_id, void* data) = 0;
//    virtual void Close() = 0;
//protected:
//    virtual ~MainWndCallback() {}
//};

class Conductor:
        public webrtc::PeerConnectionObserver,
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
    };

    Conductor(PeerConnectionClient* client, AlCallback* alCallback);

    bool connection_active() const;

    virtual void Close();

    // TODO may be make protected or private
    bool InitializePeerConnection();
    bool ReinitializePeerConnectionForLoopback();
    bool CreatePeerConnection(bool dtls);
    void DeletePeerConnection();
protected:
    ~Conductor();
//    bool InitializePeerConnection();
//    bool ReinitializePeerConnectionForLoopback();
//    bool CreatePeerConnection(bool dtls);
//    void DeletePeerConnection();
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


//    //
//    // MainWndCallback implementation.
//    //

//    virtual void StartLogin(const std::string& server, int port);
//    virtual void DisconnectFromServer();
//    virtual void ConnectToPeer(QString peer_id);
//    virtual void DisconnectFromCurrentPeer();
//    virtual void UIThreadCallback(int msg_id, void* data);

    // CreateSessionDescriptionObserver implementation.
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    virtual void OnFailure(const std::string& error);

//    //
//    // PeerConnectionClientObserver implementation.
//    //
//    virtual void OnSignedIn();
//    virtual void OnDisconnected();
//    virtual void OnPeerConnected(std::string id, const std::string& name);
//    virtual void OnPeerDisconnected(std::string id);
//    virtual void OnMessageFromPeer(std::string peer_id, const std::string& message);
//    virtual void OnMessageSent(int err);
//    virtual void OnServerConnectionFailure();

    //TODO may be not nesessary
////public Q_SLOTS:
//    void OnSignedInSlot();
//    void OnDisconnectedSlot();
//    void OnPeerDisconnectedSlot(std::string id);
//    void OnMessageFromPeerSlot(std::string peer_id, const std::string& message);
//    void OnMessageSentSlot(int err);
//    void OnServerConnectionFailureSlot();

//    void ConnectToPeerSlot(QString peer_id);
//    void DequeueMessagesFromPeerSlot();

// TODO replace to AlCallback
//Q_SIGNALS:
//    void SendToPeerSignal(QString peer_id, const QString &message);
//    void SendHangUpSignal(QString peer_id_);
//    void DequeueMessagesFromPeerSignal();

protected:
    // Send a message to the remote peer.
    void SendMessage(const std::string& json_object);

//    std::string peer_id_;
    bool loopback_;

    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
    peer_connection_factory_;
//    PeerConnectionClient* client_;
    AlCallback* m_alCallback;
//    std::deque<std::string*> pending_messages_;

    std::string server_;

    bool debug_;
    std::queue<std::map<std::string, std::string>> m_messageQueue;
    bool m_processingMsg;
    bool m_isAcceptingConnection;

public:
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peerConnection;
    std::map<std::string, rtc::scoped_refptr<webrtc::MediaStreamInterface>> m_activeStreams;
};

#endif  // WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
