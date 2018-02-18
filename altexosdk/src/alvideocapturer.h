#ifndef ALVIDEOCAPTURER_H
#define ALVIDEOCAPTURER_H

#include "media/base/videocapturer.h"
#include "rtc_base/nethelpers.h"
#include "rtc_base/physicalsocketserver.h"
#include "rtc_base/signalthread.h"
#include "rtc_base/sigslot.h"

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

  void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);
  void setImageData(std::vector<unsigned char> imageBytes, int width,
                    int height);

private:
  uint8_t *m_pImageBytes;
  size_t m_imageDataLen;
  int m_width;
  int m_height;
};

#endif // ALVIDEOCAPTURER_H
