#include "alconnclient.h"
#include <QJsonObject>
#include <QJsonDocument>

AlConnClient::AlConnClient(QObject *parent) : QObject(parent), m_debug(true)
{
    QObject::connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnectedToServerSlot()));
    QObject::connect(&m_webSocket, SIGNAL(disconnected()), this, SLOT(onDisconnectedFromServerSlot()));
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &AlConnClient::onMessageSlot);

    this->connectToServerSlot();
    this->m_receivedOffer = false;
}

void AlConnClient::connectToServerSlot() {
    //    if (address == "") {
    //        QSettings settings;
    //        QString wsLink = settings.value("altexo/wsLink", "ws://altexo.com:8889/ws").toString();
    QString wsLink = "ws://127.0.0.1:8888/";
    if (m_debug) {
        qDebug() << "WebSocket server:" << wsLink;
    }
    this->m_webSocket.open(QUrl(wsLink));
    //    } else {
    //        qDebug() << address;
    //        m_webSocket.open(QUrl(address));
    //    }
}

void AlConnClient::onConnectedToServerSlot() {
    if (m_debug) {
        qDebug() << "AlConnClient::onConnectedToServerSlot";
    }


    // **
    // * LOGIN
    // *

    QJsonObject obj;
    obj["action"] = "login";
    QJsonObject msgData;
    msgData["token"] = "2";
    obj["data"] = msgData;
    QJsonDocument doc(obj);
    this->sendMessageSlot(doc.toJson());
}

void AlConnClient::onDisconnectedFromServerSlot() {
    if (m_debug) {
        qDebug() << "AlConnClient::onDisconnectedFromServerSlot";
    }
}

void AlConnClient::sendMessageSlot(QString message) {
    if (m_debug) {
        qDebug() << "AlConnClient::sendMessageSlot";
    }
    this->m_webSocket.sendTextMessage(message);
}

void AlConnClient::onMessageSlot(const QString &message) {
    if (m_debug) {
        qDebug() << "AlConnClient::onMessageSlot";
//        qDebug() << message;
    }
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();
    QString action = jsonObj["action"].toString();

    if (action == "logged_in") {
        this->m_connId = jsonObj["data"].toObject()["id"].toString();
        Q_EMIT OnSignedInSignal();
        //CONNECT TO TEST
//        QJsonObject obj;
//        obj["action"] = "connectTo";
//        QJsonObject msgData;
//        msgData["peer_id"] = "123";
//        obj["data"] = msgData;
//        QJsonDocument doc(obj);
//        this->sendTextMessageSlot(doc.toJson());
    } else if (action == "connected") {
        qDebug() << "connected!!!!";
    } else if (action == "peer_connected") {
        this->m_peers[jsonObj["id"].toString()] = jsonObj["name"].toString();
        Q_EMIT OnPeerConnectedSignal(jsonObj["id"].toString(), jsonObj["name"].toString());
    } else if (action == "message_from_peer") {
//        qDebug() << "======================================================!!!!!!!!!!";
//        qDebug() << jsonObj["data"].toObject()["peer_id"].toString();
//        qDebug() << jsonObj["data"].toObject()["message"].toString();
        QString tmp_msg = jsonObj["data"].toObject()["message"].toString();

        QJsonDocument tmp_doc = QJsonDocument::fromJson(tmp_msg.toUtf8());
        QJsonObject tmp_jsonObj = tmp_doc.object();

        // TODO HACK!!!!!!!!!!!!!!!
//        QString type = tmp_jsonObj["type"].toString();
//        if ((type == "offer" || type == "answer") && (this->m_receivedOffer == true)) {

//        } else if (type == "offer") {
//            this->m_receivedOffer = true;
//            Q_EMIT OnMessageFromPeerSignal(jsonObj["data"].toObject()["peer_id"].toString(),
//                    jsonObj["data"].toObject()["message"].toString());
//        } else {
            Q_EMIT OnMessageFromPeerSignal(jsonObj["data"].toObject()["sender_id"].toString().toStdString(),
                    jsonObj["data"].toObject()["message"].toString().toStdString());
//        }

//        }

    } else {
//        Q_EMIT this->onTextMessageReceivedSignal(message);
    }
}

void AlConnClient::SendToPeerSlot(QString peer_id, const QString &message) {
    if (m_debug) {
        qDebug() << "AlConnClient::SendToPeerSlot";
        qDebug() << message;
    }
    QJsonObject obj;
    obj["action"] = "send_to_peer";
    QJsonObject msgData;
    msgData["peer_id"] = peer_id;
    msgData["message"] = message;
    obj["data"] = msgData;
    QJsonDocument doc(obj);
    this->sendMessageSlot(doc.toJson());
}

void AlConnClient::SendHangUpSlot(QString peer_id_) {
    if (m_debug) {
        qDebug() << "AlConnClient::SendHangUpSlot";
    }
}

