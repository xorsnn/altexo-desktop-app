#include "alconductor.h"
#include "talk/app/webrtc/test/fakeconstraints.h"
#include "talk/app/webrtc/videosourceinterface.h"
#include <string.h>
#include <QImage>
#include <QJsonDocument>

#include <QBuffer>
#include <QByteArray>

using namespace std;

class DummySetSessionDescriptionObserver : public webrtc::SetSessionDescriptionObserver
{
public:
    static DummySetSessionDescriptionObserver* Create()
    {
        return new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
    }

    void OnSuccess()
    {
        qDebug() << "DummySetSessionDescriptionObserver: onSuccess";
    }

    void OnFailure(const std::string& error)
    {
        qDebug() << "DummySetSessionDescriptionObserver::OnFailure";
        qDebug() << QString::fromUtf8(error.c_str());
    }
protected:
    DummySetSessionDescriptionObserver()
    {
    }

    ~DummySetSessionDescriptionObserver()
    {
    }
};

AlConductor::AlConductor(QObject *parent) :
    QObject(parent)
{
    qDebug() << "AlConductor::AlConductor";
    this->sdpProcessed = false;
}
AlConductor::~AlConductor()
{
    qDebug() << "AlConductor DESTRUCTOR";
}

bool AlConductor::connection_active() const {
//    qDebug() << "AlConductor::connection_active";
//  return peer_connection_.get() != NULL;
    return true;
}

cricket::VideoCapturer* AlConductor::OpenVideoCaptureDevice()
{
    qDebug() << "AlConductor::OpenVideoCaptureDevice";
    cricket::VideoCapturer* capturer = NULL;
    qDebug() << "test 1";

    this->videoCapturer_ = new AlVideoCapturer();
    capturer = this->videoCapturer_;
    qDebug() << "capturer = " << capturer;

    return capturer;
}

void AlConductor::addStreams() {
    qDebug() << "AlConductor::addStreams";
}

void AlConductor::slotProcessAnswer(QString sdpInfo) {
    qDebug() << "AlConductor::slotProcessAnswer";
    std::string sdpInfoStd = sdpInfo.toStdString();
    std::string type = "answer";
    webrtc::SdpParseError error;

    qDebug() << sdpInfo;
    qDebug() << "< 0.9";
    webrtc::SessionDescriptionInterface* pSessionDescription(
                webrtc::CreateSessionDescription("answer", sdpInfoStd, NULL));
    qDebug() << "< 1";
    this->peer_connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), pSessionDescription);
    qDebug() << "< 2";
    this->sdpProcessed = true;
    qDebug() << "< 3";
    this->processRemoteICEListSlot();
}

void AlConductor::slotProcessRemoteICELine(QJsonObject jsonMsg) {
    qDebug() << "AlConductor::slotProcessRemoteICELine";
    QJsonObject jsonObj = jsonMsg["candidate"].toObject();

    int sdpMLineIndex = jsonObj["sdpMLineIndex"].toInt();
    string sdpMid = jsonObj["sdpMid"].toString().toStdString();
    QString candidate = jsonObj["candidate"].toString();

    webrtc::SdpParseError error;

    candidate.replace(QString("\\r\\n"),QString(""));

    qDebug() << sdpMLineIndex;
    qDebug() << "ICE: " << QString(sdpMLineIndex) << ";" <<  QString::fromUtf8(sdpMid.c_str()) << ";" << candidate;

    rtc::scoped_ptr<webrtc::IceCandidateInterface> c(
                webrtc::CreateIceCandidate(sdpMid, sdpMLineIndex, candidate.toStdString(), NULL));

    if (!c.get())
    {
        qDebug() << "Can't parse candidate message";
    }
    else
    {
        if (!this->peer_connection_->AddIceCandidate(c.get()))
        {
            qDebug() << "Failed to apply the received candidate";
        }
        else {
            qDebug() << "YAHOOOO!!!!!";
        }
    }
}

void AlConductor::processRemoteICEListSlot() {
    qDebug() << "AlConductor::processRemoteICEListSlot";
    if (this->sdpProcessed) {
        while (!this->candidatesQueue.isEmpty()) {
            this->slotProcessRemoteICELine(this->candidatesQueue.dequeue());
        }
    }
}

void AlConductor::slotSetNewFrame(const QImage &img) {
    qDebug() << "AlConductor::slotSetNewFrame";
    // kind of bottleneck, need to take a look later
    QImage image = img.scaled(640,240);

    int width = image.width();
    int height = image.height();

    if (width <= 0 || height <= 0)
        return;
    if (this->videoCapturer_ == 0)
        return;

    int numBytes = width*height*4;
    uint8_t *pData = new uint8_t[numBytes];

    memcpy(pData, image.convertToFormat(QImage::Format_RGBA8888).bits(), numBytes);
    this->videoCapturer_->setImageData(pData, numBytes, width, height);
}

