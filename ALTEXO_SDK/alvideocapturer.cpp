#include "alvideocapturer.h"
//#include "talk/media/devices/deviceinfo.h"
//#include "talk/media/base/mediacommon.h"
//XORS
#include "webrtc/system_wrappers/include/logging.h"
//#include "talk/media/base/videoframe.h"
#include <QElapsedTimer>
#include <stdlib.h>
#include <QDebug>
#include <QDateTime>
using namespace cricket;

AlVideoCapturer::AlVideoCapturer() :
    m_prevTimestamp(0),
    m_pImageBytes(NULL)
{
}

bool AlVideoCapturer::GetBestCaptureFormat(const VideoFormat& desired, VideoFormat* best_format)
{
    LOG(LS_INFO) << "AlVideoCapturer::GetBestCaptureFormat";
    *best_format = desired; // TODO
    return true;
}

CaptureState AlVideoCapturer::Start(const VideoFormat& capture_format)
{
    LOG(LS_INFO) << "AlVideoCapturer::Start";

//    m_captureThread.start();

    return CS_RUNNING;
}

void AlVideoCapturer::Stop()
{
    LOG(LS_INFO) << "AlVideoCapturer::Stop";

//    m_captureThread.stop();
}

bool AlVideoCapturer::IsRunning()
{
    LOG(LS_INFO) << "AlVideoCapturer::IsRunning";
//    return m_captureThread.isRunning();
    return true;
}

bool AlVideoCapturer::IsScreencast() const
{
    //LOG(LS_INFO) << "AlVideoCapturer::IsScreencast";
    // TODO: is this appropriate here?
    return true;
}

bool AlVideoCapturer::GetPreferredFourccs(std::vector<quint32>* fourccs)
{
    fourccs->push_back(FOURCC_ABGR);
    return true;
}

void AlVideoCapturer::setImageData(uint8_t *pImageBytes, size_t len, int width, int height)
{
    if (m_pImageBytes != NULL) {
        delete [] m_pImageBytes;
    }

    m_pImageBytes = pImageBytes;
    m_imageDataLen = len;
    m_width = width;
    m_height = height;

//    m_captureThread.setImageData(pImageBytes, len, width, height);
    CapturedFrame frame;

//		frame.elapsed_time = 33333333;
    frame.time_stamp = m_prevTimestamp;
//    m_prevTimestamp += frame.elapsed_time;  // 30 fps
    m_prevTimestamp += 33333333;

//    m_imageMutex.lock();
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
            SignalFrameCaptured(this, &frame);
        }
    }
}



