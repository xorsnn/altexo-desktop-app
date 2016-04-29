/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "conductor/conductor.h"

#include <utility>
#include <vector>

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

Conductor::Conductor(PeerConnectionClient* client, AlCallback* alCallback, AlDataManager* alDataManager)
    : loopback_(false),
//      client_(client),
      m_alCallback(alCallback),
      debug_(true),
      m_processingMsg(false),
      m_isAcceptingConnection(false) {

    m_dataManager = alDataManager;
//    client_->RegisterObserver(this);
//    m_alCallback->RegisterObserver(this);
//    TODO clean up  after it will be handled with callback
//    connect(this, SIGNAL(DequeueMessagesFromPeerSignal()), this, SLOT(DequeueMessagesFromPeerSlot()));
}

Conductor::~Conductor() {
    ASSERT(m_peerConnection.get() == NULL);
}

bool Conductor::connection_active() const {
    return m_peerConnection.get() != NULL;
}

void Conductor::Close() {
//    TODO move to callback
//    client_->SignOut();
    DeletePeerConnection();
}

bool Conductor::InitializePeerConnection() {
    ASSERT(peer_connection_factory_.get() == NULL);
    ASSERT(m_peerConnection.get() == NULL);

    peer_connection_factory_  = webrtc::CreatePeerConnectionFactory();

    if (!peer_connection_factory_.get()) {
//        m_alCallback->MessageBox("Error",
//                              "Failed to initialize PeerConnectionFactory", true);
        std::cout << "Error, Failed to initialize PeerConnectionFactory" << std::endl;
        DeletePeerConnection();
        return false;
    }

    if (!CreatePeerConnection(DTLS_ON)) {
//        m_alCallback->MessageBox("Error",
//                              "CreatePeerConnection failed", true);
        std::cout << "Error, CreatePeerConnection failed" << std::endl;
        DeletePeerConnection();
    }
    AddStreams();
    return m_peerConnection.get() != NULL;
}

bool Conductor::ReinitializePeerConnectionForLoopback() {
    loopback_ = true;
    rtc::scoped_refptr<webrtc::StreamCollectionInterface> streams(
                m_peerConnection->local_streams());
    m_peerConnection = NULL;
    if (CreatePeerConnection(DTLS_OFF)) {
        for (size_t i = 0; i < streams->count(); ++i)
            m_peerConnection->AddStream(streams->at(i));
        m_peerConnection->CreateOffer(this, NULL);
    }
    return m_peerConnection.get() != NULL;
}

bool Conductor::CreatePeerConnection(bool dtls) {
    ASSERT(peer_connection_factory_.get() != NULL);
    ASSERT(m_peerConnection.get() == NULL);

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

    m_peerConnection = peer_connection_factory_->CreatePeerConnection(
                config, &constraints, NULL, NULL, this);
    return m_peerConnection.get() != NULL;
}

void Conductor::DeletePeerConnection() {
    m_peerConnection = NULL;
    m_activeStreams.clear();
    m_alCallback->stopLocalRendererCb();
    m_alCallback->stopRemoteRendererCb();
    peer_connection_factory_ = NULL;
//    peer_id_ = "-1";
    loopback_ = false;
}

void Conductor::EnsureStreamingUI() {
    ASSERT(m_peerConnection.get() != NULL);
    m_alCallback->ensureStreamingUICb();
//    if (m_alCallback->IsWindow()) {
//        if (m_alCallback->current_ui() != MainWnd::STREAMING)
//            m_alCallback->SwitchToStreamingUI();
//    }
}

// **
// * PeerConnectionObserver implementation.
// *

// Called when a remote stream is added
void Conductor::OnAddStream(webrtc::MediaStreamInterface* stream) {
    if (this->debug_) {
        std::cout << "Conductor::OnAddStream" << std::endl;
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

//        this->m_alCallback->startRemoteRendererCb(track);
        std::cout << "============qwe=============" << std::endl;
        this->m_dataManager->startRemoteRenderer(track);

        //        this->SetRemoteMedia(track);
    }
    stream -> Release();
    //   TODO REMOVE
    //    stream->AddRef();
    //    m_alCallback->queueUIThreadCallbackCb(NEW_STREAM_ADDED,
    //                                     stream);
}

