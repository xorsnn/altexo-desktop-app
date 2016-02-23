#ifndef ALCONDUCTOR_H
#define ALCONDUCTOR_H

#pragma once
#include <QObject>
#include <QDebug>

#include <deque>
#include <map>
#include <set>
#include <string>

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/scoped_ptr.h"

#include "alconductor/alvideocapturer.h"

class AlConductor : public QObject,
        public webrtc::PeerConnectionObserver,
        public webrtc::CreateSessionDescriptionObserver
{
    Q_OBJECT
public:

    AlConductor(QObject *parent = 0);
    ~AlConductor();

    bool connection_active() const;

    //
    // PeerConnectionObserver implementation.
    //
    virtual void OnStateChange(
            webrtc::PeerConnectionObserver::StateType state_changed) {}
    virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
    virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
    virtual void OnDataChannel(webrtc::DataChannelInterface* channel) {}
    virtual void OnRenegotiationNeeded() {}
    virtual void OnIceChange() {}
    virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);

    // CreateSessionDescriptionObserver implementation.
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    virtual void OnFailure(const std::string& error);

public Q_SLOTS:
    void StartAll();
    void addStreams();
    void slotProcessAnswer(QString sdpInfo);
    void slotProcessRemoteICE(QString iceStr);
    void slotProcessRemoteICELine(QString iceLine);
    void slotSetNewFrame(const QImage &img);

    void onJsonMsgSlot(QString msg);

protected:
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_;
    AlVideoCapturer *videoCapturer_;
private:
    cricket::VideoCapturer* OpenVideoCaptureDevice();

Q_SIGNALS:
    void signalSDPText(const QString &sdp);
    void signalOnLocalIceCandidate(const QString &iceCandidate);
};

#endif // ALCONDUCTOR_H
