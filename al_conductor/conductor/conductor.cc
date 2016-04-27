/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "conductor.h"

#include <utility>
#include <vector>

#include "talk/app/webrtc/videosourceinterface.h"
#include "defaults.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/app/webrtc/test/fakeconstraints.h"
#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"

#include <iostream>

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";

#define DTLS_ON  true
#define DTLS_OFF false

class DummySetSessionDescriptionObserver
        : public webrtc::SetSessionDescriptionObserver {
public:
    static DummySetSessionDescriptionObserver* Create() {
        return
                new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
    }
    virtual void OnSuccess() {
        LOG(INFO) << __FUNCTION__;
    }
    virtual void OnFailure(const std::string& error) {
        LOG(INFO) << __FUNCTION__ << " " << error;
    }

protected:
    DummySetSessionDescriptionObserver() {}
    ~DummySetSessionDescriptionObserver() {}
};

Conductor::Conductor(PeerConnectionClient* client, MainWnd* main_wnd, QObject *parent)
    : peer_id_("-1"),
      loopback_(false),
      client_(client),
      main_wnd_(main_wnd),
      debug_(true),
      QObject(parent),
      m_processingMsg(false),
      m_isAcceptingConnection(false) {

    client_->RegisterObserver(this);
    main_wnd->RegisterObserver(this);
    connect(this, SIGNAL(DequeueMessagesFromPeerSignal()), this, SLOT(DequeueMessagesFromPeerSlot()));
}

Conductor::~Conductor() {
    ASSERT(peer_connection_.get() == NULL);
}

bool Conductor::connection_active() const {
    return peer_connection_.get() != NULL;
}

void Conductor::Close() {
    client_->SignOut();
    DeletePeerConnection();
}

bool Conductor::InitializePeerConnection() {
    ASSERT(peer_connection_factory_.get() == NULL);
    ASSERT(peer_connection_.get() == NULL);

    peer_connection_factory_  = webrtc::CreatePeerConnectionFactory();

    if (!peer_connection_factory_.get()) {
        main_wnd_->MessageBox("Error",
                              "Failed to initialize PeerConnectionFactory", true);
        DeletePeerConnection();
        return false;
    }

    if (!CreatePeerConnection(DTLS_ON)) {
        main_wnd_->MessageBox("Error",
                              "CreatePeerConnection failed", true);
        DeletePeerConnection();
    }
    AddStreams();
    return peer_connection_.get() != NULL;
}

bool Conductor::ReinitializePeerConnectionForLoopback() {
    loopback_ = true;
    rtc::scoped_refptr<webrtc::StreamCollectionInterface> streams(
                peer_connection_->local_streams());
    peer_connection_ = NULL;
    if (CreatePeerConnection(DTLS_OFF)) {
        for (size_t i = 0; i < streams->count(); ++i)
            peer_connection_->AddStream(streams->at(i));
        peer_connection_->CreateOffer(this, NULL);
    }
    return peer_connection_.get() != NULL;
}

bool Conductor::CreatePeerConnection(bool dtls) {
    ASSERT(peer_connection_factory_.get() != NULL);
    ASSERT(peer_connection_.get() == NULL);

    webrtc::PeerConnectionInterface::RTCConfiguration config;
    webrtc::PeerConnectionInterface::IceServer server;
    server.uri = GetPeerConnectionString();
    config.servers.push_back(server);

    webrtc::FakeConstraints constraints;
    if (dtls) {
        constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
                                "true");
    } else {
        constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
                                "false");
    }

    peer_connection_ = peer_connection_factory_->CreatePeerConnection(
                config, &constraints, NULL, NULL, this);
    return peer_connection_.get() != NULL;
}

void Conductor::DeletePeerConnection() {
    peer_connection_ = NULL;
    active_streams_.clear();
    main_wnd_->StopLocalRenderer();
    main_wnd_->StopRemoteRenderer();
    peer_connection_factory_ = NULL;
    peer_id_ = "-1";
    loopback_ = false;
}

void Conductor::EnsureStreamingUI() {
    ASSERT(peer_connection_.get() != NULL);
    if (main_wnd_->IsWindow()) {
        if (main_wnd_->current_ui() != MainWnd::STREAMING)
            main_wnd_->SwitchToStreamingUI();
    }
}

// **
// * PeerConnectionObserver implementation.
// *