void Conductor::OnRemoveStream(webrtc::MediaStreamInterface* stream) {
    if (this->debug_) {
        std::cout << "Conductor::OnRemoveStream" << std::endl;
    }
    LOG(INFO) << __FUNCTION__ << " " << stream->label();
    stream->AddRef();
    m_alCallback->queueUIThreadCallbackCb(STREAM_REMOVED,
                                     stream);
}

void Conductor::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
    LOG(INFO) << __FUNCTION__ << " " << candidate->sdp_mline_index();
    // For loopback test. To save some connecting delay.
    if (loopback_) {
        if (!m_peerConnection->AddIceCandidate(candidate)) {
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
    //TODO move to callback
    SendMessage(writer.write(jmessage));
}

////
//// PeerConnectionClientObserver implementation.
////

//void Conductor::OnSignedIn() {
//    LOG(INFO) << __FUNCTION__;
//    m_alCallback->switchToPeerListCb(client_->peers());
//}

//void Conductor::OnDisconnected() {
//    LOG(INFO) << __FUNCTION__;

//    DeletePeerConnection();

//    m_alCallback->onDisconnectedCb();
////    TODO remove when cleaning up
////    if (m_alCallback->IsWindow())
////        m_alCallback->SwitchToConnectUI();
//}


//void Conductor::OnPeerConnected(std::string id, const std::string& name) {
//    LOG(INFO) << __FUNCTION__;
//    // Refresh the list if we're showing it.
//    //    if (m_alCallback->current_ui() == MainWnd::LIST_PEERS)
//    //        m_alCallback->switchToPeerListCb(client_->peers());
//}

//void Conductor::OnPeerDisconnected(std::string id) {
//    LOG(INFO) << __FUNCTION__;
//    if (id == peer_id_) {
//        LOG(INFO) << "Our peer disconnected";
//        m_alCallback->queueUIThreadCallbackCb(PEER_CONNECTION_CLOSED, NULL);
//    }
//    //TODO remove when cleaning up
////    else {
//        // Refresh the list if we're showing it.
////        if (m_alCallback->current_ui() == MainWnd::LIST_PEERS)
////            m_alCallback->switchToPeerListCb(client_->peers());
////    }
//}

void Conductor::OnMessageFromPeer(std::string peer_id, const std::string& message) {
    if (this->debug_) {
        std::cout << "Conductor::OnMessageFromPeer" << std::endl;
        std::cout << peer_id << std::endl;
    }
//    ASSERT(peer_id_ == peer_id || peer_id_ == "-1");
    ASSERT(!message.empty());


    if (!m_peerConnection.get()) {
//        ASSERT(peer_id_ == "-1");
//        TODO remove when cleaning up
        std::cout << "< 11" << std::endl;
        std::cout << peer_id << std::endl;
//        peer_id_ = peer_id;
        //TODO HACK!
        this->m_isAcceptingConnection = true;

        if (!InitializePeerConnection()) {
            std::cout << "< 12" << std::endl;
            LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
            std::cout << "Failed to initialize our PeerConnection instance" << std::endl;
//            TODO see
//            client_->SignOut();
            return;
        }
        std::cout << "< 13" << std::endl;
    }
//    else if (peer_id != peer_id_) {
//        ASSERT(peer_id_ != "-1");
//        LOG(WARNING) << "Received a message from unknown peer while already in a "
//                        "conversation with a different peer.";
//        std::cout << "Received a message from unknown peer while already in a "
//                    "conversation with a different peer." << std::endl;
//        return;
//    }

    if (this->debug_) {
        std::cout << "< 10" << std::endl;
    }

    Json::Reader reader;
    Json::Value jmessage;
    //TODO parced QString
    if (!reader.parse(message, jmessage)) {
        //        LOG(WARNING) << "Received unknown message. " << message;
        std::cout << "Received unknown message. " << message << std::endl;
        return;
    }
    std::string type;
    std::string json_object;

    rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionTypeName, &type);
    if (!type.empty()) {
//        if (type == "offer-loopback") {
//            // This is a loopback call.
//            // Recreate the peerconnection with DTLS disabled.
//            if (!ReinitializePeerConnectionForLoopback()) {
//                LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
//                std::cout << "Failed to initialize our PeerConnection instance" << std::endl;
//                DeletePeerConnection();
////                TODO take a look
////                client_->SignOut();
//            }
//            return;
//        }

        std::string sdp;
        if (!rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionSdpName,
                                          &sdp)) {
            LOG(WARNING) << "Can't parse received session description message.";
            std::cout << "Can't parse received session description message." << std::endl;
            return;
        }
        webrtc::SdpParseError error;
        webrtc::SessionDescriptionInterface* session_description(
                    webrtc::CreateSessionDescription(type, sdp, &error));
        if (!session_description) {
            LOG(WARNING) << "Can't parse received session description message. "
                         << "SdpParseError was: " << error.description;
            std::cout << "Can't parse received session description message. "
                    << "SdpParseError was: " << error.description << std::endl;
            return;
        }
        //        LOG(INFO) << " Received session description :" << message;
        std::cout << " Received session description :" << message << std::endl;
        m_peerConnection->SetRemoteDescription(
                    DummySetSessionDescriptionObserver::Create(), session_description);
        if (session_description->type() ==
                webrtc::SessionDescriptionInterface::kOffer) {
            std::cout << "m_peerConnection->CreateAnswer(this, NULL);" << std::endl;
            m_peerConnection->CreateAnswer(this, NULL);
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
            std::cout << "Can't parse received message." << std::endl;
            return;
        }
        webrtc::SdpParseError error;
        rtc::scoped_ptr<webrtc::IceCandidateInterface> candidate(
                    webrtc::CreateIceCandidate(sdp_mid, sdp_mlineindex, sdp, &error));
        if (!candidate.get()) {
            LOG(WARNING) << "Can't parse received candidate message. "
                         << "SdpParseError was: " << error.description;
            std::cout << "Can't parse received candidate message. "
                     << "SdpParseError was: " << error.description << std::endl;
            return;
        }
        if (!m_peerConnection->AddIceCandidate(candidate.get())) {
            LOG(WARNING) << "Failed to apply the received candidate";
            std::cout << "Failed to apply the received candidate" << std::endl;
            return;
        }
        //        LOG(INFO) << " Received candidate :" << message;
        std::cout << " Received candidate :" << message << std::endl;
        return;
    }
}

