#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include "alwsclient.h"

QT_USE_NAMESPACE

AlWsClient::AlWsClient(bool debug, QObject *parent) :
    QObject(parent),
    m_debug(debug)
{
    this->m_debug = true;
    this->one2one = true;
    // "-1" means not connected yet
    this->connId = "-1";
}

void AlWsClient::init_(QString address) {

    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    QObject::connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
//    QObject::connect(&m_webSocket, SIGNAL(disconnected()), this, SLOT(closed()));

    if (address == "") {
        QSettings settings;
        QString wsLink = settings.value("altexo/wsLink", "ws://altexo.com:8889/ws").toString();
        if (m_debug)
            qDebug() << "WebSocket server:" << wsLink;
        m_webSocket.open(QUrl(wsLink));
    } else {
        qDebug() << address;
        m_webSocket.open(QUrl(address));
    }
}

void AlWsClient::onConnected()
{
    if (m_debug) {
        qDebug() << "WebSocket connected";
    }

    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &AlWsClient::onTextMessageReceived);



    // one2one streaming
    if (this->one2one) {
        // login
        QJsonObject obj;
        obj["action"] = "login";
        QJsonObject msgData;
        msgData["token"] = "2";
        obj["data"] = msgData;

//        QSettings settings;
//        QString room = settings.value("altexo/alRoom", "altexo-chat").toString();
//        QJsonObject obj;
//        obj["room"] = room;
//        obj["msg_to"] = "server";
        QJsonDocument doc(obj);
        this->sendTextMessageSlot(doc.toJson());
    }
}

void AlWsClient::onTextMessageReceived(QString message)
{
    if (m_debug)
        qDebug() << "Message received:" << message;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();
//    TODO add case instead of if
    if (jsonObj["action"].toString() == "logged_in") {
        this->connId = jsonObj["data"].toObject()["id"].toString();
        //CONNECT TO TEST
        QJsonObject obj;
        obj["action"] = "connectTo";
        QJsonObject msgData;
        msgData["peer_id"] = "123";
        obj["data"] = msgData;
        QJsonDocument doc(obj);
        this->sendTextMessageSlot(doc.toJson());
    } else if (jsonObj["action"].toString() == "connected") {
        qDebug() << "connected!!!!";
    } else {
        Q_EMIT this->onTextMessageReceivedSignal(message);
    }

}

void AlWsClient::sendTextMessageSlot(QString message)
{
    if (m_debug) {
        qDebug() << "sending message:";
        qDebug() << message;
        qDebug() << "===================";
    }

    this->m_webSocket.sendTextMessage(message);
}

void AlWsClient::closeSlot()
{
    this->m_webSocket.close();
}

void AlWsClient::sendSdpSlot(QString message)
{
    QJsonObject obj;

    if (this->one2one) {
        QSettings settings;
        QString room = settings.value("altexo/alRoom", "altexo-chat").toString();
        obj["room"] = room;
        obj["type"] = "SDP";
        obj["body"] = message;
    } else {
        obj["id"] = "presenter";
        obj["sdpOffer"] = message;
    }

    QJsonDocument doc(obj);
    this->sendTextMessageSlot(doc.toJson());
//    this->sendTextMessageSlot(message);
}

void AlWsClient::sendIceCandidatesSlot(QString msg)
{
    qDebug() << "2 send";
    qDebug() << msg;
    QSettings settings;
    QString room = settings.value("altexo/alRoom", "altexo-chat").toString();
    QJsonObject obj;
    obj["room"] = room;
    obj["type"] = "ICE";
    obj["body"] = msg;
    QJsonDocument doc(obj);
//    TODO one to many
    if (this->one2one) {
        this->sendTextMessageSlot(doc.toJson());
    } else {
        this->sendTextMessageSlot(msg);
    }
}
