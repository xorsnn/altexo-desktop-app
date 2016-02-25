#include "alvideocapturerthread.h"
#ifndef Q_MOC_RUN
#include "talk/media/devices/deviceinfo.h"
#include "talk/media/base/mediacommon.h"
//XORS
#include "webrtc/system_wrappers/include/logging.h"
//#include "talk/base/logging.h"
#include "talk/media/base/videoframe.h"
#endif

#include <QElapsedTimer>
#include <stdlib.h>
#include <QDebug>

using namespace cricket;

AlVideoCapturerThread::AlVideoCapturerThread(VideoCapturer *pCapturer)
{
    m_pCapturer = pCapturer;
    m_stopped = false;
    m_pImageBytes = 0;
    m_width = 0;
    m_height = 0;
    m_imageDataLen = 0;
}

AlVideoCapturerThread::~AlVideoCapturerThread()
{
    this->stop();
}

void AlVideoCapturerThread::stop()
{
    QElapsedTimer timer;

    timer.start();

    m_stopMutex.lock();
    m_stopped = true;
    m_stopMutex.unlock();

    while (isRunning() && !timer.hasExpired(300)) // wait 300ms at most
    {
        QThread::msleep(50);
    }

    if (isRunning())
    {
        LOG(LS_INFO) << "AlVideoCapturerThread: KILLING THREAD";
        terminate();
    }
    else
    {
        LOG(LS_INFO) << "AlVideoCapturerThread: Thread stopped nicely";
    }

    delete [] m_pImageBytes;
    m_pImageBytes = 0;
}

void AlVideoCapturerThread::run()
{
    LOG(LS_INFO) << "AlVideoCapturerThread: STARTED";

    bool stopped = false;
    uint64_t prevTimestamp = 0;

    while (!stopped)
    {
        CapturedFrame frame;

//		frame.elapsed_time = 33333333;
        frame.time_stamp = prevTimestamp;
//		prevTimestamp += frame.elapsed_time;  // 30 fps
        prevTimestamp += 33333333;

        m_imageMutex.lock();
        if (m_pImageBytes && m_imageDataLen > 0 && m_width > 0 && m_height > 0)
        {
            frame.data = m_pImageBytes;
            frame.width = m_width;
            frame.height = m_height;
            frame.fourcc = FOURCC_ABGR;
            frame.data_size = m_imageDataLen;

            if (frame.width*frame.height*4 != m_imageDataLen)
                LOG(LS_INFO) << "AlVideoCapturerThread: with and height don't match size for ABGR data, skipping frame";
            else
            {
//                qDebug() << "test 2";
                //LOG(LS_INFO) << "AlVideoCapturerThread: got frame " << (int)frameVal << " glGetError: " << glGetError();
                m_pCapturer->SignalFrameCaptured(m_pCapturer, &frame);
            }
        }
        m_imageMutex.unlock();

        QThread::msleep(33);

        m_stopMutex.lock();
        stopped = m_stopped;
        m_stopMutex.unlock();
    }
}

void AlVideoCapturerThread::setImageData(uint8_t *pImageBytes, size_t len, int width, int height)
{
    m_imageMutex.lock();

    delete [] m_pImageBytes;

    m_pImageBytes = pImageBytes;
    m_imageDataLen = len;
    m_width = width;
    m_height = height;

    m_imageMutex.unlock();
}



