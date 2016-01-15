#include "alconductor.h"
#include "talk/app/webrtc/test/fakeconstraints.h"
#include "talk/app/webrtc/videosourceinterface.h"
#include <string.h>
#include <QImage>

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
}
AlConductor::~AlConductor()
{
    qDebug() << "AlConductor DESTRUCTOR";
}

bool AlConductor::connection_active() const {
//  return peer_connection_.get() != NULL;
    return true;
}

cricket::VideoCapturer* AlConductor::OpenVideoCaptureDevice()
{
    cricket::VideoCapturer* capturer = NULL;
    qDebug() << "test 1";

    this->videoCapturer_ = new AlVideoCapturer();
    capturer = this->videoCapturer_;
    qDebug() << "capturer = " << capturer;

    return capturer;
}

void AlConductor::addStreams() {
}

void AlConductor::slotProcessAnswer(QString sdpInfo) {
    qDebug() << "AlConductor::slotProcessAnswer";
    std::string sdpInfoStd = sdpInfo.toStdString();
    std::string type = "answer";
    webrtc::SdpParseError error;

    qDebug() << sdpInfo;

    webrtc::SessionDescriptionInterface* pSessionDescription(
                webrtc::CreateSessionDescription("answer", sdpInfoStd, NULL));

    this->peer_connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), pSessionDescription);
}

void AlConductor::slotProcessRemoteICE(QString iceStr) {
    QStringList iceInfo = iceStr.split('\n');

    for (int i = 0 ; i < iceInfo.size() ; i++)
    {
        QString iceLine = iceInfo.at(i);

        //cerr << iceLine.toStdString() << endl;

        QStringList parts = iceLine.split(',');
        QStringList remainingParts;
        for (int j = 0 ; j < parts.size() ; j++)
        {
            QString part = parts.at(j);
            qDebug() << part;

            QStringList values = part.split('"', QString::SkipEmptyParts);

            if (values.size() > 2)
            {
                remainingParts << values.at(2);
                //cerr << values.at(0).toStdString() << " " << values.at(2).toStdString() << endl;
            }
        }

        if (remainingParts.size() == 3)
        {
            int sdpMLineIndex = atoi(remainingParts.at(0).toLatin1());
            string sdpMid = remainingParts.at(1).toStdString();
            QString candidate = remainingParts.at(2);

            webrtc::SdpParseError error;

            candidate.replace(QString("\\r\\n"),QString(""));

            qDebug() << "ICE: " << QString(sdpMLineIndex) << " " <<  QString::fromUtf8(sdpMid.c_str()) << " " << candidate;

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
    }
}

void AlConductor::slotSetNewFrame(const QImage &img) {
//    qDebug() << "ping";
//    img.save("res.png", "PNG");
    int width = img.width();
    int height = img.height();

    if (width <= 0 || height <= 0)
        return;
    if (this->videoCapturer_ == 0)
        return;

    int numBytes = width*height*4;
    uint8_t *pData = new uint8_t[numBytes];

    for (int y = 0 ; y < height ; y++)
    {
        for (int x = 0 ; x < width ; x++)
        {
            QRgb pixel = img.pixel(x,y);

            int offset = (x+y*width)*4;
            pData[offset+3] = qAlpha(pixel);
            pData[offset+2] = qBlue(pixel);
            pData[offset+1] = qGreen(pixel);
            pData[offset+0] = qRed(pixel);
        }
    }
    this->videoCapturer_->setImageData(pData, numBytes, width, height);


//    m_pCapturer->setImageData(pData, numBytes, width, height);
}

void AlConductor::StartAll() {
//    if (m_pcfIface != 0)
//    {
//        cerr << "onStartClicked: already exists" << endl;
//        return;
//    }
    qDebug() << "test 9.5";
//    m_pcfIface = webrtc::CreatePeerConnectionFactory();
    this->peer_connection_factory_  = webrtc::CreatePeerConnectionFactory();
    qDebug() << "test 9.6";
    if (!this->peer_connection_factory_.get()) // This means that the initialization failed for some reason (get retrieves the pointer)
    {
        this->peer_connection_factory_ = 0; // Since it's ref counted, this will destroy it
        return;
    }
    qDebug() << "test 10";
//    webrtc::PeerConnectionInterface::IceServers servers;
    webrtc::PeerConnectionInterface::IceServer server;
    webrtc::FakeConstraints constraints;
    qDebug() << "test 11";
    webrtc::PeerConnectionInterface::RTCConfiguration config;


    //XORS
    constraints.SetAllowDtlsSctpDataChannels();
//    constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
//                                "true");

    server.uri = "stun:stun.l.google.com:19302";
//    servers.push_back(server);
    config.servers.push_back(server);
    qDebug() << "test 12";
//     TODO: this is a memory leak for now
////	TestConnectionObserver *pTestConnObserver = new TestConnectionObserver();
    this->peer_connection_ = this->peer_connection_factory_->CreatePeerConnection(config,
                                                   &constraints,
                                                   NULL,
                                                   NULL,
                                                   this);
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

    this->peer_connection_->CreateOffer(this, NULL);
}

//
// PeerConnectionObserver implementation.
//
void AlConductor::OnAddStream(webrtc::MediaStreamInterface* stream) {
    qDebug() << "AlConductor::OnAddStream";
}

void AlConductor::OnRemoveStream(webrtc::MediaStreamInterface* stream) {
    qDebug() << "AlConductor::OnRemoveStream";
}

void AlConductor::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
    qDebug() << "===========================";
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
    qDebug() << "test 5!!!!";

    webrtc::SessionDescriptionInterface* pSessionDescription(webrtc::CreateSessionDescription("offer", sdpStr, NULL));
    this->peer_connection_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), pSessionDescription);
    Q_EMIT this->signalSDPText(QString(sdpStr.c_str()));
}

void AlConductor::OnFailure(const std::string& error) {
    qDebug() << "AlConductor::OnFailure";

}