// Called when a remote stream is added
void Conductor::OnAddStream(webrtc::MediaStreamInterface* stream) {
    if (this->debug_) {
        qDebug() << "Conductor::OnAddStream";
    }
    LOG(INFO) << __FUNCTION__ << " " << stream->label();


    stream -> AddRef();
    webrtc::VideoTrackVector tracks = stream -> GetVideoTracks();

    // Only render the first track.
    if (!tracks.empty())
    {
        webrtc::VideoTrackInterface *track = tracks[0];

        // TODO additional method
        // Video track
        this->main_wnd_->StartRemoteRenderer(track);
        //        this->SetRemoteMedia(track);
    }
    stream -> Release();
    //   TODO REMOVE
    //    stream->AddRef();
    //    main_wnd_->QueueUIThreadCallback(NEW_STREAM_ADDED,
    //                                     stream);
}

void Conductor::OnRemoveStream(webrtc::MediaStreamInterface* stream) {
    if (this->debug_) {
        qDebug() << "Conductor::OnRemoveStream";
    }
    LOG(INFO) << __FUNCTION__ << " " << stream->label();
    stream->AddRef();
    main_wnd_->QueueUIThreadCallback(STREAM_REMOVED,
                                     stream);
}

void Conductor::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
    LOG(INFO) << __FUNCTION__ << " " << candidate->sdp_mline_index();
    // For loopback test. To save some connecting delay.
    if (loopback_) {
        if (!peer_connection_->AddIceCandidate(candidate)) {
            LOG(WARNING) << "Failed to apply the received candidate";
        }
        return;
    }

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
    SendMessage(writer.write(jmessage));
}

//
// PeerConnectionClientObserver implementation.
//

void Conductor::OnSignedIn() {
    LOG(INFO) << __FUNCTION__;
    main_wnd_->SwitchToPeerList(client_->peers());
}

void Conductor::OnDisconnected() {
    LOG(INFO) << __FUNCTION__;

    DeletePeerConnection();

    if (main_wnd_->IsWindow())
        main_wnd_->SwitchToConnectUI();
}


void Conductor::OnPeerConnected(QString id, const std::string& name) {
    LOG(INFO) << __FUNCTION__;
    // Refresh the list if we're showing it.
    //    if (main_wnd_->current_ui() == MainWnd::LIST_PEERS)
    //        main_wnd_->SwitchToPeerList(client_->peers());
}

void Conductor::OnPeerDisconnected(QString id) {
    LOG(INFO) << __FUNCTION__;
    if (id == peer_id_) {
        LOG(INFO) << "Our peer disconnected";
        main_wnd_->QueueUIThreadCallback(PEER_CONNECTION_CLOSED, NULL);
    } else {
        // Refresh the list if we're showing it.
        if (main_wnd_->current_ui() == MainWnd::LIST_PEERS)
            main_wnd_->SwitchToPeerList(client_->peers());
    }
}

