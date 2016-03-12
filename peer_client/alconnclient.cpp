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
    //

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
        qDebug() << message;
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
    } else {
//        Q_EMIT this->onTextMessageReceivedSignal(message);
    }
}

void AlConnClient::SendToPeerSlot(QString peer_id, const QString &message) {
    if (m_debug) {
        qDebug() << "AlConnClient::SendToPeerSlot";
        qDebug() << message;
    }
}

void AlConnClient::SendHangUpSlot(QString peer_id_) {
    if (m_debug) {
        qDebug() << "AlConnClient::SendHangUpSlot";
    }
}

