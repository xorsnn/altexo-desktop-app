#ifndef ALVIDEOCAPTURER_H
#define ALVIDEOCAPTURER_H
#include <QObject>
#include <QMutex>
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"
#include "alvideocapturerthread.h"

class AlVideoCapturer :  public cricket::VideoCapturer
{

public:
    AlVideoCapturer();

    bool GetBestCaptureFormat(const cricket::VideoFormat& desired, cricket::VideoFormat* best_format);
    cricket::CaptureState Start(const cricket::VideoFormat& capture_format);
    void Stop();
    bool IsRunning();
    bool IsScreencast() const;
    bool GetPreferredFourccs(std::vector<quint32>* fourccs);
    // XORS
    void OnMessage(rtc::Message* msg) {}

    void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);


private:
    uint64_t m_prevTimestamp;
    uint8_t *m_pImageBytes;
    size_t m_imageDataLen;
    int m_width;
    int m_height;

    AlVideoCapturerThread *m_captureThread;

};

#endif // ALVIDEOCAPTURER_H