//void Conductor::OnMessageSent(int err) {
//    // Process the next pending message if any.
//    m_alCallback->queueUIThreadCallbackCb(SEND_MESSAGE_TO_PEER, NULL);
//}

//void Conductor::OnServerConnectionFailure() {
//    if (this->debug_) {
//        std::cout << "Error " << "Failed to connect to " << server_ << std::endl;
//    }
////    TODO remove when cleaning up
////    m_alCallback->MessageBox("Error", ("Failed to connect to " + server_).c_str(),
////                          true);
//}

////=====================
//void Conductor::OnSignedInSlot() {
//    if (this->debug_) {
//        std::cout << "Conductor::OnSignedInSlot" << std::endl;
//    }
//    this->OnSignedIn();
//}

//void Conductor::OnDisconnectedSlot() {
//    this->OnDisconnected();
//}

////void Conductor::OnPeerConnectedSlot(QString id, QString name) {
////    this->OnPeerConnected(id, name);
////}

//void Conductor::OnPeerDisconnectedSlot(std::string id) {
//    this->OnPeerDisconnected(id);
//}

//// enqueue messages for processing
//void Conductor::OnMessageFromPeerSlot(std::string peer_id, const std::string& message) {
//    std::map<std::string, std::string> curMsg;
//    curMsg[peer_id] = message;
//    this->m_messageQueue.push(curMsg);
//    std::cout << "QUEUE LEN: " << std::endl;
//    std::cout << this->m_messageQueue.size() << std::endl;
//    if (!this->m_processingMsg) {
//        Q_EMIT DequeueMessagesFromPeerSignal();
//    }
//}

