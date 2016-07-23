#include "alvideocapturer.h"
#include "webrtc/system_wrappers/include/logging.h"
#include <stdlib.h>

using namespace cricket;

AlVideoCapturer::AlVideoCapturer()
    : m_prevTimestamp(0), m_pImageBytes(NULL), m_debug(true) {
  // m_stopped = false;
  m_width = 0;
  m_height = 0;
  m_imageDataLen = 0;
  m_pImageBytes = (unsigned char *)malloc(1280 * 480 * 3);
  // this must be the last
  m_internalThread = boost::thread(&AlVideoCapturer::threadMain, this);
}

AlVideoCapturer::~AlVideoCapturer() {
  if (m_pImageBytes) {
    free(m_pImageBytes);
    m_pImageBytes = NULL;
  }
  m_internalThread.interrupt();
  m_internalThread.join();
}

bool AlVideoCapturer::GetBestCaptureFormat(const VideoFormat &desired,
                                           VideoFormat *best_format) {
  LOG(LS_INFO) << "AlVideoCapturer::GetBestCaptureFormat";
  *best_format = desired; // TODO
  return true;
}

CaptureState AlVideoCapturer::Start(const VideoFormat &capture_format) {
  LOG(LS_INFO) << "AlVideoCapturer::Start";

  // m_captureThread.start();

  return CS_RUNNING;
}

void AlVideoCapturer::Stop() {
  LOG(LS_INFO) << "AlVideoCapturer::Stop";

  //    m_captureThread.stop();
}

bool AlVideoCapturer::IsRunning() {
  LOG(LS_INFO) << "AlVideoCapturer::IsRunning";
  //    return m_captureThread.isRunning();
  return true;
}

bool AlVideoCapturer::IsScreencast() const {
  // LOG(LS_INFO) << "AlVideoCapturer::IsScreencast";
  // TODO: is this appropriate here?
  return true;
}

bool AlVideoCapturer::GetPreferredFourccs(std::vector<uint32_t> *fourccs) {
  // fourccs->push_back(FOURCC_ABGR);
  fourccs->push_back(FOURCC_RAW); // TODO take a look here
  return true;
}

void AlVideoCapturer::threadMain() {
  if (m_debug) {
    std::cout << "===========================" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "AlVideoCapturer::threadMain" << std::endl;
    std::cout << "===========================" << std::endl;
  }
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
        frame.fourcc = cricket::FOURCC_RAW;
        frame.data_size = m_imageDataLen;

        if (frame.width * frame.height * 3 != m_imageDataLen)
          std::cout << "AlVideoCapturerThread: with and height don't match "
                       "size for ABGR data, skipping frame"
                    << std::endl;
        else {
          SignalFrameCaptured(this, &frame);
        }
      }
      m_imageMutex.unlock();

      boost::this_thread::sleep(boost::posix_time::milliseconds(33));

      // m_stopMutex.lock();
      // stopped = m_stopped;
      // m_stopMutex.unlock();
    }
  } catch (boost::thread_interrupted &interruption) {
    // thread was interrupted, this is expected.
    std::cout << "tread interrupted" << std::endl;

  } catch (std::exception &e) {
    // an unhandled exception reached this point, this constitutes an error
    std::cout << "tread exception" << std::endl;
  }
} // threadMain

void AlVideoCapturer::setImageData(uint8_t *pImageBytes, size_t len, int width,
                                   int height) {
  // TODO: check if nesessary
}

void AlVideoCapturer::setImageData(std::vector<unsigned char> imageBytes,
                                   int width, int height) {
  m_imageMutex.lock();
  std::copy(imageBytes.begin(), imageBytes.end(), m_pImageBytes);
  m_imageDataLen = 1280 * 480 * 3;
  m_width = width;
  m_height = height;
  m_imageMutex.unlock();
}
