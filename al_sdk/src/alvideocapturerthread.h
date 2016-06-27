#ifndef ALVIDEOCAPTURERTHREAD_H
#define ALVIDEOCAPTURERTHREAD_H

#include "boost/thread.hpp"
#include <iostream>
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"

class AlVideoCapturerThread
{
public:
    AlVideoCapturerThread(cricket::VideoCapturer *pCapturer);
    ~AlVideoCapturerThread();

    void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);

private:
    void threadMain();

    cricket::VideoCapturer *m_capturer;
    boost::mutex m_imageMutex;

    boost::thread m_internalThread;

    uint8_t *m_pImageBytes;
    size_t m_imageDataLen;
    int m_width;
    int m_height;
};

#endif // ALVIDEOCAPTURERTHREAD_H
