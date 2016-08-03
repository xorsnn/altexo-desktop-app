#ifndef ALLOCALVIDEORENDERER_H
#define ALLOCALVIDEORENDERER_H

#include "chromium/src/breakpad/src/common/scoped_ptr.h"
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"

#include "alcallback.h"

class AlLocalVideoRenderer
    : public rtc::VideoSinkInterface<cricket::VideoFrame> {
public:
  AlLocalVideoRenderer(const int target,
                       webrtc::VideoTrackInterface *track_to_render,
                       AlCallback *alCallback);
  virtual ~AlLocalVideoRenderer();

  // VideoSinkInterface implementation
  void OnFrame(const cricket::VideoFrame &frame) override;
  const uint8_t *image() const { return m_image.get(); }
  int width() const { return m_width; }
  int height() const { return m_height; }

protected:
  void SetSize(int width, int height);
  google_breakpad::scoped_ptr<uint8_t> m_image;
  int m_width;
  int m_height;
  rtc::scoped_refptr<webrtc::VideoTrackInterface> m_rendered_track;
  const int m_nameReceiver;
  AlCallback *m_alCallback;
};

#endif // ALLOCALVIDEORENDERER_H
