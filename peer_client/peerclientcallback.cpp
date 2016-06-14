#include "peerclientcallback.h"
#include <iostream>

PeerClientCallback::PeerClientCallback(QApplication* app, QObject *parent) :
    QObject(parent),
    m_debug(true),
    m_peerId("-1"),
    m_manager(NULL),
    m_processingMsg(false)
{
    std::cout << "PeerClientCallback::PeerClientCallback" << std::endl;
    m_app = app;
}

void PeerClientCallback::sendToPeerCb(std::string peer_id, const std::string &message) {

}

void PeerClientCallback::sendHangUpCb(std::string peer_id) {
    std::cout << "PeerClientCallback::sendHangUpCb" << std::endl;
    std::cout << peer_id << std::endl;
}

void PeerClientCallback::dequeueMessagesFromPeerCb() {

}

void PeerClientCallback::stopLocalRendererCb() {

}

void PeerClientCallback::stopRemoteRendererCb() {

}

void PeerClientCallback::ensureStreamingUICb() {

}

void PeerClientCallback::startRemoteRendererCb(webrtc::VideoTrackInterface*) {

}

void PeerClientCallback::startLocalRendererCb(webrtc::VideoTrackInterface*) {

}

void PeerClientCallback::queueUIThreadCallbackCb(int msg_id, void* data) {
    UIThreadCallback(msg_id, data);
}

void PeerClientCallback::switchToPeerListCb(const Peers& peers) {

}

void PeerClientCallback::onDisconnectedCb() {

}

void PeerClientCallback::processUiEventsCb() {
    //    std::cout << "PeerClientCallback::processUiEventsCb" << std::endl;
    m_app->processEvents();
}

void PeerClientCallback::SwitchToStreamingUI() // ????
{

}

// **
// * SLOTS
// *

void PeerClientCallback::OnSignedInSlot() {

}

void PeerClientCallback::OnDisconnectedSlot() {

}

void PeerClientCallback::OnPeerDisconnectedSlot(std::string id) {

}

void PeerClientCallback::OnMessageFromPeerSlot(std::string peer_id, const std::string& message) {
    std::cout << "PeerClientCallback::OnMessageFromPeerSlot" << std::endl;
    if (m_peerId == "-1") {
        m_peerId = peer_id;
    }
    QMap<QString, QString> curMsg;
    curMsg[QString::fromStdString(peer_id)] = QString::fromStdString(message);
    m_messageQueue.enqueue(curMsg);
    qDebug() << "QUEUE LEN: ";
    qDebug() << m_messageQueue.length();
    if (!this->m_processingMsg) {
        //        Q_EMIT DequeueMessagesFromPeerSignal();
        DequeueMessagesFromPeerSlot();
    }
}

void PeerClientCallback::DequeueMessagesFromPeerSlot() {
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
            //            TODO restore
            //            this->OnMessageFromPeer(i.key(), i.value());
            m_manager->OnMessageFromPeer(i.key().toStdString(), i.value().toStdString());
        }
    }
    this->m_processingMsg = false;
    //    Q_EMIT DequeueMessagesFromPeerSignal();
}

//void PeerClientCallback::OnMessageFromPeer(std::string peer_id, const std::string& message) {
//    if (this->m_debug) {
//        std::cout << "Conductor::OnMessageFromPeer" << std::endl;
//        std::cout << m_peerId << std::endl;
//    }
//    ASSERT(m_peerId == peer_id || m_debug == "-1");
//    ASSERT(!message.empty());


//    if (!m_peerConnection.get()) {
//        ASSERT(peer_id_ == "-1");
//        //        TODO remove when cleaning up
//        std::cout << "< 11" << std::endl;
//        std::cout << peer_id << std::endl;
//        m_peerId = peer_id;
//        //TODO HACK!
//        this->m_isAcceptingConnection = true;

//        if (!m_manager->InitializePeerConnection()) {
//            std::cout << "< 12" << std::endl;
//            LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
//            std::cout << "Failed to initialize our PeerConnection instance" << std::endl;
//            m_manager->m_client.SignOut();
//            return;
//        }
//        std::cout << "< 13" << std::endl;
//    } else if (peer_id != m_peerId) {
//        ASSERT(m_peerId != "-1");
//        LOG(WARNING) << "Received a message from unknown peer while already in a "
//                        "conversation with a different peer.";
//        std::cout << "Received a message from unknown peer while already in a "
//                     "conversation with a different peer." << std::endl;
//        return;
//    }

//    if (m_debug) {
//        std::cout << "< 10" << std::endl;
//    }

//    Json::Reader reader;
//    Json::Value jmessage;
//    //TODO parced QString
//    if (!reader.parse(message, jmessage)) {
//        //        LOG(WARNING) << "Received unknown message. " << message;
//        std::cout << "Received unknown message. " << message << std::endl;
//        return;
//    }
//    std::string type;
//    std::string json_object;