void AlConductor::StartAll() {
    qDebug() << "AlConductor::StartAll";
//    if (m_pcfIface != 0)
//    {
//        cerr << "onStartClicked: already exists" << endl;
//        return;
//    }
    this->peer_connection_factory_  = webrtc::CreatePeerConnectionFactory();
    if (!this->peer_connection_factory_.get()) // This means that the initialization failed for some reason (get retrieves the pointer)
    {
        this->peer_connection_factory_ = 0; // Since it's ref counted, this will destroy it
        return;
    }
    webrtc::PeerConnectionInterface::IceServer server;
    webrtc::FakeConstraints constraints;
    webrtc::PeerConnectionInterface::RTCConfiguration config;

//    constraints.SetAllowDtlsSctpDataChannels();
    bool dtls = true;
    if (dtls) {
        constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
            "true");
    } else {
        constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
            "false");
    }

    server.uri = "stun:stun.l.google.com:19302";
    config.servers.push_back(server);
//     TODO: this is a memory leak for now
////	TestConnectionObserver *pTestConnObserver = new TestConnectionObserver();
    this->peer_connection_ = this->peer_connection_factory_->CreatePeerConnection(config,
                                                   &constraints,
                                                   NULL,
                                                   NULL,
                                                   this);
    qDebug() << "OOOOOOO!!!!";
    qDebug() << this->peer_connection_;

//    const PeerConnectionInterface::RTCConfiguration& configuration,
//    const MediaConstraintsInterface* constraints,
//    rtc::scoped_ptr<cricket::PortAllocator> allocator,
//    rtc::scoped_ptr<DtlsIdentityStoreInterface> dtls_identity_store,
//    PeerConnectionObserver* observer) = 0;

    rtc::scoped_refptr<webrtc::VideoSourceInterface> videoSource;
    videoSource = this->peer_connection_factory_->CreateVideoSource(this->OpenVideoCaptureDevice(), 0);

    rtc::scoped_refptr<webrtc::VideoTrackInterface> videoTrack;
    videoTrack = this->peer_connection_factory_->CreateVideoTrack("testVideoTrack", videoSource);

    rtc::scoped_refptr<webrtc::AudioTrackInterface> audioTrack;
    audioTrack = this->peer_connection_factory_->CreateAudioTrack("testAudioTrack", this->peer_connection_factory_->CreateAudioSource(NULL));

    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream;
    stream = this->peer_connection_factory_->CreateLocalMediaStream("testStream");

    stream->AddTrack(audioTrack);
    stream->AddTrack(videoTrack);

    if (!this->peer_connection_->AddStream(stream))
    {
            qDebug() << "Adding stream to PeerConnection failed";
        return;
    }
    qDebug() << "Successfully added stream";

    Q_EMIT onInitSignal();

}

void AlConductor::startInitialExchangeSlot() {
    qDebug() << "AlConductor::startInitialExchangeSlot";
    this->peer_connection_->CreateOffer(this, NULL);
}

// **
// * PeerConnectionObserver implementation.
// *

// Called when a remote stream is added
void AlConductor::OnAddStream(webrtc::MediaStreamInterface* stream) {
    qDebug() << "AlConductor::OnAddStream";
}

void AlConductor::OnRemoveStream(webrtc::MediaStreamInterface* stream) {
    qDebug() << "AlConductor::OnRemoveStream";
}

void AlConductor::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
    qDebug() << "AlConductor::OnIceCandidate";
    string iceStr;

    int mlineIdx = candidate->sdp_mline_index();
    string sdpMid = candidate->sdp_mid();
    candidate->ToString(&iceStr);

    size_t len = iceStr.length();
    if (len > 0 && iceStr[len-1] == '\n')
        iceStr = iceStr.substr(0, len-1);

    len = iceStr.length();
    if (len > 0 && iceStr[len-1] == '\r')
        iceStr = iceStr.substr(0, len-1);

#define BUFLEN 1024
    char buf[BUFLEN];

    snprintf(buf, BUFLEN, "{\"sdpMLineIndex\":%d,\"sdpMid\":\"%s\",\"candidate\":\"%s\\r\\n\"}", mlineIdx, sdpMid.c_str(), iceStr.c_str());

    Q_EMIT this->signalOnLocalIceCandidate(QString(buf));
}

void AlConductor::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
    qDebug() << "AlConductor::OnSuccess";
    std::string sdpStr;

    desc->ToString(&sdpStr);
    qDebug() << QString(sdpStr.c_str());

    webrtc::SessionDescriptionInterface* pSessionDescription(webrtc::CreateSessionDescription("offer", sdpStr, NULL));
    this->peer_connection_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), pSessionDescription);

    Q_EMIT this->signalSDPText(QString(sdpStr.c_str()));
}

void AlConductor::OnFailure(const std::string& error) {
    qDebug() << "AlConductor::OnFailure";
}

void AlConductor::onJsonMsgSlot(QString msg)
{
    qDebug() << "AlConductor::onJsonMsgSlot";
//    TODO copy processing from main window
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject jsonObj = doc.object();
    if (jsonObj["type"].toString() == "SDP") {
        this->slotProcessAnswer(jsonObj["body"].toString());
    } else if (jsonObj["type"].toString() == "ICE") {
        //TODO process somehow!!!
//        this->slotProcessRemoteICE(jsonObj["body"].toString());
    } else if (jsonObj["id"].toString() == "iceCandidate") {
//        QJsonDocument doc(jsonObj["candidate"].toObject());
        this->candidatesQueue.enqueue(jsonObj);
        this->processRemoteICEListSlot();
//        this->slotProcessRemoteICELine(doc.toJson());
    } else if (jsonObj["id"].toString() == "presenterResponse") {
        this->slotProcessAnswer(jsonObj["sdpAnswer"].toString());
    }
}
