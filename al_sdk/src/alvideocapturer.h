#ifndef ALVIDEOCAPTURER_H
#define ALVIDEOCAPTURER_H

#include "webrtc/media/base/videocapturer.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

class AlVideoCapturer : public cricket::VideoCapturer {

public:
  AlVideoCapturer();
  ~AlVideoCapturer();

  bool GetBestCaptureFormat(const cricket::VideoFormat &desired,
                            cricket::VideoFormat *best_format);
  cricket::CaptureState Start(const cricket::VideoFormat &capture_format);
  void Stop();
  bool IsRunning();
  bool IsScreencast() const;
  bool GetPreferredFourccs(std::vector<uint32_t> *fourccs);
  // XORS
  void OnMessage(rtc::Message *msg) {}

  void threadMain();
  void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);
  void setImageData(std::vector<unsigned char> imageBytes, int width,
                    int height);

  // bool apply_rotation() { return apply_rotation_; }
  // virtual bool GetPreferredFourccs(std::vector<uint32_t> *fourccs) = 0;

private:
  uint64_t m_prevTimestamp;
  uint8_t *m_pImageBytes;
  size_t m_imageDataLen;
  int m_width;
  int m_height;

  boost::mutex m_imageMutex;
  boost::thread m_internalThread;

  bool m_debug;
};

#endif // ALVIDEOCAPTURER_H