//    rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionTypeName, &type);
//    if (!type.empty()) {
////        TODO clear loopback
////        if (type == "offer-loopback") {
////            // This is a loopback call.
////            // Recreate the peerconnection with DTLS disabled.
////            if (!ReinitializePeerConnectionForLoopback()) {
////                LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
////                std::cout << "Failed to initialize our PeerConnection instance" << std::endl;
////                DeletePeerConnection();
////                m_manager->m_client.SignOut();
////            }
////            return;
////        }

//        std::string sdp;
//        if (!rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionSdpName,
//                                          &sdp)) {
//            LOG(WARNING) << "Can't parse received session description message.";
//            std::cout << "Can't parse received session description message." << std::endl;
//            return;
//        }
//        webrtc::SdpParseError error;
//        webrtc::SessionDescriptionInterface* session_description(
//                    webrtc::CreateSessionDescription(type, sdp, &error));
//        if (!session_description) {
//            LOG(WARNING) << "Can't parse received session description message. "
//                         << "SdpParseError was: " << error.description;
//            std::cout << "Can't parse received session description message. "
//                      << "SdpParseError was: " << error.description << std::endl;
//            return;
//        }
//        //        LOG(INFO) << " Received session description :" << message;
//        std::cout << " Received session description :" << message << std::endl;
//        m_peerConnection->SetRemoteDescription(
//                    DummySetSessionDescriptionObserver::Create(), session_description);
//        if (session_description->type() ==
//                webrtc::SessionDescriptionInterface::kOffer) {
//            std::cout << "m_peerConnection->CreateAnswer(this, NULL);" << std::endl;
//            m_peerConnection->CreateAnswer(this, NULL);
//        }
//        return;
//    } else {
//        std::string sdp_mid;
//        int sdp_mlineindex = 0;
//        std::string sdp;
//        if (!rtc::GetStringFromJsonObject(jmessage, kCandidateSdpMidName,
//                                          &sdp_mid) ||
//                !rtc::GetIntFromJsonObject(jmessage, kCandidateSdpMlineIndexName,
//                                           &sdp_mlineindex) ||
//                !rtc::GetStringFromJsonObject(jmessage, kCandidateSdpName, &sdp)) {
//            LOG(WARNING) << "Can't parse received message.";
//            std::cout << "Can't parse received message." << std::endl;
//            return;
//        }
//        webrtc::SdpParseError error;
//        rtc::scoped_ptr<webrtc::IceCandidateInterface> candidate(
//                    webrtc::CreateIceCandidate(sdp_mid, sdp_mlineindex, sdp, &error));
//        if (!candidate.get()) {
//            LOG(WARNING) << "Can't parse received candidate message. "
//                         << "SdpParseError was: " << error.description;
//            std::cout << "Can't parse received candidate message. "
//                      << "SdpParseError was: " << error.description << std::endl;
//            return;
//        }
//        if (!m_peerConnection->AddIceCandidate(candidate.get())) {
//            LOG(WARNING) << "Failed to apply the received candidate";
//            std::cout << "Failed to apply the received candidate" << std::endl;
//            return;
//        }
//        //        LOG(INFO) << " Received candidate :" << message;
//        std::cout << " Received candidate :" << message << std::endl;
//        return;
//    }
//}

void PeerClientCallback::OnMessageSentSlot(int err) {

}

void PeerClientCallback::OnServerConnectionFailureSlot() {

}

void PeerClientCallback::ConnectToPeerSlot(QString peer_id) {
    std::cout << "PeerClientCallback::ConnectToPeerSlot" << std::endl;
    std::cout << peer_id.toStdString() << std::endl;
    this->ConnectToPeer(peer_id);
}




//
// MainWndCallback implementation.
//

void PeerClientCallback::StartLogin(const std::string& server, int port) {
    if (m_debug) {
        qDebug() << "Conductor::StartLogin";
        qDebug() << QString::fromStdString(server);
        //        qDebug() << port;
    }
    if (m_manager->m_client.is_connected())
        return;
    //    server_ = server;
    qDebug() << "server:";
    qDebug() << QString::fromStdString(server);
    m_manager->m_client.Connect(server, port, GetPeerName());
}

void PeerClientCallback::DisconnectFromServer() {
    if (m_manager->m_client.is_connected())
        m_manager->m_client.SignOut();
}

