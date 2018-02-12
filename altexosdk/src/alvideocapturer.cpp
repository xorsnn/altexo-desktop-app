#include "alvideocapturer.h"
#include "libyuv/convert.h"
#include "webrtc/api/video/i420_buffer.h"
#include "webrtc/rtc_base/logging.h"
#include "webrtc/rtc_base/timeutils.h"
#include <stdlib.h>

using namespace cricket;

AlVideoCapturer::AlVideoCapturer() : m_pImageBytes(NULL) {
  m_width = 0;
  m_height = 0;
  m_imageDataLen = 0;
  m_pImageBytes = NULL;
}

AlVideoCapturer::~AlVideoCapturer() {
  if (m_pImageBytes) {
    free(m_pImageBytes);
    m_pImageBytes = NULL;
  }
}

bool AlVideoCapturer::GetBestCaptureFormat(const VideoFormat &desired,
                                           VideoFormat *best_format) {
  *best_format = desired; // TODO
  return true;
}

CaptureState AlVideoCapturer::Start(const VideoFormat &capture_format) {
  return CS_RUNNING;
}

void AlVideoCapturer::Stop() {}

bool AlVideoCapturer::IsRunning() { return true; }

bool AlVideoCapturer::IsScreencast() const {
  // TODO: is this appropriate here?
  return true;
}

bool AlVideoCapturer::GetPreferredFourccs(std::vector<uint32_t> *fourccs) {
  fourccs->push_back(FOURCC_RAW); // TODO take a look here
  return true;
}

void AlVideoCapturer::setImageData(uint8_t *pImageBytes, size_t len, int width,
                                   int height) {}

void AlVideoCapturer::setImageData(std::vector<unsigned char> imageBytes,
                                   int width, int height) {

  const uint8_t *array = &imageBytes[0];

  /*
   * Frame
   */

  // TODO: now we need to multiply by 2, not sure it should look like so
  rtc::scoped_refptr<webrtc::I420Buffer> buffer =
      webrtc::I420Buffer::Create(width * 2, height);

  libyuv::RGB24ToI420(array, width * 3 * 2, buffer->MutableDataY(),
                      buffer->StrideY(), buffer->MutableDataU(),
                      buffer->StrideU(), buffer->MutableDataV(),
                      buffer->StrideV(), width * 2, height);

  webrtc::VideoFrame frame(buffer, rtc::Time32(), 0,
                           webrtc::VideoRotation::kVideoRotation_0);

  OnFrame(frame, width, height);
}