//void Conductor::DequeueMessagesFromPeerSlot() {
//    this->m_processingMsg = true;
//    while (!this->m_messageQueue.isEmpty()) {
//        qDebug() << "AAAAAAAAAAA!!!!!!!!!!!!!!r";
//        QMap<QString, QString> curMsg;
//        curMsg = this->m_messageQueue.dequeue();
//        QMapIterator<QString, QString> i(curMsg);
//        while (i.hasNext()) {
//            i.next();
//            qDebug() << "this->OnMessageFromPeer";
//            qDebug() << i.value();
//            this->OnMessageFromPeer(i.key(), i.value());
//        }
//    }
//    this->m_processingMsg = false;
////    Q_EMIT DequeueMessagesFromPeerSignal();
//}

//void Conductor::OnMessageSentSlot(int err) {
//    this->OnMessageSent(err);
//}

//void Conductor::OnServerConnectionFailureSlot() {
//    this->OnServerConnectionFailure();
//}

//void Conductor::ConnectToPeerSlot(QString peer_id) {
//    this->ConnectToPeer(peer_id);
//}

//~=====================

////
//// MainWndCallback implementation.
////

//void Conductor::StartLogin(const std::string& server, int port) {
//    if (this->debug_) {
//        qDebug() << "Conductor::StartLogin";
//        qDebug() << QString::fromStdString(server);
//        //        qDebug() << port;
//    }
//    if (client_->is_connected())
//        return;
//    server_ = server;
//    qDebug() << "server:";
//    qDebug() << QString::fromStdString(server);
//    client_->Connect(server, port, GetPeerName());
//}

//void Conductor::DisconnectFromServer() {
//    if (client_->is_connected())
//        client_->SignOut();
//}

//void Conductor::ConnectToPeer(QString peer_id) {
//    qDebug() << "Conductor::ConnectToPeer";
//    ASSERT(peer_id_ == "-1");
//    ASSERT(peer_id != "-1");

//    if (m_peerConnection.get()) {
//        m_alCallback->MessageBox("Error",
//                              "We only support connecting to one peer at a time", true);
//        return;
//    }

//    if (InitializePeerConnection()) {
//        peer_id_ = peer_id;
//        m_peerConnection->CreateOffer(this, NULL);
//    } else {
//        m_alCallback->MessageBox("Error", "Failed to initialize PeerConnection", true);
//    }
//}