// TODO move to manager!!!
void PeerClientCallback::ConnectToPeer(QString peer_id) {
    qDebug() << "Conductor::ConnectToPeer";
    ASSERT(m_peerId == "-1");
    ASSERT(peer_id != "-1");

    qDebug() << "> 1";
    //    qDebug() << m_manager->getConductor();
    //    if (m_manager->getConductor()->m_peerConnection.get()) {
    if (m_manager->hasConnections()) {
        std::cout << "Error. We only support connecting to one peer at a time" << std::endl;
        return;
    }
    qDebug() << "> 2";
    m_manager->InitializePeerConnection();
    //    if (m_manager->getConductor()->InitializePeerConnection()) {qDebug() << "> 1";
    //        qDebug() << "> 3";
    m_peerId = peer_id.toStdString();
    //        m_manager->getConductor()->m_peerConnection->CreateOffer(m_manager->getConductor(), NULL);
    //    } else {
    //        qDebug() << "> 4";
    //        std::cout << "Error. Failed to initialize PeerConnection" << std::endl;
    //    }
}

void PeerClientCallback::DisconnectFromCurrentPeer() {
    LOG(INFO) << __FUNCTION__;
    //    if (m_manager->getConductor()->m_peerConnection.get()) {
    if (m_manager->hasConnections()) {
        //        m_manager->m_client.SendHangUp(peer_id_);
        Q_EMIT SendHangUpSignal(m_peerId);
        //        m_manager->getConductor()->DeletePeerConnection();
        m_manager->DeletePeerConnection();
    }

    //    if (main_wnd_->IsWindow())
    //        main_wnd_->SwitchToPeerList(m_manager->m_client.peers());
}

void PeerClientCallback::UIThreadCallback(int msg_id, void* data) {
    if (m_debug) {
        qDebug() << "Conductor::UIThreadCallback";
    }
    switch (msg_id) {
    case Conductor::PEER_CONNECTION_CLOSED:
        LOG(INFO) << "PEER_CONNECTION_CLOSED";
        //        m_manager->getConductor()->DeletePeerConnection();
        m_manager->DeletePeerConnection();

        //TODO restore
        //        ASSERT(m_manager->getConductor()->m_activeStreams.empty());

        //        TODO take a look later
        //        if (main_wnd_->IsWindow()) {
        //            if (m_manager->m_client.is_connected()) {
        //                main_wnd_->SwitchToPeerList(m_manager->m_client.peers());
        //            } else {
        //                main_wnd_->SwitchToConnectUI();
        //            }
        //        } else {
        //            DisconnectFromServer();
        //        }
        break;

    case Conductor::SEND_MESSAGE_TO_PEER: {
        LOG(INFO) << "SEND_MESSAGE_TO_PEER";
        std::cout << "SEND_MESSAGE_TO_PEER" << std::endl;
        std::string* msg = reinterpret_cast<std::string*>(data);
        if (msg) {
            // For convenience, we always run the message through the queue.
            // This way we can be sure that messages are sent to the server
            // in the same order they were signaled without much hassle.
            m_pendingMessages.push_back(msg);
        }

        if (!m_pendingMessages.empty() && !m_manager->m_client.IsSendingMessage()) {
            msg = m_pendingMessages.front();
            m_pendingMessages.pop_front();

            // TODO hangle failed sending
            qDebug() << "Q_EMIT SendToPeerSignal(peer_id_, QString::fromStdString(*msg));";
            qDebug() << QString::fromStdString(m_peerId);
            Q_EMIT SendToPeerSignal(QString::fromStdString(m_peerId), QString::fromStdString(*msg));
            //            if (!m_manager->m_client.SendToPeer(peer_id_, *msg) && peer_id_ != "-1") {
            //                LOG(LS_ERROR) << "SendToPeer failed";
            //                DisconnectFromServer();
            //            }
            delete msg;
        }

        if (!m_manager->hasConnections())
            m_peerId = "-1";

        break;
    }

    case Conductor::NEW_STREAM_ADDED: {
        webrtc::MediaStreamInterface* stream =
                reinterpret_cast<webrtc::MediaStreamInterface*>(
                    data);
        webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
        // Only render the first track.
        if (!tracks.empty()) {
            webrtc::VideoTrackInterface* track = tracks[0];
            //TODO find
            //            main_wnd_->StartRemoteRenderer(track);
        }
        stream->Release();
        break;
    }

    case Conductor::STREAM_REMOVED: {
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

void PeerClientCallback::Close() {
    m_manager->m_client.SignOut();
    m_manager->DeletePeerConnection();
}

// **
// * Getters/setters
// *
void PeerClientCallback::setManager(AlManager* manager) {
    m_manager = manager;
}

void PeerClientCallback::updateFrameCb(const uint8_t* image, int width, int height) {
    std::cout << "PeerClientCallback::updateFrameCb" << std::endl;
//    m_img.loadFromData(image, width*height*4, QImage::Format_ARGB32);
    QImage img(image, width, height, QImage::Format_ARGB32);
    m_img = img;
//    img.save("out_123.png", "PNG");
}
