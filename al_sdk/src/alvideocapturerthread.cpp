#include "alvideocapturerthread.h"
#include <boost/date_time/posix_time/posix_time.hpp>

AlVideoCapturerThread::AlVideoCapturerThread(
    cricket::VideoCapturer *pCapturer) {
  m_capturer = pCapturer;
  //    m_stopped = false;
  m_pImageBytes = 0;
  m_width = 0;
  m_height = 0;
  m_imageDataLen = 0;
  // this must be the last
  m_internalThread = boost::thread(&AlVideoCapturerThread::threadMain, this);
}

AlVideoCapturerThread::~AlVideoCapturerThread() {
  m_internalThread.interrupt();
  m_internalThread.join();
}

void AlVideoCapturerThread::threadMain() {
  try {
    bool stopped = false;
    uint64_t prevTimestamp = 0;
    std::cout << "tread body" << std::endl;
    while (!stopped) {
      cricket::CapturedFrame frame;
      frame.time_stamp = prevTimestamp;
      prevTimestamp += 33333333;

      m_imageMutex.lock();
      if (m_pImageBytes && m_imageDataLen > 0 && m_width > 0 && m_height > 0) {
        frame.data = m_pImageBytes;
        frame.width = m_width;
        frame.height = m_height;
        frame.fourcc = cricket::FOURCC_ABGR;
        frame.data_size = m_imageDataLen;

        if (frame.width * frame.height * 4 != m_imageDataLen)
          std::cout << "AlVideoCapturerThread: with and height don't match "
                       "size for ABGR data, skipping frame"
                    << std::endl;
        else {
          m_capturer->SignalFrameCaptured(m_capturer, &frame);
        }
      }
      m_imageMutex.unlock();

      boost::this_thread::sleep(boost::posix_time::milliseconds(33));

      //            m_stopMutex.lock();
      //            stopped = m_stopped;
      //            m_stopMutex.unlock();
    }
  } catch (boost::thread_interrupted &interruption) {
    // thread was interrupted, this is expected.
    std::cout << "tread interrupted" << std::endl;

  } catch (std::exception &e) {
    // an unhandled exception reached this point, this constitutes an error
    std::cout << "tread exception" << std::endl;
  }
} // threadMain

void AlVideoCapturerThread::setImageData(uint8_t *pImageBytes, size_t len,
                                         int width, int height) {
  m_imageMutex.lock();

  delete[] m_pImageBytes;

  m_pImageBytes = pImageBytes;
  m_imageDataLen = len;
  m_width = width;
  m_height = height;

  m_imageMutex.unlock();
}
