#ifndef ALCONNCLIENT_H
#define ALCONNCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>

class AlConnClient : public QObject
{
    Q_OBJECT
public:
    explicit AlConnClient(QObject *parent = 0);

Q_SIGNALS:
    void OnSignedInSignal();
    void OnDisconnectedSignal();
    void OnPeerConnectedSignal(QString id, QString name);
    void OnPeerDisconnectedSignal(int id);
    void OnMessageFromPeerSignal(std::string peer_id, std::string message);
    void OnMessageSentSignal(int err);
    void OnServerConnectionFailureSignal();

public Q_SLOTS:
    void connectToServerSlot();
    void onConnectedToServerSlot();
    void onDisconnectedFromServerSlot();

    void sendMessageSlot(QString message);
    void onMessageSlot(const QString &message);

    void SendToPeerSlot(QString peer_id, const QString &message);
    void SendHangUpSlot(QString peer_id_);

    QMap<QString, QString> getPeers() {
        return this->m_peers;
    }

private:
    QWebSocket m_webSocket;
    bool m_debug;
    QString m_connId;
    QMap<QString, QString> m_peers;

    //TODO kind of a hack
    bool m_receivedOffer;
};

#endif // ALCONNCLIENT_H