cricket::VideoCapturer* Conductor::OpenVideoCaptureDevice() {
    rtc::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(
                cricket::DeviceManagerFactory::Create());
    if (!dev_manager->Init()) {
        LOG(LS_ERROR) << "Can't create device manager";
        std::cout << "Can't create device manager" << std::endl;
        return NULL;
    }
    std::vector<cricket::Device> devs;
    if (!dev_manager->GetVideoCaptureDevices(&devs)) {
        LOG(LS_ERROR) << "Can't enumerate video devices";
        std::cout << "Can't enumerate video devices" << std::endl;
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
    std::cout << "RETURNING CAPTURER!!!!" << std::endl;
    std::cout << capturer << std::endl;
    return capturer;
}

void Conductor::AddStreams() {
    std::cout << "Conductor::AddStreams" << std::endl;
    if (m_activeStreams.find(kStreamLabel) != m_activeStreams.end()) {
        std::cout << "Already added!!!!!!!!" << std::endl;
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
    m_alCallback->startLocalRendererCb(video_track);

    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream =
            peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);

    stream->AddTrack(audio_track);
    stream->AddTrack(video_track);
    std::cout << "ADDDINGGGG STREAM!!!" << std::endl;
    if (!m_peerConnection->AddStream(stream)) {
        LOG(LS_ERROR) << "Adding stream to PeerConnection failed";
        std::cout << "Adding stream to PeerConnection failed" << std::endl;
    }
    typedef std::pair<std::string,
            rtc::scoped_refptr<webrtc::MediaStreamInterface> >
            MediaStreamPair;
    m_activeStreams.insert(MediaStreamPair(stream->label(), stream));
    m_alCallback->SwitchToStreamingUI();
}

//void Conductor::DisconnectFromCurrentPeer() {
//    LOG(INFO) << __FUNCTION__;
//    if (m_peerConnection.get()) {
//        //        client_->SendHangUp(peer_id_);
//        Q_EMIT SendHangUpSignal(peer_id_);
//        DeletePeerConnection();
//    }

//    if (m_alCallback->IsWindow())
//        m_alCallback->switchToPeerListCb(client_->peers());
//}

//void Conductor::UIThreadCallback(int msg_id, void* data) {
//    if (this->debug_) {
//        qDebug() << "Conductor::UIThreadCallback";
//    }
//    switch (msg_id) {
//    case PEER_CONNECTION_CLOSED:
//        LOG(INFO) << "PEER_CONNECTION_CLOSED";
//        DeletePeerConnection();

//        ASSERT(m_activeStreams.empty());

//        if (m_alCallback->IsWindow()) {
//            if (client_->is_connected()) {
//                m_alCallback->switchToPeerListCb(client_->peers());
//            } else {
//                m_alCallback->SwitchToConnectUI();
//            }
//        } else {
//            DisconnectFromServer();
//        }
//        break;

//    case SEND_MESSAGE_TO_PEER: {
//        LOG(INFO) << "SEND_MESSAGE_TO_PEER";
//        std::string* msg = reinterpret_cast<std::string*>(data);
//        if (msg) {
//            // For convenience, we always run the message through the queue.
//            // This way we can be sure that messages are sent to the server
//            // in the same order they were signaled without much hassle.
//            pending_messages_.push_back(msg);
//        }

//        if (!pending_messages_.empty() && !client_->IsSendingMessage()) {
//            msg = pending_messages_.front();
//            pending_messages_.pop_front();

//            // TODO hangle failed sending
//            qDebug() << "Q_EMIT SendToPeerSignal(peer_id_, QString::fromStdString(*msg));";
//            qDebug() << peer_id_;
//            Q_EMIT SendToPeerSignal(peer_id_, QString::fromStdString(*msg));
//            //            if (!client_->SendToPeer(peer_id_, *msg) && peer_id_ != "-1") {
//            //                LOG(LS_ERROR) << "SendToPeer failed";
//            //                DisconnectFromServer();
//            //            }
//            delete msg;
//        }

//        if (!m_peerConnection.get())
//            peer_id_ = "-1";

//        break;
//    }

//    case NEW_STREAM_ADDED: {
//        webrtc::MediaStreamInterface* stream =
//                reinterpret_cast<webrtc::MediaStreamInterface*>(
//                    data);
//        webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
//        // Only render the first track.
//        if (!tracks.empty()) {
//            webrtc::VideoTrackInterface* track = tracks[0];
//            m_alCallback->startRemoteRendererCb(track);
//        }
//        stream->Release();
//        break;
//    }

//    case STREAM_REMOVED: {
//        // Remote peer stopped sending a stream.
//        webrtc::MediaStreamInterface* stream =
//                reinterpret_cast<webrtc::MediaStreamInterface*>(
//                    data);
//        stream->Release();
//        break;
//    }

//    default:
//        ASSERT(false);
//        break;
//    }
//}

// **
// * CreateSessionDescriptionObserver implementation.
// *
void Conductor::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
    m_peerConnection->SetLocalDescription(
                DummySetSessionDescriptionObserver::Create(), desc);

    std::string sdp;
    desc->ToString(&sdp);

    // For loopback test. To save some connecting delay.
    if (loopback_) {
        // Replace message type from "offer" to "answer"
        webrtc::SessionDescriptionInterface* session_description(
                    webrtc::CreateSessionDescription("answer", sdp, nullptr));
        m_peerConnection->SetRemoteDescription(
                    DummySetSessionDescriptionObserver::Create(), session_description);
        return;
    }

    Json::StyledWriter writer;
    Json::Value jmessage;
    jmessage[kSessionDescriptionTypeName] = desc->type();
    jmessage[kSessionDescriptionSdpName] = sdp;
//    TODO move to callback
    SendMessage(writer.write(jmessage));
}

void Conductor::OnFailure(const std::string& error) {
    LOG(LERROR) << error;
}

void Conductor::SendMessage(const std::string& json_object) {
    std::string* msg = new std::string(json_object);
    m_alCallback->queueUIThreadCallbackCb(SEND_MESSAGE_TO_PEER, msg);
}

