#include "peerclientcallback.h"
#include <iostream>
#include <QTimer>
#include <QJsonDocument>
#include <QTextCodec>

std::string GetPeerName() {
//  char computer_name[256];
//  std::string ret(GetEnvVarOrDefault("USERNAME", "user"));
//  ret += '@';
//  if (gethostname(computer_name, arraysize(computer_name)) == 0) {
//    ret += computer_name;
//  } else {
//    ret += "host";
//  }
//  return ret;
    return "test_host";
}

PeerClientCallback::PeerClientCallback(QApplication* app, QObject *parent) :
    QObject(parent),
    m_debug(true),
    m_peerId("-1"),
    m_managerInterface(NULL),
    m_processingMsg(false)
{
    std::cout << "PeerClientCallback::PeerClientCallback" << std::endl;
    m_app = app;
    m_videoDeviceName = "";

    m_remoteSDP = "";
    m_localSDP = "";

    m_candidates = 0;
    m_remoteCandidates = 0;
    m_remoteCandidatesProcessed = 0;
}

void PeerClientCallback::timeoutSlot() {
    qDebug() << "PeerClientCallback::timeoutSlot";
    QImage tmpImg;
    tmpImg.load("/home/xors/cto_intro.png");
    int numBytes = tmpImg.width()*tmpImg.height()*4;
    uint8_t *pData = new uint8_t[numBytes];

    memcpy(pData, tmpImg.convertToFormat(QImage::Format_RGBA8888).bits(), numBytes);
    m_managerInterface->setImageData(pData, numBytes, tmpImg.width(), tmpImg.height());
//    m_managerInterface->setImageData();
}

void PeerClientCallback::sendToPeerCb(const std::string &msg) {

    qDebug() << "1.1";
    qDebug() << QString::fromStdString(msg);

    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(msg).toUtf8());
    QJsonObject jsonObj = doc.object();
    if (jsonObj.contains("sdp")) {
        m_localSDP = QString::fromStdString(msg);
    } else {
        m_candidates++;
        qDebug() << "CANDIDATES:";
        qDebug() << m_candidates;
    }

    Q_EMIT SendToPeerSignal(QString::fromStdString(m_peerId), QString::fromStdString(msg));

    if (!this->m_processingMsg) {
        if (m_remoteSDP != "" && m_localSDP != "") {
            DequeueMessagesFromPeerSlot();
        }
    }
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

void PeerClientCallback::queueUIThreadCallbackCb(int msg_id, void* data) {
//    UIThreadCallback(msg_id, data);
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
        if (m_videoDeviceName != "") {
            m_managerInterface->setDesiredDataSource(AlManagerInterface::CAMERA);
        } else {
            // **
            // * INITIATE MODE
            // *
            QJsonObject obj;
            obj["mode"] = "hologram";
            QJsonDocument doc(obj);
            // (1015 is UTF-16, 1014 UTF-16LE, 1013 UTF-16BE, 106 UTF-8)
            Q_EMIT SendToPeerSignal(QString::fromStdString(m_peerId), QTextCodec::codecForMib(106)->toUnicode(doc.toJson()));

            Q_EMIT requestNewFrameSignal();
//            QTimer *timer = new QTimer(this);
//            connect(timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
//            timer->start(1000);
        }
    }

    QMap<QString, QString> curMsg;
    // defining type
    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(message).toUtf8());
    QJsonObject jsonObj = doc.object();
    qDebug() << jsonObj.contains("sdp");

    if (jsonObj.contains("sdp")) {
        m_managerInterface->OnMessageFromPeer(peer_id, message);
        m_remoteSDP = QString::fromStdString(message);
    } else {
        m_remoteCandidates++;
        qDebug() << "REMOTE CANDIDATES: ";
        qDebug() << m_remoteCandidates;
        curMsg[QString::fromStdString(peer_id)] = QString::fromStdString(message);
        m_messageQueue.enqueue(curMsg);
    }
    if (!this->m_processingMsg) {
        if (m_remoteSDP != "" && m_localSDP != "") {
            DequeueMessagesFromPeerSlot();
        }
    }
}

void PeerClientCallback::DequeueMessagesFromPeerSlot() {
    this->m_processingMsg = true;
    while (!this->m_messageQueue.isEmpty()) {
        QMap<QString, QString> curMsg;
        curMsg = this->m_messageQueue.dequeue();
        QMapIterator<QString, QString> i(curMsg);
        while (i.hasNext()) {
            i.next();
            m_remoteCandidatesProcessed++;
            qDebug() << "REMOTE CANDIDATES PROCESSED: ";
            qDebug() << m_remoteCandidatesProcessed;
            m_managerInterface->OnMessageFromPeer(i.key().toStdString(), i.value().toStdString());
        }
    }
    this->m_processingMsg = false;
}

