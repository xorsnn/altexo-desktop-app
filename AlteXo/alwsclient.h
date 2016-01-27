#ifndef ALWSCLIENT_H
#define ALWSCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>

class AlWsClient : public QObject
{
    Q_OBJECT
public:
    explicit AlWsClient(bool debug = false, QObject *parent = Q_NULLPTR);

Q_SIGNALS:
    void closed();
    void onTextMessageReceivedSignal(QString message);

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

public Q_SLOTS:
    void sendTextMessageSlot(QString message);
    void closeSlot();

    void sendSdpSlot(QString message);
    void sendIceCandidatesSlot(QString msg);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;

};

#endif // ALWSCLIENT_H
