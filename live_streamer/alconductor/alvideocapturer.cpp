#include "alvideocapturer.h"
#include "talk/media/devices/deviceinfo.h"
#include "talk/media/base/mediacommon.h"
//XORS
#include "webrtc/system_wrappers/include/logging.h"
#include "talk/media/base/videoframe.h"
#include <QElapsedTimer>
#include <stdlib.h>
#include <QDebug>
#include <QDateTime>
using namespace cricket;

//AlVideoCapturer::AlVideoCapturer()
//{
//    this->isRunning = false;
//    this->m_pImageBytes = 0;
//    this->m_width = 0;
//    this->m_height = 0;
//    this->m_imageDataLen = 0;
//}
//bool AlVideoCapturer::GetBestCaptureFormat(const VideoFormat& desired, VideoFormat* best_format)
//{
//    LOG(LS_INFO) << "AlVideoCapturer::GetBestCaptureFormat";
//    *best_format = desired; // TODO
//    return true;
//}

//CaptureState AlVideoCapturer::Start(const VideoFormat& capture_format)
//{
//    LOG(LS_INFO) << "AlVideoCapturer::Start";

////    m_captureThread.start();
//    this->isRunning = true;

//    return CS_RUNNING;
//}

//void AlVideoCapturer::Stop()
//{
//    LOG(LS_INFO) << "AlVideoCapturer::Stop";
//    this->isRunning = false;
////    m_captureThread.stop();
//}

//bool AlVideoCapturer::IsRunning()
//{
//    LOG(LS_INFO) << "AlVideoCapturer::IsRunning";
////    return m_captureThread.isRunning();
////    return true;
//    this->isRunning;
//}

//bool AlVideoCapturer::IsScreencast() const
//{
//    //LOG(LS_INFO) << "AlVideoCapturer::IsScreencast";
//    // TODO: is this appropriate here?
//    return true;
//}

//bool AlVideoCapturer::GetPreferredFourccs(std::vector<quint32>* fourccs)
//{
//    fourccs->push_back(FOURCC_ABGR);
//    return true;
//}

//void AlVideoCapturer::setImageData(uint8_t *pImageBytes, size_t len, int width, int height)
//{
////    m_captureThread.setImageData(pImageBytes, len, width, height);

////    bool stopped = false;
////	uint64_t prevTimestamp = 0;
//    delete [] this->m_pImageBytes;

//    this->m_pImageBytes = pImageBytes;
//    this->m_imageDataLen = len;
//    this->m_width = width;
//    this->m_height = height;

////	while (!stopped)
////	{
//        CapturedFrame frame;

////		frame.elapsed_time = 33333333;
////        frame.time_stamp = prevTimestamp;
//        frame.time_stamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
////		prevTimestamp += frame.elapsed_time;  // 30 fps
////        prevTimestamp += 33333333;

////        m_imageMutex.lock();
//        if (this->m_pImageBytes && len > 0 && width > 0 && height > 0)
//        {
//            frame.data = this->m_pImageBytes;
//            frame.width = width;
//            frame.height = height;
//            frame.fourcc = FOURCC_ABGR;
//            frame.data_size = len;

//            if (frame.width*frame.height*4 != len)
//                LOG(LS_INFO) << "CaptureThread: with and height don't match size for ABGR data, skipping frame";
//            else
//            {

//                //LOG(LS_INFO) << "CaptureThread: got frame " << (int)frameVal << " glGetError: " << glGetError();
//                if (this->isRunning){
//                    qDebug() << "SENDING!!!!";
//                    this->SignalFrameCaptured(this, &frame);
//                }
//            }
//        }
////        m_imageMutex.unlock();

////        QThread::msleep(33);

////        m_stopMutex.lock();
////        stopped = m_stopped;
////        m_stopMutex.unlock();
////    }
//}


AlVideoCapturer::AlVideoCapturer() : m_captureThread(this)
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

    m_captureThread.start();

    return CS_RUNNING;
}

void AlVideoCapturer::Stop()
{
    LOG(LS_INFO) << "AlVideoCapturer::Stop";

    m_captureThread.stop();
}

bool AlVideoCapturer::IsRunning()
{
    LOG(LS_INFO) << "AlVideoCapturer::IsRunning";
    return m_captureThread.isRunning();
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
    m_captureThread.setImageData(pImageBytes, len, width, height);
}



