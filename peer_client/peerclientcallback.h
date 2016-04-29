#ifndef PEERCLIENTCALLBACK_H
#define PEERCLIENTCALLBACK_H

#include "alcallback.h"
#include <QApplication>
#include "almanager.h"
#include "mainwindow.h"
#include <QQueue>

class PeerClientCallback : public QObject,
        public AlCallback,
        public MainWndCallback
{
    Q_OBJECT
public:
    PeerClientCallback(QApplication* app, QObject *parent = 0);
    virtual void sendToPeerCb(std::string peer_id, const std::string &message);
    virtual void sendHangUpCb(std::string peer_id);
    virtual void dequeueMessagesFromPeerCb();
    virtual void stopLocalRendererCb();
    virtual void stopRemoteRendererCb();
    virtual void ensureStreamingUICb();
    virtual void startRemoteRendererCb(webrtc::VideoTrackInterface*);
    virtual void startLocalRendererCb(webrtc::VideoTrackInterface*);
    virtual void queueUIThreadCallbackCb(int msg_id, void* data);
    virtual void switchToPeerListCb(const Peers& peers);
    virtual void onDisconnectedCb();
    virtual void processUiEventsCb();
    virtual void SwitchToStreamingUI(); // ????

    // **
    // * MainWndCallback implementation
    // *
    virtual void StartLogin(const std::string& server, int port); // +
    virtual void DisconnectFromServer(); // +
    virtual void ConnectToPeer(QString peer_id); // +
    virtual void DisconnectFromCurrentPeer(); // +
    virtual void UIThreadCallback(int msg_id, void* data); // +
    virtual void Close();

    // **
    // * Getters/setters
    // *
    void setManager(AlManager*manager);


//    void OnMessageFromPeer(std::string peer_id, const std::string& message);
public Q_SLOTS:
    void OnSignedInSlot();
    void OnDisconnectedSlot();
    void OnPeerDisconnectedSlot(std::string id);
    void OnMessageFromPeerSlot(std::string peer_id, const std::string& message);
    void OnMessageSentSlot(int err);
    void OnServerConnectionFailureSlot();

    void ConnectToPeerSlot(QString peer_id);
    void DequeueMessagesFromPeerSlot();

Q_SIGNALS:
    void SendToPeerSignal(QString peer_id, const QString &message);
    void SendHangUpSignal(std::string peer_id);
    void DequeueMessagesFromPeerSignal();

private:
    QApplication* m_app;
    bool m_debug;
    AlManager* m_manager;
    std::string m_peerId;
    std::deque<std::string*> m_pendingMessages;
    QQueue<QMap<QString, QString>> m_messageQueue;
    bool m_processingMsg;
};

#endif // PEERCLIENTCALLBACK_H
