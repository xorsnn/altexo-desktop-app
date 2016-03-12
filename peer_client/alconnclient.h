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
    void OnPeerConnectedSignal(int id, QString name);
    void OnPeerDisconnectedSignal(int id);
    void OnMessageFromPeerSignal(int peer_id, QString message);
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

private:
    QWebSocket m_webSocket;
    bool m_debug;
    QString m_connId;
};

#endif // ALCONNCLIENT_H
