#include "peerclientcallback.h"
#include <iostream>

PeerClientCallback::PeerClientCallback(QApplication* app, QObject *parent) :
    QObject(parent),
    m_debug(true),
    m_peerId("-1")
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

}

void PeerClientCallback::OnMessageSentSlot(int err) {

}

void PeerClientCallback::OnServerConnectionFailureSlot() {

}

void PeerClientCallback::ConnectToPeerSlot(QString peer_id) {
    std::cout << "PeerClientCallback::ConnectToPeerSlot" << std::endl;
    std::cout << peer_id.toStdString() << std::endl;
    this->ConnectToPeer(peer_id);
}

void PeerClientCallback::DequeueMessagesFromPeerSlot() {

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
    if (m_manager->getConductor()->m_peerConnection.get()) {
        std::cout << "Error. We only support connecting to one peer at a time" << std::endl;
        return;
    }
    qDebug() << "> 2";
    if (m_manager->getConductor()->InitializePeerConnection()) {qDebug() << "> 1";
        qDebug() << "> 3";
        m_peerId = peer_id.toStdString();
        m_manager->getConductor()->m_peerConnection->CreateOffer(m_manager->getConductor(), NULL);
    } else {
        qDebug() << "> 4";
        std::cout << "Error. Failed to initialize PeerConnection" << std::endl;
    }
}

void PeerClientCallback::DisconnectFromCurrentPeer() {
    LOG(INFO) << __FUNCTION__;
    if (m_manager->getConductor()->m_peerConnection.get()) {
        //        m_manager->m_client.SendHangUp(peer_id_);
        Q_EMIT SendHangUpSignal(m_peerId);
        m_manager->getConductor()->DeletePeerConnection();
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
        m_manager->getConductor()->DeletePeerConnection();

        ASSERT(m_manager->getConductor()->m_activeStreams.empty());

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

        if (!m_manager->getConductor()->m_peerConnection.get())
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
    m_manager->getConductor()->DeletePeerConnection();
}

// **
// * Getters/setters
// *
void PeerClientCallback::setManager(AlManager* manager) {
    m_manager = manager;
}
