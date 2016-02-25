#ifndef ALVIDEOCAPTURERTHREAD_H
#define ALVIDEOCAPTURERTHREAD_H
#include <QThread>
#include <QMutex>
#include <QObject>
#include "talk/media/base/videocapturer.h"

class AlVideoCapturerThread : public QThread
{
    Q_OBJECT
public:
    AlVideoCapturerThread(cricket::VideoCapturer *pCapturer);
    ~AlVideoCapturerThread();
    void stop();
    void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);
private:
    void run();

    cricket::VideoCapturer *m_pCapturer;
    bool m_stopped;
    QMutex m_stopMutex;
    QMutex m_imageMutex;

    uint8_t *m_pImageBytes;
    size_t m_imageDataLen;
    int m_width;
    int m_height;
};

#endif // ALVIDEOCAPTURERTHREAD_H
