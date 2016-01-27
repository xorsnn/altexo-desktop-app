#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include "alwsclient.h"

QT_USE_NAMESPACE

AlWsClient::AlWsClient(bool debug, QObject *parent) :
    QObject(parent),
    m_debug(debug)
{
    QSettings settings;

    connect(&m_webSocket, &QWebSocket::connected, this, &AlWsClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &AlWsClient::closed);
    QString wsLink = settings.value("altexo/wsLink", "ws://altexo.com:8889/ws").toString();
    if (m_debug)
        qDebug() << "WebSocket server:" << wsLink;
    m_webSocket.open(QUrl(wsLink));
}

void AlWsClient::onConnected()
{
    if (m_debug)
        qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &AlWsClient::onTextMessageReceived);
}

void AlWsClient::onTextMessageReceived(QString message)
{
    if (m_debug)
        qDebug() << "Message received:" << message;

    Q_EMIT this->onTextMessageReceivedSignal(message);
}

void AlWsClient::sendTextMessageSlot(QString message)
{
    if (m_debug)
        qDebug() << "sending message";

    this->m_webSocket.sendTextMessage(message);
}

void AlWsClient::closeSlot()
{
    this->m_webSocket.close();
}

void AlWsClient::sendSdpSlot(QString message)
{
    QJsonObject obj;
    obj["type"] = "SDP";
    obj["body"] = message;
    QJsonDocument doc(obj);

    this->sendTextMessageSlot(doc.toJson());
}

void AlWsClient::sendIceCandidatesSlot(QString msg)
{
    qDebug() << "QQQQQQQQQQQQQQQQQQQQQQQQQQ~~~~~~~~~~~~~~";
    QJsonObject obj;
    obj["type"] = "ICE";
    obj["body"] = msg;
    QJsonDocument doc(obj);
    this->sendTextMessageSlot(doc.toJson());
}