void Conductor::OnMessageFromPeer(QString peer_id, const QString& message) {
    if (this->debug_) {
        qDebug() << "Conductor::OnMessageFromPeer";
        qDebug() << peer_id_;
        //        qDebug() << message;
    }
    ASSERT(peer_id_ == peer_id || peer_id_ == "-1");
    ASSERT(!message.toStdString().empty());


    if (!peer_connection_.get()) {
        ASSERT(peer_id_ == "-1");
        qDebug() << "< 11";
        qDebug() << peer_id;
        peer_id_ = peer_id;
        //TODO HACK!
        this->m_isAcceptingConnection = true;

        if (!InitializePeerConnection()) {
            qDebug() << "< 12";
            LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
            qDebug() << "Failed to initialize our PeerConnection instance";
            client_->SignOut();
            return;
        }
        qDebug() << "< 13";
    } else if (peer_id != peer_id_) {
        ASSERT(peer_id_ != "-1");
        LOG(WARNING) << "Received a message from unknown peer while already in a "
                        "conversation with a different peer.";
        qDebug() << "Received a message from unknown peer while already in a "
                    "conversation with a different peer.";
        return;
    }

    if (this->debug_) {
        qDebug() << "< 10";
    }

    Json::Reader reader;
    Json::Value jmessage;
    //TODO parced QString
    if (!reader.parse(message.toStdString(), jmessage)) {
        //        LOG(WARNING) << "Received unknown message. " << message;
        qDebug() << "Received unknown message. " << message;
        return;
    }
    std::string type;
    std::string json_object;

    rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionTypeName, &type);
    if (!type.empty()) {
        if (type == "offer-loopback") {
            // This is a loopback call.
            // Recreate the peerconnection with DTLS disabled.
            if (!ReinitializePeerConnectionForLoopback()) {
                LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
                qDebug() << "Failed to initialize our PeerConnection instance";
                DeletePeerConnection();
                client_->SignOut();
            }
            return;
        }

        std::string sdp;
        if (!rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionSdpName,
                                          &sdp)) {
            LOG(WARNING) << "Can't parse received session description message.";
            qDebug() << "Can't parse received session description message.";
            return;
        }
        webrtc::SdpParseError error;
        webrtc::SessionDescriptionInterface* session_description(
                    webrtc::CreateSessionDescription(type, sdp, &error));
        if (!session_description) {
            LOG(WARNING) << "Can't parse received session description message. "
                         << "SdpParseError was: " << error.description;
            qDebug() << "Can't parse received session description message. "
                    << "SdpParseError was: " << QString::fromStdString(error.description);
            return;
        }
        //        LOG(INFO) << " Received session description :" << message;
        qDebug() << " Received session description :" << message;
        peer_connection_->SetRemoteDescription(
                    DummySetSessionDescriptionObserver::Create(), session_description);
        if (session_description->type() ==
                webrtc::SessionDescriptionInterface::kOffer) {
            qDebug() << "peer_connection_->CreateAnswer(this, NULL);";
            peer_connection_->CreateAnswer(this, NULL);
        }
        return;
    } else {
        std::string sdp_mid;
        int sdp_mlineindex = 0;
        std::string sdp;
        if (!rtc::GetStringFromJsonObject(jmessage, kCandidateSdpMidName,
                                          &sdp_mid) ||
                !rtc::GetIntFromJsonObject(jmessage, kCandidateSdpMlineIndexName,
                                           &sdp_mlineindex) ||
                !rtc::GetStringFromJsonObject(jmessage, kCandidateSdpName, &sdp)) {
            LOG(WARNING) << "Can't parse received message.";
            qDebug() << "Can't parse received message.";
            return;
        }
        webrtc::SdpParseError error;
        rtc::scoped_ptr<webrtc::IceCandidateInterface> candidate(
                    webrtc::CreateIceCandidate(sdp_mid, sdp_mlineindex, sdp, &error));
        if (!candidate.get()) {
            LOG(WARNING) << "Can't parse received candidate message. "
                         << "SdpParseError was: " << error.description;
            qDebug() << "Can't parse received candidate message. "
                     << "SdpParseError was: " << QString::fromStdString(error.description);
            return;
        }
        if (!peer_connection_->AddIceCandidate(candidate.get())) {
            LOG(WARNING) << "Failed to apply the received candidate";
            qDebug() << "Failed to apply the received candidate";
            return;
        }
        //        LOG(INFO) << " Received candidate :" << message;
        qDebug() << " Received candidate :" << message;
        return;
    }
}

void Conductor::OnMessageSent(int err) {
    // Process the next pending message if any.
    main_wnd_->QueueUIThreadCallback(SEND_MESSAGE_TO_PEER, NULL);
}

void Conductor::OnServerConnectionFailure() {
    if (this->debug_) {
        qDebug() << "Error " << "Failed to connect to " << QString::fromStdString(server_);
    }
    main_wnd_->MessageBox("Error", ("Failed to connect to " + server_).c_str(),
                          true);
}

//=====================
void Conductor::OnSignedInSlot() {
    if (this->debug_) {
        qDebug() << "Conductor::OnSignedInSlot";
    }
    this->OnSignedIn();
}

void Conductor::OnDisconnectedSlot() {
    this->OnDisconnected();
}

//void Conductor::OnPeerConnectedSlot(QString id, QString name) {
//    this->OnPeerConnected(id, name);
//}

void Conductor::OnPeerDisconnectedSlot(QString id) {
    this->OnPeerDisconnected(id);
}

// enqueue messages for processing
void Conductor::OnMessageFromPeerSlot(QString peer_id, const QString& message) {
    QMap<QString, QString> curMsg;
    curMsg[peer_id] = message;
    this->m_messageQueue.enqueue(curMsg);
    qDebug() << "QUEUE LEN: ";
    qDebug() << this->m_messageQueue.length();
    if (!this->m_processingMsg) {
        Q_EMIT DequeueMessagesFromPeerSignal();
    }
}

