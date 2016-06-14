#ifndef ALCONNCLIENT_H
#define ALCONNCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QNetworkAccessManager>
#include <QJsonObject>


class AlConnClient : public QObject
{
    Q_OBJECT
public:
    explicit AlConnClient(QObject *parent = 0);

    enum HTTP_METHODS {
        HTTP_GET = 1,
        HTTP_POST,
        HTTP_PUT,
        HTTP_DELETE,
    };

Q_SIGNALS:
    void OnSignedInSignal();
    void OnDisconnectedSignal();
    void OnPeerConnectedSignal(QString id, QString name);
    void OnPeerDisconnectedSignal(int id);
    void OnMessageFromPeerSignal(std::string peer_id, std::string message);
    void OnMessageSentSignal(int err);
    void OnServerConnectionFailureSignal();

    void ConnectToPeerSignal(QString peerId);

public Q_SLOTS:
    void connectToServerSlot();
    void onConnectedToServerSlot();
    void onDisconnectedFromServerSlot();

    void sendMessageSlot(QString message);
    void onMessageSlot(const QString &message);

    void SendToPeerSlot(QString peer_id, const QString &message);
    void SendHangUpSlot(QString peer_id_);

    // HTTP
    void handleNetworkDataSlot(QNetworkReply* reply);
    void handleSSLErrorsSlot(QNetworkReply* reply);

    void sendHttpRequestSlot(QString host, QJsonObject data, int method);


    QMap<QString, QString> getPeers() {
        return this->m_peers;
    }

    void ConnectToPeerSlot(QString peerId);

private:
    QWebSocket m_webSocket;
    bool m_debug;
    QString m_connId;
    QMap<QString, QString> m_peers;


    QNetworkAccessManager* m_qnam;
    //TODO kind of a hack
    bool m_receivedOffer;

    QString m_sessionToken;
    QString m_wsLink;
//    QString m_peerId;
};

#endif // ALCONNCLIENT_H