void PeerClientCallback::OnMessageSentSlot(int err) {

}

void PeerClientCallback::OnServerConnectionFailureSlot() {

}

void PeerClientCallback::ConnectToPeerSlot(QString peer_id) {
    if (m_debug) {
        std::cout << "PeerClientCallback::ConnectToPeerSlot" << std::endl;
    }
    this->ConnectToPeer(peer_id);
}

//
// MainWndCallback implementation.
//

void PeerClientCallback::StartLogin(const std::string& server, int port) {
    if (m_debug) {
        qDebug() << "Conductor::StartLogin";
        qDebug() << QString::fromStdString(server);
    }
    if (m_managerInterface->isClientConnected())
        return;
    //    server_ = server;
    qDebug() << "server:";
    qDebug() << QString::fromStdString(server);
    m_managerInterface->clientConnect(server, port, GetPeerName());
}

void PeerClientCallback::DisconnectFromServer() {
    if (m_managerInterface->isClientConnected())
        m_managerInterface->clientSignOut();
}

// TODO move to manager!!!
void PeerClientCallback::ConnectToPeer(QString peer_id) {
    if (m_videoDeviceName != "") {
        m_managerInterface->setDesiredDataSource(AlManagerInterface::CAMERA);
    } else {
        // **
        // * INITIATE MODE
        // *
        QJsonObject obj;
        obj["mode"] = "hologram";
        QJsonDocument doc(obj);
        // (1015 is UTF-16, 1014 UTF-16LE, 1013 UTF-16BE, 106 UTF-8)
        Q_EMIT SendToPeerSignal(peer_id, QTextCodec::codecForMib(106)->toUnicode(doc.toJson()));

        Q_EMIT requestNewFrameSignal();
//        QTimer *timer = new QTimer(this);
//        connect(timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
//        timer->start(1000);
    }
    qDebug() << "Conductor::ConnectToPeer";
//    ASSERT(m_peerId == "-1");
//    ASSERT(peer_id != "-1");

    if (m_managerInterface->hasConnections()) {
        std::cout << "Error. We only support connecting to one peer at a time" << std::endl;
        return;
    }
    m_managerInterface->InitializePeerConnection();
    m_peerId = peer_id.toStdString();
}

void PeerClientCallback::DisconnectFromCurrentPeer() {
    if (m_managerInterface->hasConnections()) {
        //        m_managerInterface->m_client.SendHangUp(peer_id_);
        Q_EMIT SendHangUpSignal(m_peerId);
        m_managerInterface->DeletePeerConnection();
    }
}

void PeerClientCallback::Close() {
//    TODO restore this
    m_managerInterface->clientSignOut();
    m_managerInterface->DeletePeerConnection();
}

// **
// * Getters/setters
// *
void PeerClientCallback::setManager(AlManagerInterface* manager) {
    m_managerInterface = manager;
}

void PeerClientCallback::updateFrameCb(const uint8_t* image, int width, int height) {
//    std::cout << "PeerClientCallback::updateFrameCb" << std::endl;
    QImage img(image, width, height, QImage::Format_ARGB32);
    m_img = img;
    Q_EMIT newFrameSignal(&m_img);
}

void PeerClientCallback::onDevicesListChangedCb(std::vector<std::string> device_names) {
//    qDebug() << "PeerClientCallback::onDevicesListChangedCb";
    Q_EMIT onDevicesListChangedSignal(device_names);
}

void PeerClientCallback::selectVideoDeviceSlot(QString deviceName) {
    qDebug() << deviceName;
    m_videoDeviceName = deviceName.toStdString();
}

std::string PeerClientCallback::getVideoDeviceName() {
    return m_videoDeviceName;
}

//NEW
void PeerClientCallback::requestNewFrameSlot() {
    Q_EMIT requestNewFrameSignal();
}

void PeerClientCallback::newFrameSlot(QImage* img) {
    // TODO: make quality configurable
    QImage newImgTmp = img->scaled(640, 240);
    QImage * newImg = &newImgTmp;
    int numBytes = newImg->width()*newImg->height()*4;
    uint8_t *pData = new uint8_t[numBytes];

    memcpy(pData, newImg->convertToFormat(QImage::Format_RGBA8888).bits(), numBytes);
    m_managerInterface->setImageData(pData, numBytes, newImg->width(), newImg->height());

    QTimer::singleShot(1000/40, this, SLOT(requestNewFrameSlot()));
}