void Conductor::DequeueMessagesFromPeerSlot() {
    this->m_processingMsg = true;
    while (!this->m_messageQueue.isEmpty()) {
        qDebug() << "AAAAAAAAAAA!!!!!!!!!!!!!!r";
        QMap<QString, QString> curMsg;
        curMsg = this->m_messageQueue.dequeue();
        QMapIterator<QString, QString> i(curMsg);
        while (i.hasNext()) {
            i.next();
            qDebug() << "this->OnMessageFromPeer";
            qDebug() << i.value();
            this->OnMessageFromPeer(i.key(), i.value());
        }
    }
    this->m_processingMsg = false;
//    Q_EMIT DequeueMessagesFromPeerSignal();
}

void Conductor::OnMessageSentSlot(int err) {
    this->OnMessageSent(err);
}

void Conductor::OnServerConnectionFailureSlot() {
    this->OnServerConnectionFailure();
}

void Conductor::ConnectToPeerSlot(QString peer_id) {
    this->ConnectToPeer(peer_id);
}

//~=====================

//
// MainWndCallback implementation.
//

void Conductor::StartLogin(const std::string& server, int port) {
    if (this->debug_) {
        qDebug() << "Conductor::StartLogin";
        qDebug() << QString::fromStdString(server);
        //        qDebug() << port;
    }
    if (client_->is_connected())
        return;
    server_ = server;
    qDebug() << "server:";
    qDebug() << QString::fromStdString(server);
    client_->Connect(server, port, GetPeerName());
}

void Conductor::DisconnectFromServer() {
    if (client_->is_connected())
        client_->SignOut();
}

void Conductor::ConnectToPeer(QString peer_id) {
    qDebug() << "Conductor::ConnectToPeer";
    ASSERT(peer_id_ == "-1");
    ASSERT(peer_id != "-1");

    if (peer_connection_.get()) {
        main_wnd_->MessageBox("Error",
                              "We only support connecting to one peer at a time", true);
        return;
    }

    if (InitializePeerConnection()) {
        peer_id_ = peer_id;
        peer_connection_->CreateOffer(this, NULL);
    } else {
        main_wnd_->MessageBox("Error", "Failed to initialize PeerConnection", true);
    }
}

cricket::VideoCapturer* Conductor::OpenVideoCaptureDevice() {
    rtc::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(
                cricket::DeviceManagerFactory::Create());
    if (!dev_manager->Init()) {
        LOG(LS_ERROR) << "Can't create device manager";
        qDebug() << "Can't create device manager";
        return NULL;
    }
    std::vector<cricket::Device> devs;
    if (!dev_manager->GetVideoCaptureDevices(&devs)) {
        LOG(LS_ERROR) << "Can't enumerate video devices";
        qDebug() << "Can't enumerate video devices";
        return NULL;
    }
    std::vector<cricket::Device>::iterator dev_it = devs.begin();
    cricket::VideoCapturer* capturer = NULL;
    //TODO HACK for 2 devices on 1 machine
    int iter = 0;
    for (; dev_it != devs.end(); ++dev_it) {
        if (!this->m_isAcceptingConnection || iter == 1) {
            capturer = dev_manager->CreateVideoCapturer(*dev_it);
            if (capturer != NULL)
                break;
        }
        iter++;
    }
    qDebug() << "RETURNING CAPTURER!!!!";
    qDebug() << capturer;
    return capturer;
}

void Conductor::AddStreams() {
    qDebug() << "Conductor::AddStreams";
    if (active_streams_.find(kStreamLabel) != active_streams_.end()) {
        qDebug() << "Already added!!!!!!!!";
        return;  // Already added.
    }

    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
                peer_connection_factory_->CreateAudioTrack(
                    kAudioLabel, peer_connection_factory_->CreateAudioSource(NULL)));

    rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track(
                peer_connection_factory_->CreateVideoTrack(
                    kVideoLabel,
                    peer_connection_factory_->CreateVideoSource(OpenVideoCaptureDevice(),
                                                                NULL)));
    main_wnd_->StartLocalRenderer(video_track);

    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream =
            peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);

    stream->AddTrack(audio_track);
    stream->AddTrack(video_track);
    qDebug() << "ADDDINGGGG STREAM!!!";
    if (!peer_connection_->AddStream(stream)) {
        LOG(LS_ERROR) << "Adding stream to PeerConnection failed";
        qDebug() << "Adding stream to PeerConnection failed";
    }
    typedef std::pair<std::string,
            rtc::scoped_refptr<webrtc::MediaStreamInterface> >
            MediaStreamPair;
    active_streams_.insert(MediaStreamPair(stream->label(), stream));
    main_wnd_->SwitchToStreamingUI();
}

