#ifndef PEERCLIENTCALLBACK_H
#define PEERCLIENTCALLBACK_H

#include "alcallback.h"
#include <QApplication>
#include "almanagerinterface.h"
#include "mainwindow.h"
#include <QQueue>
#include <deque>

class PeerClientCallback : public QObject,
        public AlCallback,
        public MainWndCallback
{
    Q_OBJECT
public:
    PeerClientCallback(QApplication* app, QObject *parent = 0);
    virtual void sendToPeerCb(const std::string &message);
    virtual void sendHangUpCb(std::string peer_id);
    virtual void dequeueMessagesFromPeerCb();
    virtual void stopLocalRendererCb();
    virtual void stopRemoteRendererCb();
    virtual void ensureStreamingUICb();
    virtual void queueUIThreadCallbackCb(int msg_id, void* data);
    virtual void switchToPeerListCb(const Peers& peers);
    virtual void onDisconnectedCb();
    virtual void processUiEventsCb();
    virtual void SwitchToStreamingUI(); // ????
    virtual void onDevicesListChangedCb(std::vector<std::string> device_names);

    virtual void updateFrameCb(const uint8_t* image, int width, int height);

    virtual std::string getVideoDeviceName();

    // **
    // * MainWndCallback implementation
    // *
    virtual void StartLogin(const std::string& server, int port); // +
    virtual void DisconnectFromServer(); // +
    virtual void ConnectToPeer(QString peer_id); // +
    virtual void DisconnectFromCurrentPeer(); // +
//    virtual void UIThreadCallback(int msg_id, void* data); // +
    virtual void Close();

    // **
    // * Getters/setters
    // *
    void setManager(AlManagerInterface* manager);


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

    void selectVideoDeviceSlot(QString deviceName);
    void timeoutSlot();

    void newFrameSlot(QImage* img);
    void requestNewFrameSlot();

Q_SIGNALS:
    void SendToPeerSignal(QString peer_id, const QString &message);
    void SendHangUpSignal(std::string peer_id);
    void DequeueMessagesFromPeerSignal();

    void newFrameSignal(QImage* img);
    void onDevicesListChangedSignal(std::vector<std::string>);

    void requestNewFrameSignal();

private:
    QApplication* m_app;
    bool m_debug;
    AlManagerInterface* m_managerInterface;
    std::string m_peerId;
//    std::deque<std::string*> m_pendingMessages;
    QQueue<QMap<QString, QString>> m_messageQueue;

    QString m_remoteSDP;
    QQueue<QString> m_remoteICE;
    QString m_localSDP;
    QQueue<QString> m_localICE;

    int m_candidates;
    int m_remoteCandidates;
    int m_remoteCandidatesProcessed;

    bool m_processingMsg;
    QImage m_img;
    std::string m_videoDeviceName;
};

#endif // PEERCLIENTCALLBACK_H
