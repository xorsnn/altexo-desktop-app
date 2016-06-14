#include "alconnclient.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonArray>
#include <QTextCodec>

QString SERVER_LINK = "https://dev.lugati.ru";


AlConnClient::AlConnClient(QObject *parent) : QObject(parent), m_debug(true)
{
    QObject::connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnectedToServerSlot()));
    QObject::connect(&m_webSocket, SIGNAL(disconnected()), this, SLOT(onDisconnectedFromServerSlot()));
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &AlConnClient::onMessageSlot);

    m_qnam = new QNetworkAccessManager(this);
    QObject::connect(m_qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleNetworkDataSlot(QNetworkReply*)));
    QObject::connect(m_qnam,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(handleSSLErrorsSlot(QNetworkReply*)));

    // **
    // * LOGIN
    // *
    QJsonObject obj;
//    obj["username"] = "xors_desktop";
//    obj["password"] = "eleimt";
    obj["username"] = "Galya";
    obj["password"] = "ujnm567";
    QString host = SERVER_LINK + "/users/auth/login/";
    this->sendHttpRequestSlot(host, obj, HTTP_POST);

    this->m_receivedOffer = false;

    m_sessionToken = "";
    m_wsLink = "";
}

void AlConnClient::connectToServerSlot() {
    if (m_wsLink != "" && m_sessionToken != "") {
        QString wsLink = m_wsLink;
        qDebug() << wsLink;
        this->m_webSocket.open(QUrl(m_wsLink));
    } else {
        qDebug() << "ERROR, no ws link!";
    }
}

void AlConnClient::handleNetworkDataSlot(QNetworkReply* reply) {
    qDebug() << "AlConnClient::handleNetworkDataSlot";
    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();
    if (jsonObj.contains("auth_token")) {
        m_sessionToken = jsonObj["auth_token"].toString();
        // right after login get profile info
        QJsonObject obj;
        QString host = SERVER_LINK + "/users/auth/me/";
        this->sendHttpRequestSlot(host, obj, HTTP_GET);
    }
    // by email and username we define 'me' call
    else if (jsonObj.contains("email") && jsonObj.contains("username")) {
        QJsonObject obj;
        this->sendHttpRequestSlot(SERVER_LINK + "/chat/api/join/" , obj, HTTP_POST);
    }
    // consider it is initiation parameters
    else if (jsonObj.contains("params")) {
        m_wsLink = jsonObj["params"].toObject()["wss_url"].toString();
        connectToServerSlot();
    }
    else {
        qDebug() << "unhandled response type";
        qDebug() << strReply;
    }
}

void AlConnClient::handleSSLErrorsSlot(QNetworkReply* reply) {
    qDebug() << "AlConnClient::handleSSLErrorsSlot";
}

void AlConnClient::onConnectedToServerSlot() {
    if (m_debug) {
        qDebug() << "AlConnClient::onConnectedToServerSlot";
    }
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
    qDebug() << action;
    if (action == "logged_in") {
        this->m_connId = jsonObj["data"].toObject()["id"].toString();
        Q_EMIT OnSignedInSignal();
    } else if (action == "peer_connected") {
        this->m_peers[jsonObj["id"].toString()] = jsonObj["name"].toString();
        Q_EMIT OnPeerConnectedSignal(jsonObj["id"].toString(), jsonObj["name"].toString());
    } else if (action == "message_from_peer") {
        QString tmp_msg = jsonObj["data"].toObject()["message"].toString();
        QJsonDocument tmp_doc = QJsonDocument::fromJson(tmp_msg.toUtf8());
        QJsonObject tmp_jsonObj = tmp_doc.object();
        if (tmp_jsonObj.contains("callAccepted")) {
            Q_EMIT ConnectToPeerSignal(jsonObj["data"].toObject()["sender_id"].toString());
        } else {
            Q_EMIT OnMessageFromPeerSignal(jsonObj["data"].toObject()["sender_id"].toString().toStdString(),
                    jsonObj["data"].toObject()["message"].toString().toStdString());
        }

    } else if (action == "update_user_list") {
        QJsonArray contactsOnline = jsonObj["data"].toArray();
        for (int i = 0; i < contactsOnline.size(); i++) {
            if (contactsOnline[i].toObject()["id"].toString() != m_connId) {
                this->m_peers[contactsOnline[i].toObject()["id"].toString()] = contactsOnline[i].toObject()["name"].toString();
            }
        }
        Q_EMIT OnPeerConnectedSignal(jsonObj["id"].toString(), jsonObj["name"].toString());
    } else {
        //        Q_EMIT this->onTextMessageReceivedSignal(message);
    }
}

void AlConnClient::SendToPeerSlot(QString peer_id, const QString &message) {
    if (m_debug) {
        qDebug() << "AlConnClient::SendToPeerSlot";
        qDebug() << message;
        qDebug() << "peer:";
        qDebug() << peer_id;
    }
    QJsonObject obj;
    obj["action"] = "send_to_peer";
    QJsonObject msgData;
    msgData["peer_id"] = peer_id;
    msgData["message"] = message;
    obj["data"] = msgData;
    //TODO check for android
    QJsonDocument doc(obj);
    this->sendMessageSlot(doc.toJson());
}

void AlConnClient::SendHangUpSlot(QString peer_id_) {
    if (m_debug) {
        qDebug() << "AlConnClient::SendHangUpSlot";
    }
}

void AlConnClient::ConnectToPeerSlot(QString peerId) {
    if (m_debug) {
        qDebug() << "AlConnClient::ConnectToPeerSlot";
    }
    // TODO: is set only when we are calling
//    m_peerId = peerId;

    QJsonObject objCall;
    objCall["call"] = "true";
    QJsonDocument docCall(objCall);
    // (1015 is UTF-16, 1014 UTF-16LE, 1013 UTF-16BE, 106 UTF-8)
    SendToPeerSlot(peerId, QTextCodec::codecForMib(106)->toUnicode(docCall.toJson()));
//    Q_EMIT ConnectToPeerSignal(peerId);
}

void AlConnClient::sendHttpRequestSlot(QString host, QJsonObject data, int method) {
    qDebug() << "<++++";
    qDebug() << host;
    qDebug() << ">++++";
    // Build your JSON string as usual
    QJsonDocument doc(data);
    QByteArray jsonString = doc.toJson();
    // For your "Content-Length" header
    QByteArray postDataSize = QByteArray::number(jsonString.size());

    // Time for building your request
    QUrl serviceURL(host);
    QNetworkRequest request(serviceURL);

    // Add the headers specifying their names and their values with the following method : void QNetworkRequest::setRawHeader(const QByteArray & headerName, const QByteArray & headerValue);
    request.setRawHeader("User-Agent", "Altexo v0.1");
    request.setRawHeader("X-Custom-User-Agent", "Altexo v0.1");
    request.setRawHeader("Content-Type", "application/json");

    if (m_sessionToken != "") {
        QString tokenHeader = "Token " + m_sessionToken;
        qDebug() << "TOKEN:" + tokenHeader;
        request.setRawHeader("Authorization", tokenHeader.toUtf8());
    }

    // Use QNetworkReply * QNetworkAccessManager::post(const QNetworkRequest & request, const QByteArray & data); to send your request. Qt will rearrange everything correctly.
    switch ( method ) {
    case HTTP_GET: {
        m_qnam->get(request);
        break;
    }
    case HTTP_POST: {
        request.setRawHeader("Content-Length", postDataSize);
        m_qnam->post(request, jsonString);
        break;
    }
    case HTTP_PUT: {
        break;
    }
    case HTTP_DELETE: {
        break;
    }
    }
}