void Conductor::DisconnectFromCurrentPeer() {
    LOG(INFO) << __FUNCTION__;
    if (peer_connection_.get()) {
        //        client_->SendHangUp(peer_id_);
        Q_EMIT SendHangUpSignal(peer_id_);
        DeletePeerConnection();
    }

    if (main_wnd_->IsWindow())
        main_wnd_->SwitchToPeerList(client_->peers());
}

void Conductor::UIThreadCallback(int msg_id, void* data) {
    if (this->debug_) {
        qDebug() << "Conductor::UIThreadCallback";
    }
    switch (msg_id) {
    case PEER_CONNECTION_CLOSED:
        LOG(INFO) << "PEER_CONNECTION_CLOSED";
        DeletePeerConnection();

        ASSERT(active_streams_.empty());

        if (main_wnd_->IsWindow()) {
            if (client_->is_connected()) {
                main_wnd_->SwitchToPeerList(client_->peers());
            } else {
                main_wnd_->SwitchToConnectUI();
            }
        } else {
            DisconnectFromServer();
        }
        break;

    case SEND_MESSAGE_TO_PEER: {
        LOG(INFO) << "SEND_MESSAGE_TO_PEER";
        std::string* msg = reinterpret_cast<std::string*>(data);
        if (msg) {
            // For convenience, we always run the message through the queue.
            // This way we can be sure that messages are sent to the server
            // in the same order they were signaled without much hassle.
            pending_messages_.push_back(msg);
        }

        if (!pending_messages_.empty() && !client_->IsSendingMessage()) {
            msg = pending_messages_.front();
            pending_messages_.pop_front();

            // TODO hangle failed sending
            qDebug() << "Q_EMIT SendToPeerSignal(peer_id_, QString::fromStdString(*msg));";
            qDebug() << peer_id_;
            Q_EMIT SendToPeerSignal(peer_id_, QString::fromStdString(*msg));
            //            if (!client_->SendToPeer(peer_id_, *msg) && peer_id_ != "-1") {
            //                LOG(LS_ERROR) << "SendToPeer failed";
            //                DisconnectFromServer();
            //            }
            delete msg;
        }

        if (!peer_connection_.get())
            peer_id_ = "-1";

        break;
    }

    case NEW_STREAM_ADDED: {
        webrtc::MediaStreamInterface* stream =
                reinterpret_cast<webrtc::MediaStreamInterface*>(
                    data);
        webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
        // Only render the first track.
        if (!tracks.empty()) {
            webrtc::VideoTrackInterface* track = tracks[0];
            main_wnd_->StartRemoteRenderer(track);
        }
        stream->Release();
        break;
    }

    case STREAM_REMOVED: {
        // Remote peer stopped sending a stream.
        webrtc::MediaStreamInterface* stream =
                reinterpret_cast<webrtc::MediaStreamInterface*>(
                    data);
        stream->Release();
        break;
    }

    default:
        ASSERT(false);
        break;
    }
}

void Conductor::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
    peer_connection_->SetLocalDescription(
                DummySetSessionDescriptionObserver::Create(), desc);

    std::string sdp;
    desc->ToString(&sdp);

    // For loopback test. To save some connecting delay.
    if (loopback_) {
        // Replace message type from "offer" to "answer"
        webrtc::SessionDescriptionInterface* session_description(
                    webrtc::CreateSessionDescription("answer", sdp, nullptr));
        peer_connection_->SetRemoteDescription(
                    DummySetSessionDescriptionObserver::Create(), session_description);
        return;
    }

    Json::StyledWriter writer;
    Json::Value jmessage;
    jmessage[kSessionDescriptionTypeName] = desc->type();
    jmessage[kSessionDescriptionSdpName] = sdp;
    SendMessage(writer.write(jmessage));
}

void Conductor::OnFailure(const std::string& error) {
    LOG(LERROR) << error;
}

void Conductor::SendMessage(const std::string& json_object) {
    std::string* msg = new std::string(json_object);
    main_wnd_->QueueUIThreadCallback(SEND_MESSAGE_TO_PEER, msg);
}

